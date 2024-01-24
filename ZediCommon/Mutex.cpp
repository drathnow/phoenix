/* @file Mutex.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * Class implemenation file for Mutex.  The Mutex class provide a thread syncronization mechanism
 * that multiple threads can use for synchronizing access to share resources.
 *
 * @history
 * Created Jul 30, 2013 by daver:
 * IncrDev Jun 26, 2014 by eb: added in mutex_timedlock function
 * @endhistory
 */

#include <cstring>
#include <cerrno>
#include <iostream>
#include "Mutex.h"
#include <cstdio>

namespace zios {
namespace common {

Mutex::Mutex(Type type) {
    int errorNumber;
    if ((errorNumber = ::pthread_mutexattr_init(&_mutexAttributes)) != 0)
        THROW_EXCEPTION("Unable to init mutex attr. Error: "<< ::strerror(errorNumber) << "(errorNumber=" << errorNumber<< ")");

    if (RECURSIVE == type) {
        if ((errorNumber = ::pthread_mutexattr_settype(&_mutexAttributes, PTHREAD_MUTEX_RECURSIVE)) != 0)
            THROW_EXCEPTION("Unable to set mutex attr. Error: "<< ::strerror(errorNumber) << "(errorNumber=" << errorNumber<< ")");
    }

    if ((errorNumber = ::pthread_mutex_init(&_mutex, &_mutexAttributes)) != 0)
        THROW_EXCEPTION("Unable to init mutex. Error: "<< ::strerror(errorNumber) << "(errorNumber=" << errorNumber<< ")");
}

Mutex::~Mutex() {
    ::pthread_mutexattr_destroy(&_mutexAttributes);
    ::pthread_mutex_destroy(&_mutex);
}

void Mutex::lock() {
    int errorNumber = ::pthread_mutex_lock(&_mutex);
    if (errorNumber != 0)
        THROW_EXCEPTION("Unable to lock mutex. Error: " << ::strerror(errorNumber) << "(errorNumber=" << errorNumber << ")");
}

bool Mutex::tryLock() {
    int errorNumber = ::pthread_mutex_trylock(&_mutex);
    if (errorNumber != 0 && errorNumber != EBUSY)
        THROW_EXCEPTION("Unable to trylock mutex. Error: " << ::strerror(errorNumber) << "(errorNumber=" << errorNumber << ")");
    return errorNumber == 0;
}

bool Mutex::tryLockTime(timespec& ts) {
    int errorNumber = ::pthread_mutex_timedlock(&_mutex, &ts);
    if (errorNumber != 0 && errorNumber != ETIMEDOUT)
        THROW_EXCEPTION("Unable to trylockTime mutex. Error: " << ::strerror(errorNumber) << "(errorNumber=" << errorNumber << ")");
    return errorNumber == 0;
}

void Mutex::unlock() {
    int errorNumber = ::pthread_mutex_unlock(&_mutex);
    if (errorNumber != 0)
        THROW_EXCEPTION("Unable to unlock mutex. Error: " << ::strerror(errorNumber) << "(errorNumber=" << errorNumber << ")");
}

} /* namespace common */
} /* namespace zios */
