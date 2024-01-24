/** @file ThreadPool.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 17, 2017 by daver:
 * @endhistory
 */
#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Thread.h"
#include "Runnable.h"
#include "Mutex.h"
#include "Queue.h"
#include "MonotonicTime.h"

#include <ctime>

namespace zios {
namespace common {

class ThreadPoolRunner;


typedef struct runner_block_t {
    runner_block_t(Runnable* rb, bool di) :
        runnable(rb),
        deleteIt(di) {
    }
    Runnable* runnable;
    bool deleteIt;
} runner_block_t;

typedef std::map<ThreadPoolRunner*, Thread*>    runner_map_t;
typedef std::pair<ThreadPoolRunner*, Thread*>   runner_pair_t;
typedef BlockingQueue<runner_block_t*>          runner_queue_t;


class ThreadPoolRunner : public Runnable {
public:
    ThreadPoolRunner(BlockingQueue<runner_block_t*>& runnableQueue, int number, ThreadPool* threadPool, MonotonicTime* systemTime = NULL);
    virtual ~ThreadPoolRunner();

    virtual void run();
    virtual bool isActive() const;
    virtual void shutdown();

    bool operator==(const ThreadPoolRunner& otherRunner) const {
        return this == &otherRunner;
    }

    friend class ThreadPool;

private:
    runner_queue_t& _runnableQueue;
    ThreadPool* _threadPool;
    bool _shutdown;
    int _number;
    time_t _activeStartTime;
    MonotonicTime* _systemTime;
    static const Logger _logger;
};

class ThreadPoolRunnerFactory {
public:
    ThreadPoolRunnerFactory();
    virtual ~ThreadPoolRunnerFactory();

    virtual ThreadPoolRunner* newThreadPoolRunner(runner_queue_t& runnableQueue,
                                                  int number,
                                                  ThreadPool* threadPool) const;
};

class ThreadFactory {
public:
    ThreadFactory();
    virtual ~ThreadFactory();

    virtual Thread* newThread(Runnable& runnable, const char* name = NULL) const;
};

class ThreadPool : zios::common::Noncopyable {
public:
    /** Constructor ThreadPool
     *
     * Constructs a thread pool.
     *
     * @param initialSize - the initial number of threads in this pool
     *
     * @param maxSize - the maximum number of threads the pool can grow to.  If maxSize is zero, or not specified,
     * the maxSize is equal to the initialSize;
     *
     * @param runnerFactory - optional pointer to a factory to create the ThreadPoolRunners.
     * If no factory is provided a default factory will be used.  If a factory is passed it, it must be created
     * on the heap and will be deleted by this object when it is destroyed.
     *
     * @param threadFctory - optional pointer to a factory to create Thread objects.
     * If no factory is provided a default factory will be used.  If a factory is passed it, it must be created
     * on the heap and will be deleted by this object when it is destroyed.
     *
     */
    ThreadPool(uint32_t initialSize, uint32_t maxSize = 0, ThreadPoolRunnerFactory* runnerFactory = NULL, ThreadFactory* threadFctory = NULL);
    virtual ~ThreadPool();

    /** excutes
     *
     * Queues a Runnable object for execution.
     *
     * @param[in] task - pointer to a runnable object.  Ownership of the object is retained by the caller.
     * The runnable object will not be delete when execution completes.
     *
     * @param[in] deleteIt - true if we are to delete the runnable object associated with the thread
     * @return bool - true if the runnable was accepted.  false if the thread pool has been shutdown and
     * will not accept any new tasks.
     */
    virtual bool execute(Runnable* task, bool deleteIt = false);

    /**
     * Returnes the number of executing threads in the pool. A thread is executing if it is executing a
     * runnable task.
     *
     * @return int - count of active threads.
     */
    virtual int executingCount();

    /**
     * Returns the number of active threads in the pool.  A thread is active if it is either executing
     * or waiting for work.
     *
     * @return int - number of active threads.
     */
    virtual int activeCount();

    /**
     * Returns the number of threads in the pool.
     *
     * @return int - number of threads in the pool.
     */
    virtual int poolSize() const;

    /** Shutdown
     *
     * Initiates an orderly shutdown in which previously submitted tasks are executed, but no new tasks will be accepted
     *
     * @param waitSeconds - Number of seconds to wait for all threads to exit. Zero or no value will result in no waiting.
     * NOTE: If all threads do not exit within the specified wait time, and this ThreadPool object is deleted,
     * the results to your program are indeterminent. (e.g. seg faults, memory leaks).  If you choose to shutdown
     * with zero wait seconds, callers should use the activeCount() method to determine if any of the threads in
     * the pool are still active before deleting this object, or call awaitTermination to wait for all threads
     * to terminate.
     *
     * @return  true if all threads existed.false if at least one thread did not exit
     * within the alloted time period.
     */
    virtual bool shutdown(uint32_t waitSeconds = 0);

    /**
     * Waits for all threads in the thread pool to terminate after shutdown() has been called.
     *
     * @param maxWaitSeconds - maximum number of seconds to wait for all threads to terminate.
     *
     * @return boolean - true if all threads terminate. false if at least one thread did not terminate;
     *
     */
    bool awaitTermination(uint32_t maxWaitSeconds);

    friend class ThreadPoolRunner;

private:
    bool _shutdown;
    ThreadPoolRunnerFactory* _runnerFactory;
    ThreadFactory* _threadFactory;
    int _executingCount;
    int _poolSize;
    int _maxPoolSize;
    uint32_t _threadIndex;
    Mutex _lock;
    runner_queue_t _runnableQueue;
    runner_map_t _runnerMap;
    runner_map_t _cleanUpMap;

    void beforeExecuting(ThreadPoolRunner* threadPoolRunner);
    void afterExecuting(ThreadPoolRunner* threadPoolRunner);
    void poolThreadTerminating(ThreadPoolRunner* runner);
    void addThreadToPool();

    static const Logger _logger;
};

} /* namespace commonEx */
} /* namespace zios */

#endif /* THREADPOOL_H_ */
