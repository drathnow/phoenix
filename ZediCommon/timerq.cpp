/** @file timerq.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 22, 2015 by daver:
 * @endhistory
 */
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "TimeUnit.h"
#include "MonotonicTime.h"
#include "timerq.h"
#include "Thread.h"

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;

static const Logger logger = Logger::getInstance("zios.common.TimerQueue");

AtomicInteger TimerQueueEntry::_uniqueIdGenerator;

TimerQueueEntry::TimerQueueEntry(int64_t dueTime, TimerTask* timerTask) :
        _dueTime(dueTime),
        _timerTask(timerTask),
        _uniqueId((tqe_id_t)_uniqueIdGenerator.incrementAndGet()),
        _cancelled(false) {
}

TimerQueueEntry::~TimerQueueEntry() {
    delete _timerTask;
}

bool TimerQueueEntry::operator==(const TimerQueueEntry& otherEntry) const {
    return this == &otherEntry;
}

void TimerQueueEntry::execute() {
    try {
        _timerTask->run();
    } catch (...) {
        LOG4CPLUS_ERROR(logger, "Unhandled exception was thrown from TimerTask!!");
    }
    delete _timerTask;
    _timerTask = NULL;
}

PrivatePriorityQueue::PrivatePriorityQueue() {
}

PrivatePriorityQueue::~PrivatePriorityQueue() {
}

void PrivatePriorityQueue::removeTQE(tqe_id_t tqeId) {
    for (vector<TimerQueueEntry*>::iterator iter = c.begin(); iter != c.end(); ++iter) {
        if ((*iter)->uniqueId() == tqeId) {
            TimerQueueEntry* tqe = *iter;
            c.erase(iter);
            delete tqe;
            return;
        }
    }
}

TimerQueue::TimerQueue(MonotonicTime* monotonicTime, ConditionVariable* conditionVariable) :
        _shutdown(false),
        _monotonicTime(monotonicTime == NULL ? new MonotonicTime() : monotonicTime),
        _conditionVariable(conditionVariable){
}

TimerQueue::~TimerQueue() {
    if (_monotonicTime != &MonotonicTime::globalSystemTime)
        delete _monotonicTime;
    while (_privateQueue.empty() == false) {
        delete _privateQueue.top();
        _privateQueue.pop();
    }
    delete _conditionVariable;
}

void TimerQueue::insertTQE(TimerQueueEntry* timerQueueEntry) {
    _conditionVariable->lock();
    _privateQueue.push(timerQueueEntry);
    _conditionVariable->signal();
    _conditionVariable->unlock();
}

void TimerQueue::removeTQEWithId(tqe_id_t id) {
    _conditionVariable->lock();
    _privateQueue.removeTQE(id);
    _conditionVariable->signal();
    _conditionVariable->unlock();
}

void TimerQueue::shutdown() {
    _conditionVariable->lock();
    _shutdown = true;
    _conditionVariable->signal();
    _conditionVariable->unlock();
}

void TimerQueue::processEntriesOrWait() {
    bool done = false;
    TimerQueueEntry* tqe = NULL;

    //
    // Check the queue to see if any TQEs are due for execution.  We need to
    // aquire the lock while we check the queue.  Once we have a TQE to execute,
    // we release the lock and execute the TQE. This is done to avoid possible deadlock
    // that could be caused by a timer task calling back into the timer service.
    //
    while (false == done) {
        _conditionVariable->lock();
        if (false == _privateQueue.empty()) {
            tqe = _privateQueue.top();
            if (tqe->isCancelled()) {
                _privateQueue.pop();
                delete tqe;
                tqe = NULL;
            } else if (tqe->dueTime() <= _monotonicTime->getTimeMsec())
                _privateQueue.pop();
            else {
                done = true;
                tqe = NULL;
            }
        } else
            done = true;
        _conditionVariable->unlock();

        //
        // Execute the TQE outside the lock so TimerTasks can call back into the TimerService
        // if they like.
        //
        if (NULL != tqe) {
            try {
                tqe->execute();
            } catch (...) {
                LOG4CPLUS_ERROR(logger, "Unhandled exception caught from TimerQueueEntry->execute().  Execution will continue.");
            }
            delete tqe;
            tqe = NULL;
        }
    }

    if (false == _shutdown) {

        //
        // Lock again and then decide how long we have to wait.  If the queue is empty, then we wait until
        // something wakes us up. Otherwise, we take our wait time from the first entry in the queue.
        //
        _conditionVariable->lock();
        int64_t dueTime = _privateQueue.empty() ? 0 : _privateQueue.top()->dueTime();
        if (dueTime == 0)
            _conditionVariable->wait();
        else if (dueTime > _monotonicTime->getTimeMsec()) {
            int64_t diff = dueTime - _monotonicTime->getTimeMsec();
            if (diff > 0)
                _conditionVariable->wait(diff);

        }
        _conditionVariable->unlock();
    }
}

void TimerQueue::mainLoop() {
    while (false == _shutdown)
        processEntriesOrWait();
}

TimerServiceProvider::TimerServiceProvider(TimerQueue* timerQueue) :
        _timerThread(NULL),
        _timerQueue(timerQueue),
        _started(false) {
}

TimerServiceProvider::~TimerServiceProvider() {
    delete _timerQueue;
    delete _timerThread;
}

tqe_id_t TimerServiceProvider::schedule(TimerTask* timerTask, int64_t delay, const TimeUnit& timeUnit) {
    MonotonicTime monotonicTime;
    int64_t dueTime = monotonicTime.getTimeMsec() + timeUnit.toMillis(delay);
    TimerQueueEntry* tqe = new TimerQueueEntry(dueTime, timerTask);
    tqe_id_t id = tqe->uniqueId();
    _timerQueue->insertTQE(tqe);
    return id;
}

void TimerServiceProvider::cancel(tqe_id_t tqeId) {
    _timerQueue->removeTQEWithId(tqeId);
}

void TimerServiceProvider::shutdown() {
    _timerQueue->shutdown();
    _timerThread->join(10);
}

void TimerServiceProvider::start() {
    if (_timerThread == NULL) {
        _timerThread = new Thread(*this);
        _timerThread->start();
    }
}

void TimerServiceProvider::run() {
    LOG4CPLUS_INFO(logger, "TimerService thread starting");
    _timerQueue->mainLoop();
    LOG4CPLUS_INFO(logger, "TimerService thread exiting");
}


} /* namespace net */
} /* namespace zios */
