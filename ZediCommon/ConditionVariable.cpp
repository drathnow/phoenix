/** @file ConditionVariable.cpp
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
 * BugFix_ Feb 17, 2016 by eb:   remove duplicate pthread mutex destroy (_mutex destructor takes care of it)
 * @endhistory
 */

#include <sys/time.h>
#include <cassert>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Utl.h"
#include "MonotonicTime.h"
#include "templates.h"
#include "ConditionVariable.h"

namespace zios {
namespace common {

using namespace std;

static void throwExecptionWithError(int errorNumber, const char* message) {
    if (errorNumber != 0) {
        char messageBuffer[100];
        ::snprintf(messageBuffer, sizeof(messageBuffer), "%s Error: %s(errorNumber=%d)", message, ::strerror(errorNumber), errorNumber);
        throw std::runtime_error(messageBuffer);
    }
}

ConditionVariable::Status ConditionVariable::doTimedWait(long waitTimeMilliseconds) {
    int errorNumber;
    struct timespec timeToWait;

    _monotonicTime->getTime(&timeToWait);
    Utl::timespecAddTime(&timeToWait, waitTimeMilliseconds);

    errorNumber = _pthreadWrapper->pthread_cond_timedwait(&_conditionVariable, &_mutex._mutex, &timeToWait);
    if (errorNumber != 0 && errorNumber != ETIMEDOUT) {
        std::ostringstream oss;
        oss << "timedWait on condition variable. WaitTime = {tv_sec="
            << timeToWait.tv_sec
            << ", tv_nsec="
            << timeToWait.tv_nsec
            << "} Error = "
            << errorNumber;
        throwExecptionWithError(errorNumber, oss.str().c_str());
    }
    return errorNumber == ETIMEDOUT ? ConditionVariable::TIMED_OUT : ConditionVariable::OK;
}

ConditionVariable::Status ConditionVariable::waitForEverIfNeedBe() {
    int errorNumber = _pthreadWrapper->pthread_cond_wait(&_conditionVariable, &_mutex._mutex);
    if (errorNumber != 0)
        throwExecptionWithError(errorNumber, "Unable to lockAndWait on condition variable.");
    return ConditionVariable::OK;
}

ConditionVariable::ConditionVariable(PthreadWrapper* pthreadWrapper, MonotonicTime* monotonicTime) :
        _pthreadWrapper(pthreadWrapper == NULL ? new PthreadWrapper() : pthreadWrapper),
        _monotonicTime(monotonicTime == NULL ? new MonotonicTime() : monotonicTime),
        _mutex(Mutex::RECURSIVE) {
    // change default clock mode - note this affects the clock source for all future calls to
    // pthread_cond_timedwait using condition variables initialized this way
    // attr structure only needs to be initialized when the condition variable is initialized and can
    // be destroyed later without affecting it.
    pthread_condattr_t attr;
    _pthreadWrapper->pthread_condattr_init(&attr);
    _pthreadWrapper->pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);

    int errorNumber = _pthreadWrapper->pthread_cond_init(&_conditionVariable, &attr);

    _pthreadWrapper->pthread_condattr_destroy(&attr);

    if (errorNumber != 0)
        THROW_EXCEPTION("Unable to create condition variable. Error: " << ::strerror(errorNumber) << " (errorNumber=" << errorNumber << ")");
}

ConditionVariable::~ConditionVariable() {
    _pthreadWrapper->pthread_cond_destroy(&_conditionVariable);
    delete _pthreadWrapper;
    delete _monotonicTime;
}

void ConditionVariable::lock() {
    _mutex.lock();
}

void ConditionVariable::unlock() {
    _mutex.unlock();
}

bool ConditionVariable::tryLock() {
    return _mutex.tryLock();
}

ConditionVariable::Status ConditionVariable::wait(long waitTimeMilliseconds) {
    assert(waitTimeMilliseconds >= 0);
    if (waitTimeMilliseconds > 0)
        return doTimedWait(waitTimeMilliseconds);
    else
        return waitForEverIfNeedBe();
}

ConditionVariable::Status ConditionVariable::lockAndWait(long waitTimeMilliseconds) {
    ScopedLock<Mutex> lock(_mutex);
    if (waitTimeMilliseconds > 0)
        return doTimedWait(waitTimeMilliseconds);
    return waitForEverIfNeedBe();
}

void ConditionVariable::signal() {
    int errorNumber = _pthreadWrapper->pthread_cond_signal(&_conditionVariable);
    if (errorNumber != 0)
        throwExecptionWithError(errorNumber, "Unable to signal condition variable.");
}

void ConditionVariable::broadcast() {
    int errorNumber = _pthreadWrapper->pthread_cond_broadcast(&_conditionVariable);
    if (errorNumber != 0)
        throwExecptionWithError(errorNumber, "Unable to signal condition variable.");
}

}
} /* namespace zios */
