#ifndef __JOBQUEUE_H_
#define __JOBQUEUE_H_

#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include "Clock.h"
#include "ExtendedPriorityQueue.h"

namespace zios::foundation
{

class Job
{
public:
    virtual ~Job() {}

    virtual void execute() = 0;
};

struct tqe
{
    time_t dueTime;
    Job *job;
    uint32_t uniqueId;

    tqe(time_t t, Job *j, uint32_t uid) :
            dueTime { t }, job { j }, uniqueId(uid)
    {
    }

    bool isDue(Clock &clock) const
    {
        return clock.currentTime() > dueTime;
    }

    friend bool operator>(const tqe &lhs, const tqe &rhs)
    {
        return lhs.dueTime >= rhs.dueTime;
    }
};

class JobQueue
{
public:
    JobQueue(Clock &clock = MonotonicClock::globalMonotonicClock);
    ~JobQueue();

    Job* nextDueJob();
    uint32_t submit(Job &job, time_t dueTime);
    Job* remove(uint32_t uniqueJobId);
    void wake();

    friend class JobQueueTest;

private:
    uint32_t _nextId;
    Clock &_clock;
    ExtendedPriorityQueue<tqe> _theQueue;
    std::mutex _mutex;
    std::condition_variable _conditionVariable;

    uint32_t nextId();
};

}
#endif // __JOBQUEUE_H_
