/** @file ConditionVariable.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 20, 2013 by daver:
 * @endhistory
 */

#ifndef CONDITIONVARIABLE_H_
#define CONDITIONVARIABLE_H_
#include <pthread.h>
#include "Utl.h"
#include "Lock.h"
#include "Mutex.h"

namespace zios {
namespace common {

class PthreadWrapper;
class MonotonicTime;


/** ConditionVariable
 *
 * A ConditionVariable can be used to synchronize the execution of two or more threads.  If one or more
 * threads need to wait for a condition, they can call the wait() method. Threads will block until another
 * thread invokes the signal() method.
 *
 */
class ConditionVariable : public Lock, private Noncopyable {
public:
    ConditionVariable(PthreadWrapper* pthreadWrapper = NULL, MonotonicTime* monotonicTime = NULL);
    virtual ~ConditionVariable();

    enum Status {
        OK,
        TIMED_OUT
    };

    virtual void lock();
    virtual void unlock();
    virtual bool tryLock();

    /** wait
     *
    *  Waits for this condition variable to be signaled.  Before calling this method, clients MUST
    *  call lock()
    *
    *  @param[in] waitTimeMilliseconds - the number of milliseconds to wait.  If no value, or zero,
    *  is specified, then the calling thread will block until signal is called. If the wait value is
    *  greater than zero, the calling thread will wait the specified number of milliseconds or until
    *  the signal() method is called, whichever happens first.
    *
    *  @param[in] waitTimeMilliseconds - wait time in milliseconds. Default value is zero, which
    *  means wait for ever
    *
    *  @returns Status - OK means the wait was terminated by a call to signal().  TIMED_OUT is
    *  returned if the waitTimeMilliseconds argument is greater than zero and the specified time
    *  elapsed without signal() being called.
    *
    *  @throws Exception if an exception happens trying trying to wait on the underlying condition
    *  variable.
    */
    virtual Status wait(long waitTimeMilliseconds = 0);

    /** lockAndWait
    *  Locks the condition variable a waits for this condition variable to be signaled.
    *
    *  @param[in] waitTimeMilliseconds - the number of milliseconds to wait.  If no value, or zero,
    *  is specified, then the calling thread will block until signal is called. If the wait value is
    *  greater than zero, the calling thread will wait the specified number of milliseconds or until
    *  the signal() method is called, whichever happens first.
    *
    *  @param[in] waitTimeMilliseconds - wait time in milliseconds. Default value is zero
    *
    *  @returns Status - OK means the wait was terminated by a call to signal().  TIMED_OUT is
    *  returned if the waitTimeMilliseconds argument is greater than zero and the specified time
    *  elapsed without signal() being called.
    *
    *  @throws Exception if an exception happens trying lock or unlock the underlying condition
    *  variable.
    */
    virtual Status lockAndWait(long waitTimeMilliseconds = 0);

    /** signal
    *
    *  Signals the condition variable. If there is more than one thread waiting, only a single thread
    *  (magically chosen) will be released. If you want to release all threads, use the broadcast()
    *  method.
    *
    *  @returns int - the current value
    */
    virtual void signal();

    /** broadcast
     *
     * Broadcasts the condition variable.  If there is more than one thread blocked, all threads will be
     * released.
     */
    virtual void broadcast();

private:
    PthreadWrapper* _pthreadWrapper;
    MonotonicTime* _monotonicTime;
    Mutex _mutex;
    pthread_cond_t  _conditionVariable;

    Status waitForEverIfNeedBe();
    Status doTimedWait(long waitTimeMilliseconds);

};

}
} /* namespace zios */
#endif /* CONDITIONVARIABLE_H_ */
