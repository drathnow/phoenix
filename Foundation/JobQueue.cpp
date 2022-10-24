#include <chrono>
#include <queue>
#include <iostream>
#include <ctime>
#include <iomanip>

#include "JobQueue.h"

namespace zios::foundation
{

using namespace std;
using namespace std::chrono;


JobQueue::JobQueue(Clock& clock) : _nextId(0), _clock(clock)
{
}

JobQueue::~JobQueue()
{
    while (!_theQueue.empty())
    {
        delete _theQueue.top().job;
        _theQueue.pop();
    }
}

Job* JobQueue::nextDueJob()
{
    Job* job = nullptr;
    
    std::unique_lock<mutex> lock(_mutex, std::try_to_lock);
    if (!lock.owns_lock())
        return job;

    if (_theQueue.empty())
    {
        _conditionVariable.wait(lock);
    }

    while (!_theQueue.empty())
    {
        if (_theQueue.top().isDue(_clock))
        {
            job = _theQueue.top().job;
            _theQueue.pop();
            break;
        }

        _conditionVariable.wait_until(lock, std::chrono::system_clock::from_time_t(_theQueue.top().dueTime));

        //
        // Have to do this check again just in case something woke us from our slumber by signalling the condition
        // variable.  If the top element isn't due, then we return null.
        //
        if (!_theQueue.top().isDue(_clock))
            break;
    }

    return job;
}

Job* JobQueue::remove(uint32_t uniqueJobId)
{
    std::unique_lock<mutex> lock(_mutex);

    vector<tqe>::iterator iter = _theQueue.begin();
    while (iter != _theQueue.end())
    {
        if (iter->uniqueId == uniqueJobId)
        {
            Job* job = iter->job;
            _theQueue.erase(iter);
            return job;
        }
    }

    return nullptr;
}

void JobQueue::wake()
{
    std::unique_lock<mutex> lock(_mutex);
    _conditionVariable.notify_all();
}

uint32_t JobQueue::submit(Job& job, time_t dueTime)
{
    lock_guard<mutex> lock(_mutex);

    time_t now = _clock.currentTime();
    time_t diff = dueTime - now;
    time_t due = now + diff;

    _theQueue.push(tqe(due, &job, nextId()));
    _conditionVariable.notify_all();
    
    return 0;
}

uint32_t JobQueue::nextId()
{
    if (_nextId == UINT32_MAX)
        _nextId = 1;
    return _nextId++;

}

}
