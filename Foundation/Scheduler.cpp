#include <iostream>
#include <log4cplus/log4cplus.h>

#include "Clock.h"
#include "Scheduler.h"
#include "EventSchedule.h"

using namespace std;
using namespace log4cplus;

namespace dios::foundation
{



class JobWrapper: public Job
{
public:
    JobWrapper(Job &job, EventSchedule&& eventSchedule, Scheduler &scheduler) :
            _job(job),
            _eventSchedule(eventSchedule),
            _scheduler(scheduler),
            _startTime(0),
            _elapsedTime(0)
    {
    }

    ~JobWrapper()
    {
        delete &_job;
    }

    void execute() noexcept
    {
        _startTime = MonotonicClock::globalMonotonicClock.currentTime();
        try
        {
            _job.execute();
        } catch (exception& e)
        {
            LOG4CPLUS_ERROR(Logger::getInstance("dios.foundation.scheduler"), "Unhandled exception from job: " << e.what());
        }

        _elapsedTime = MonotonicClock::globalMonotonicClock.currentTime() - _startTime;
        _scheduler._handleJobCompletion(this);
    }

    const EventSchedule& eventSchedule()
    {
        return _eventSchedule;
    }

    int32_t elapsedTimeSeconds() const
    {
        return _elapsedTime;
    }

private:
    Job &_job;
    EventSchedule _eventSchedule;
    Scheduler &_scheduler;
    time_t _startTime;
    int32_t _elapsedTime;
};

Scheduler::Scheduler(WatchdogClient &&watchdogClient) :
        _shutdown
        { true }, _thread
        { nullptr }, _watchdogClient
        { watchdogClient }
{
    _watchdogClient.registerThread("Scheduler", WATCHDOG_TIMEOUT_SECONDS * 2);
}

Scheduler::~Scheduler()
{
    delete _thread;
}

uint32_t Scheduler::submit(Job *job)
{
    return submit(job, 0, 0, 0);
}

uint32_t Scheduler::submit(Job *job, time_t startTime)
{
    return submit(job, startTime, 0, 0);
}

uint32_t Scheduler::submit(Job *job, time_t startTime, int intervalSeconds)
{
    return submit(job, startTime, intervalSeconds, 0);
}

uint32_t Scheduler::submit(Job *job, time_t startTime, int intervalSeconds, int durationSeconds)
{
    uint32_t uid = 0;

    time_t diff = startTime - SystemClock::globalSystemTime.currentTime();
    time_t monoDueTime = MonotonicClock::globalMonotonicClock.currentTime() + diff;
    EventSchedule eventSchedule(monoDueTime, intervalSeconds, durationSeconds, &MonotonicClock::globalMonotonicClock);
    monoDueTime = eventSchedule.nextDueTime();
    if (monoDueTime > 0)
    {
        JobWrapper *jobWrapper = new JobWrapper(*job, std::move(eventSchedule), *this);
        uid = _jobQueue.submit(*jobWrapper, monoDueTime);
    }
    return uid;
}

void Scheduler::start()
{
    std::lock_guard<mutex> lock(_mutex);
    if (_shutdown && _thread == nullptr)
    {
        _shutdown = false;
        _thread = new std::thread(&Scheduler::_run, this);
    }
}

void Scheduler::shutdown(int timeoutSeconds)
{
    std::lock_guard<mutex> lock(_mutex);
    _shutdown = true;
    _jobQueue.wake();
    _thread->join();
}

void Scheduler::_mainLoop(int timeoutSeconds)
{
    JobWrapper *jobWrapper = dynamic_cast<JobWrapper*>(_jobQueue.nextDueJob(timeoutSeconds));
    if (jobWrapper != nullptr)
    {
        try
        {
            _threadPool.submit<void>(&JobWrapper::execute, jobWrapper); // @suppress("Return value not evaluated")
        } catch (const std::exception &e)
        {
            std::cerr << "Unexpected exception from job: " << e.what() << '\n';
        }
    }
}

void Scheduler::_handleJobCompletion(JobWrapper* jobWrapper)
{
    time_t nextDueTime = 0;
    if (jobWrapper->elapsedTimeSeconds() == 0)
    {
        time_t now = MonotonicClock::globalMonotonicClock.currentTime();
        nextDueTime = jobWrapper->eventSchedule().nextDueTimeFromTime(now+1);
    }
    else
    {
        nextDueTime = jobWrapper->eventSchedule().nextDueTime();
    }

    if (nextDueTime != 0)
        _jobQueue.submit(*jobWrapper, nextDueTime);
    else
        delete jobWrapper;
}


void Scheduler::_run()
{
    _watchdogClient.start();
    time_t now = MonotonicClock::globalMonotonicClock.currentTime();
    int nextWatchdogKickTime = now + WATCHDOG_TIMEOUT_SECONDS;

    while (!_shutdown)
    {
        if (now >= nextWatchdogKickTime)
        {
            _watchdogClient.kick();
            nextWatchdogKickTime = now + WATCHDOG_TIMEOUT_SECONDS;
        }
        _mainLoop(nextWatchdogKickTime - now);
        now = MonotonicClock::globalMonotonicClock.currentTime();
    }
    _watchdogClient.stop();
}

} // namespace dios::sched
