/** @file ReadWriteLock.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 23, 2014 by daver:
 * @endhistory
 */
#include <cstring>

#include "Utl.h"
#include "ReadWriteLock.h"
#include "Exception.h"

namespace zios {
namespace common {

using namespace std;

ReadWriteLock::ReadWriteLock() {
    if (::pthread_rwlock_init(&_rwLock, NULL) != 0)
        Utl::throwExceptionWithMessage(errno, "Unable to init ReadWriteLog");
}

ReadWriteLock::~ReadWriteLock() {
    ::pthread_rwlock_destroy(&_rwLock);
}

void ReadWriteLock::lockForRead() {
    if (::pthread_rwlock_rdlock(&_rwLock) != 0)
        Utl::throwExceptionWithMessage(errno, "ReadWriteLock::lockForRead failed");
}

void ReadWriteLock::unlock() {
    if (::pthread_rwlock_unlock(&_rwLock) != 0)
        Utl::throwExceptionWithMessage(errno, "ReadWriteLock::unlock failed");
}

void ReadWriteLock::lockForWrite() {
    if (::pthread_rwlock_wrlock(&_rwLock) != 0)
        Utl::throwExceptionWithMessage(errno, "ReadWriteLock::lockForWrite failed");
}

} /* namespace common */
} /* namespace zios */
