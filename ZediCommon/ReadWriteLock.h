/** @file ReadWriteLock.h
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
#ifndef READWRITELOCK_H_
#define READWRITELOCK_H_

#include <pthread.h>
#include <stdio.h>

namespace zios {
namespace common {

class ReadWriteLock {
public:
    /**
     * Constructs a ReadWriteLog
     *
     * @throws Exception if the read write lock could not be initialized
     */
    ReadWriteLock();
    virtual ~ReadWriteLock();

    /** lockForRead
     *
     * Acquires a concurrent read lock on this lock.  The lock will not be granted if another
     * thread holds a write lock on this ReadWriteLock.  That is, you can have multiple readers
     * but only one writer
     *
     * @throws Exception - if something broke while trying to acquire the lock
     */
    virtual void lockForRead();

    /** lockForWrite
     *
     * Acquires a exclusive write lock on this lock.  The calling thread will block if
     * any concurrent read locks are active or if another thread has already aquired the
     * write lock.
     *
     * @throws Exception - if something broke while trying to acquire the lock
     */
    virtual void lockForWrite();

    /** unlock
     *
     * You figure it out.  You can only call this method after lockForRead has been called.
     *
     * @throws Exception - if something broke while trying to acquire the lock
     */
    virtual void unlock();

private:
    pthread_rwlock_t _rwLock;
};

/**
 * Handy macro to ensure rw locks are locked and unlocked within a specific scope.
 */
template<class RWLockable>
class ScopedReadLock {
public:
    ScopedReadLock(RWLockable& lock) :
            _lock(lock) {
        _lock.lockForRead();
    }

    ~ScopedReadLock() {
        _lock.unlock();
    }
private:
    RWLockable& _lock;
};

template<class RWLockable>
class ScopedWriteLock {
public:
    ScopedWriteLock(RWLockable& lock) :
            _lock(lock) {
        _lock.lockForWrite();
    }

    ~ScopedWriteLock() {
        _lock.unlock();
    }
private:
    RWLockable& _lock;
};

} /* namespace common */
} /* namespace zios */

#endif /* READWRITELOCK_H_ */

