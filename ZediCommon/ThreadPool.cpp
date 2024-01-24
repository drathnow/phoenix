/** @file ThreadPool.cpp
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
#include "ThreadPool.h"
#include <unistd.h>
#include <log4cplus/loggingmacros.h>

namespace zios {
namespace common {

using namespace log4cplus;

const Logger ThreadPool::_logger = Logger::getInstance("zios.common.ThreadPool");
const Logger ThreadPoolRunner::_logger = Logger::getInstance("zios.common.ThreadPoolRunner");

static void cleanUpMap(runner_map_t& runnerMap) {
    for (runner_map_t::iterator iter = runnerMap.begin(); iter != runnerMap.end(); ++iter) {
        delete iter->first;
        delete iter->second;
    }
    runnerMap.clear();
}

static void cleanUpMapSync(runner_map_t& runnerMap, Mutex& lock) {
    lock.lock();
    cleanUpMap(runnerMap);
    lock.unlock();
}

ThreadFactory::ThreadFactory() {
}

ThreadFactory::~ThreadFactory() {
}

Thread* ThreadFactory::newThread(Runnable& runnable, const char* name) const {
    return new Thread(runnable, name);
}

ThreadPoolRunner::ThreadPoolRunner(runner_queue_t& runnableQueue, int number, ThreadPool* threadPool, MonotonicTime* systemTime) :
        _runnableQueue(runnableQueue),
        _threadPool(threadPool),
        _shutdown(false),
        _number(number),
        _activeStartTime(0),
        _systemTime(systemTime == NULL ? &MonotonicTime::globalSystemTime : systemTime){
}

ThreadPoolRunner::~ThreadPoolRunner() {
    if (_systemTime != &MonotonicTime::globalSystemTime)
        delete _systemTime;
}

bool ThreadPoolRunner::isActive() const {
    return _activeStartTime != 0;
}

/**
 * This is the main entry point for threads that make up a ThreadPool.
 */
void ThreadPoolRunner::run() {
    LOG4CPLUS_INFO(_logger, "Pool thread runner starting " << _number);

    while (_shutdown == false) {
        // Assume we aren't doing anything.
        _activeStartTime = 0;
        // Block here until we have something to do.
        runner_block_t* runBlock = _runnableQueue.removeFront(0);
        if (runBlock != NULL) {
            _activeStartTime = _systemTime->getTimeSec();
            _threadPool->beforeExecuting(this);
            try {
                runBlock->runnable->run();
            } catch (...) {
                LOG4CPLUS_ERROR(_logger, "Unexpected exception caught from Runnable");
            }
            _threadPool->afterExecuting(this);
            if (runBlock->deleteIt)
                delete runBlock->runnable;
            delete runBlock;
        }
    }
    _threadPool->poolThreadTerminating(this);
    LOG4CPLUS_DEBUG(_logger, "Pool thread exiting " << _number);
}

void ThreadPoolRunner::shutdown() {
    _shutdown = true;
}

ThreadPoolRunnerFactory::ThreadPoolRunnerFactory() {
}

ThreadPoolRunnerFactory::~ThreadPoolRunnerFactory() {
}

ThreadPoolRunner* ThreadPoolRunnerFactory::newThreadPoolRunner(runner_queue_t& runnableQueue, int number, ThreadPool* threadPool) const {
    return new ThreadPoolRunner(runnableQueue, number, threadPool);
}

void ThreadPool::addThreadToPool() {
    if (_runnerMap.size() < (unsigned)_maxPoolSize) {
        ThreadPoolRunner* runner = _runnerFactory->newThreadPoolRunner(_runnableQueue, _threadIndex++, this);
        Thread* thread = _threadFactory->newThread(*runner, "ThreadPoolWorker");
        _runnerMap.insert(runner_pair_t(runner, thread));
        LOG4CPLUS_DEBUG(_logger, "ThreadPool starting thread " << (_threadIndex-1));
        thread->setDetached(true);
        thread->start();
    } else
        LOG4CPLUS_WARN(_logger, "Request to add thread rejected because pool size has reached max (" << _maxPoolSize << ")");
}


ThreadPool::ThreadPool(uint32_t initialSize, uint32_t maxSize, ThreadPoolRunnerFactory* runnerFactory, ThreadFactory* threadFactory)  :
        _shutdown(false),
        _runnerFactory(runnerFactory == NULL ? new ThreadPoolRunnerFactory() : runnerFactory),
        _threadFactory(threadFactory == NULL ? new ThreadFactory() : threadFactory),
        _executingCount(0),
        _poolSize(initialSize),
        _maxPoolSize(maxSize == 0 ? _poolSize : maxSize),
        _threadIndex(1) {
    assert(_maxPoolSize >= _poolSize);

    for (int i = _poolSize; i > 0; i--)
        addThreadToPool();
}

ThreadPool::~ThreadPool() {
    cleanUpMapSync(_cleanUpMap, _lock);
    cleanUpMap(_runnerMap);
    delete _runnerFactory;
    delete _threadFactory;
}

bool ThreadPool::execute(Runnable* runnable, bool deleteIt) {
    if (_shutdown)
        return false;

    //
    // A little house keeping first
    //
    _lock.lock();
    if (_executingCount >= _poolSize && _executingCount < _maxPoolSize)
        addThreadToPool();
    cleanUpMap(_cleanUpMap);
    _lock.unlock();


    //
    // Now queue the work
    //
    runner_block_t* runnerBlock = new runner_block_t(runnable, deleteIt);
    _runnableQueue.addBack(runnerBlock);

    return true;
}

int ThreadPool::executingCount()  {
    _lock.lock();
    int tmp = _executingCount;
    _lock.unlock();
    return tmp;
}

int ThreadPool::poolSize() const {
    return _poolSize;
}

void ThreadPool::poolThreadTerminating(ThreadPoolRunner* runner) {
    _lock.lock();
    runner_map_t::iterator iter = _runnerMap.find(runner);
    if (iter != _runnerMap.end()) {
        _cleanUpMap.insert(runner_pair_t(iter->first, iter->second));
        _runnerMap.erase(iter);
    }
    _lock.unlock();
}

void ThreadPool::beforeExecuting(ThreadPoolRunner* threadPoolRunner) {
    _lock.lock();
    _executingCount++;
    _lock.unlock();
}

void ThreadPool::afterExecuting(ThreadPoolRunner* threadPoolRunner) {
    _lock.lock();
    _executingCount--;
    _lock.unlock();
}

int ThreadPool::activeCount(){
    _lock.lock();
    int tmp = _runnerMap.size();
    _lock.unlock();
    return tmp;
}

bool ThreadPool::awaitTermination(uint32_t maxWaitSeconds) {
    time_t now = ::time(NULL);
    _lock.lock();
    while (_runnerMap.size() > 0U && ::time(NULL) - now < (time_t)maxWaitSeconds){
        _lock.unlock();
        ::usleep(100000);
        _lock.lock();
    }
    bool tempRet = _runnerMap.size() == 0;
    _lock.unlock();
    return tempRet;
}

bool ThreadPool::shutdown(uint32_t waitSeconds) {
    if (_shutdown)
        return true;
    _lock.lock();
    _shutdown = true;
    for (runner_map_t::iterator iter = _runnerMap.begin(); iter != _runnerMap.end(); ++iter)
        iter->first->shutdown();
    _lock.unlock();
    _runnableQueue.wake();

    return awaitTermination(waitSeconds);
}
} /* namespace commonEx */
} /* namespace zios */
