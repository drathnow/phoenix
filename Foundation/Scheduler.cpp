#include <iostream>

#include "Clock.h"
#include "Scheduler.h"
#include "EventSchedule.h"

using namespace std;

namespace zios::foundation
{

class JobWrapper: public Job
{
public:
    JobWrapper(Job &job, EventSchedule* eventSchedule) :
            _job(job), _eventSchedule(eventSchedule)
    {
    }

    ~JobWrapper()
    {
        delete &_job;
        delete _eventSchedule;
    }

    void execute()
    {
        _job.execute();
    }

    const EventSchedule& eventSchedule()
    {
        return *_eventSchedule;
    }

private:
    Job &_job;
    EventSchedule* _eventSchedule;
};

Scheduler::Scheduler() :
        _shutdown { true }, _thread { nullptr }
{
}

Scheduler::~Scheduler()
{
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
    time_t diff = startTime - SystemClock::globalSystemTime.currentTime();
    time_t monoDueTime = MonotonicClock::globalMonotonicClock.currentTime() + diff;
    EventSchedule* eventSchedule = new EventSchedule(monoDueTime, intervalSeconds, durationSeconds);
    JobWrapper *jobWrapper = new JobWrapper(*job, eventSchedule);
    return _jobQueue.submit(*jobWrapper, monoDueTime);
}

void Scheduler::start()
{
    std::lock_guard<mutex> lock(_mutex);
    if (_shutdown && _thread == nullptr)
    {
        _shutdown = false;
        _thread = new std::thread(&Scheduler::run, this);
    }
}

void Scheduler::shutdown(int timeoutSeconds)
{
    std::lock_guard<mutex> lock(_mutex);
    _shutdown = true;
    _jobQueue.wake();
    _thread->join();
}

void Scheduler::mainLoop()
{
    JobWrapper *jobWrapper = dynamic_cast<JobWrapper*>(_jobQueue.nextDueJob());
    if (jobWrapper != nullptr)
    {
        try
        {
            jobWrapper->execute();
            time_t nextDueTime = jobWrapper->eventSchedule().nextDueTime();
            if (nextDueTime != 0)
                _jobQueue.submit(*jobWrapper, nextDueTime);
            else
                delete jobWrapper;
        } catch (const std::exception &e)
        {
            std::cerr << "Unexpected exception from job: " << e.what() << '\n';
        }

    }
}

void Scheduler::run()
{
    while (!_shutdown)
    {
        mainLoop();
    }
}

} // namespace zios::sched
