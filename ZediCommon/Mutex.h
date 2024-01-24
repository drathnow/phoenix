/** @file Mutex.h
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
 * Created Jul 31, 2013 by daver:
 * IncrDev Jun 26, 2014 by eb: added in mutex_timedlock function
 * @endhistory
 */

#ifndef MUTEX_H_
#define MUTEX_H_
#include <pthread.h>

#include "Utl.h"
#include "Lock.h"
#include "Exception.h"

namespace zios {
namespace common {

class Mutex : Noncopyable, public Lock {
public:
    enum Type {
        DEFAULT,
        RECURSIVE
    };

	Mutex(Type = RECURSIVE);
	virtual ~Mutex();

	/**
	 * Aquires a lock. If the lock is currently not locked by another thread, the calling
	 * thread will hang until the lock is released.  If the lock is free, this method returns
	 * immediately.
	 *
	 * @throws Exception& - thrown if an attempt to aquire the lock fails.
	 */
	virtual void lock();

	/**
	 * Trys to aquire a lock. If the lock is not free, this method will return immediately and return
	 * false to the caller.  If the lock is aquired, the method will return and return true to the caller
	 *
	 * @throws Exception& - thrown if an attempt to aquire the lock fails.
	 */
	virtual bool tryLock();

	/**
	 * Trys to aquire a lock. If the lock is not free, this method will return after timeout and return
	 * false to the caller.  If the lock is aquired, the method will return and return true to the caller
	 *
	 * @throws Exception& - thrown if an attempt to aquire the lock fails.
	 */
	virtual bool tryLockTime(timespec& ts);
	/**
	 * Releases the lock.
	 *
	 * @throws Exception& - thrown if an attempt to aquire the lock fails.
	 */
	virtual void unlock();

	friend class ConditionVariable;

private:
	pthread_mutex_t _mutex;
	pthread_mutexattr_t _mutexAttributes;
};

} /* namespace common */
} /* namespace zios */
#endif /* MUTEX_H_ */
