/** @file Thread.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * Class implemenation file for Thread
 *
 * @history
 * Created Jul 31, 2013 by daver:
 * @endhistory
 */

#ifndef THREAD_H_
#define THREAD_H_
#include <log4cplus/logger.h>
#include <map>
#include <deque>
#include <pthread.h>
#include "Utl.h"
#include "Runnable.h"
#include "Queue.h"
#include "Mutex.h"
#include "ConditionVariable.h"

namespace zios {
namespace common {

class Exception;
class Runnable;
class ThreadPool;

using namespace log4cplus;

/**
 *
 * NOTE: By default Theads are created as 'attached' threads, which means they MUST be joined or memory
 * leaks can occur. If you have no intention of joining with a thread, created it as a 'detached'
 * by calling the setDetached() method.
 *
 */
class Thread {
public:

    /** Thread(Runnable& runnable)
     *
     *  Constructs a Thread object with the given Runnable.
     *
     *  @param[in] runnable - Reference to a runnable object. This is not a transfer of ownership
     *
     *  @param[in] name - name attached to thread.
     */
    Thread(Runnable& runnable, const char* name = NULL);

    virtual ~Thread();

    /** start
     *
     *  Starts execution of the thread.  The run method of the Runnable object passed to
     *  the constructor will be invoked.
     *
     *  @returns int - 0 if thread was started.  -1 if the thread was previously started.
     *  errno if thread was not started.
     */
    virtual int start();

    /**
     * Returns the name of the thread
     *
     * @return string& - Const reference to the name of the thread.
     */
    virtual const std::string& name() const { return _name; }

    /**
     * Sends a signal to this thread
     *
     * @param signo
     */
    virtual void signal(int signo) const;

    /**
     * Indicates if this thread is currently running.  By "running" we mean the thread's
     * runnable object has been invoked (via the 'run()' method).
     *
     * @return true - This thread is running. False - if not
     */
    virtual bool isRunning() const;

    /** join
     *
     *  Jions the execution of this thread.  The calling thread will block until the call
     *  returns unless a wait time, in seconds, is specified and it is greater than zero
     *
     *  @param[in] waitTimeSeconds - number of seconds to wait for the specified thread to exit.
     *  Default value is zero, which means wait forever or until the thread exists.
     *
     *  @return Status - 0 if the thread exited before the specified wait time expired. ETIMEDOUT
     *      if the specified wait time expired.
     *
     *  @throws Exception& - thrown if an error occurs while waiting for a thread to join.
     */
    virtual int join(long waitTimeSeconds = 0) const;

    /**
     * Sets this thread as being a detached thread.  Detached threads cannot be joined with
     * using the join() method.
     *
     * @param detached - true, thread will be created as a detached thread.  false - it will not.
     */
    virtual inline void setDetached(bool detached) { _detached = true; }

    /**
     * Returns a pointer to the current thread object.  Callers should not hold the
     * reference and should not delete the object.
     *
     * @return Thread* - pointer to a Thread object. NULL if no current thread exists.
     */
    static Thread* currentThread();

    friend void *threadMain(void *ptr);

private:
    Runnable& _runnable;
    std::string _name;
    bool _running;
    pthread_t _pthread;
    pid_t _threadId;
    bool _detached;
    std::map<int, void*> _localStorage;

    /** Thread(const Thread& thread);
     *
     * Copy contructor is private
     */
    Thread(const Thread& thread);

    /** operator= (const Thread& otherThread)
     *
     * Assignment operator is private
     *
     */
    Thread& operator= (const Thread& otherThread);

    static const Logger _logger;
};

} // namespace common
} // namespace zios

#endif /* THREAD_H_ */
