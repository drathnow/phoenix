/** @file timerq.h
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
#ifndef TIMERQ_H_
#define TIMERQ_H_
#include <vector>
#include <queue>
#include <stdint.h>

#include "Utl.h"
#include "AtomicInteger.h"
#include "timerq.h"
#include "ConditionVariable.h"
#include "Runnable.h"
#include "TimeUnit.h"
#include "MonotonicTime.h"

namespace zios {
namespace common {

class Thread;

typedef int   tqe_id_t;

/** class TimerTask
 *
 * A TimerTask is the executable interface used by Timers. When a timer expires, the TimerTask run() method
 * will be called.  By default, TimerTasks are cleaned up by the Timer Service so when a TimerTask is scheduled
 * for execution, it should not be accessed by the client.  The exception is if the TimerTask is passed to
 * the Timer Service and it is told not to delete the timer task.  See the "schedule" method on TimerService.
 *
 * TimerTasks are invoked using a single thread, while holding an internal lock within the Timer Service.  Clients
 * need to ensure that when passing a TimerTask to the Timer Service that no deadlocks can ocurr.
 *
 */
class TimerTask : public Runnable {
public:
    virtual ~TimerTask() {}

    /**
     * Invoked to execute the TimerTask function.
     */
    virtual void run() = 0;
};

/**
 * An entry to put in a TimerQueue that holds information about a TimerTask to be
 * executed at a future time.
 *
 */
class TimerQueueEntry : zios::common::Noncopyable {
public:
    /**
     * Constructs a TimerQueueEntry
     *
     * @param dueTime - The absolute time (i.e. wall clock time) this entry is due.
     *
     * @param timerTask - Timer task to execute.  This is a transfer of ownership.
     */
    TimerQueueEntry(int64_t dueTime, TimerTask* timerTask);
    virtual ~TimerQueueEntry();

    virtual void execute();
    virtual inline int64_t dueTime() const { return _dueTime; }
    virtual inline TimerTask* timerTask() const { return _timerTask; }
    virtual inline tqe_id_t uniqueId() const { return _uniqueId; }
    virtual inline bool isCancelled() const { return _cancelled; }

    bool operator==(const TimerQueueEntry& otherEntry) const;

private:
    int64_t _dueTime;
    TimerTask* _timerTask;
    tqe_id_t _uniqueId;
    bool _cancelled;

    static AtomicInteger _uniqueIdGenerator;
};


/**
 * Class that provider comparison operator between two TimerQueueEntries.
 */
class CompareTQE {
public:
    CompareTQE() {}
    ~CompareTQE() {}

    inline bool operator()(TimerQueueEntry* tqe1, TimerQueueEntry* tqe2) {
        return tqe1->dueTime() > tqe2->dueTime();
    }
};

/**
 * A private priority queue used to hold TimerQueueEntries that are sorted by their due time.
 */
class PrivatePriorityQueue : public std::priority_queue<TimerQueueEntry*, std::vector<TimerQueueEntry*>, CompareTQE> {
public:
    PrivatePriorityQueue();
    ~PrivatePriorityQueue();

    void removeTQE(tqe_id_t tqeId);
};

/**
 * A TimerQueue maintains an internal queue of TimerQueueEntries and executes them as they become due.
 */
class TimerQueue : zios::common::Noncopyable {
public:
    TimerQueue(MonotonicTime* MonotonicTime = &MonotonicTime::globalSystemTime,
               ConditionVariable* conditionVariable = new ConditionVariable());
    virtual ~TimerQueue();

    void insertTQE(TimerQueueEntry* timerQueueEntry);
    void removeTQEWithId(tqe_id_t id);
    void shutdown();

    void mainLoop();

    friend class TimerQueueTest;

protected:
    virtual void processEntriesOrWait();

private:
    bool _shutdown;
    zios::common::MonotonicTime* _monotonicTime;
    ConditionVariable* _conditionVariable;
    PrivatePriorityQueue _privateQueue;
};


/**
 * The TimerSevice is an interface used to schedule events to be executed in the future.
 *
 */
class TimerService {
public:
    TimerService() {}
    virtual ~TimerService() {}

    /**
     * Schedules a TimerTask for execution in the future.
     *
     * @param timerTask - The task to be executed. This is a transfer of ownerthip.  The
     * timerTask object will be deleted after execution completes
     *
     * @param delay - Time to delay before the timer task should be executed
     *
     * @param timeUnit - The unit of time for the delay argument
     *
     * @return  A unique id to identify timer queue entry for this TimerTask. As stated above, if
     * the deleteTask flag is set to true, this object will take ownership of the TimerTask.  If the
     * flag is set to false, the client MUST NOT delete this object until it has been cancelled by
     * calling the cancel() method and passing the unique id returned by this method.
     */
    virtual tqe_id_t schedule(TimerTask* timerTask, int64_t delay, const TimeUnit& timeUnit) = 0;

    /**
     * Cancels a TimerTask associated by the tqeId.  AFTER this method is called, the client
     * can delete the TimerTask if it was scheduled with the deleteTask flag set to false.  If
     * the TimerTask was scheduled with the deleteTask set to true, the caller need take no further
     * action as the TimerTask will be deleted after it is removed from the queue.
     *
     * Each call to schedule returns a unique identifier, weather or not the TimerTask is scheduled
     * more than once. If you wish to remove a TimerTask that was scheduled multiple times, you must
     * call cancel for each unique ID that was returned by the schedule method.
     *
     * @param tqeId - A unique identifer that identifies the TimerTask.
     */
    virtual void cancel(tqe_id_t tqeId) = 0;
};

/**
 * An implementation of the TimerService
 */
class TimerServiceProvider : zios::common::Noncopyable, public TimerService, public Runnable {
public:
    TimerServiceProvider(TimerQueue* timerQueue);
    virtual ~TimerServiceProvider();

    tqe_id_t schedule(TimerTask* timerTask, int64_t delay, const TimeUnit& timeUnit);
    void cancel(tqe_id_t tqeId);
    void shutdown();
    void start();
    void run();

private:
    Thread* _timerThread;
    TimerQueue* _timerQueue;
    bool _started;
    Mutex _mutex;
};


} /* namespace net */
} /* namespace zios */

#endif /* TIMERQUEUEEX_H_ */
