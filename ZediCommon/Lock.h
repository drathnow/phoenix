/** @file Lock.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 31, 2013 by daver:
 * @endhistory
 */

#ifndef LOCK_H_
#define LOCK_H_
#include "Exception.h"

namespace zios {
namespace common {

/** Lock
 *
 * Defines a general purpose locking API for object that synchronize access to various resources.
 */
class Lock {
public:
	Lock() {}
	virtual ~Lock() {}

	/**
	 * Aquires a lock. If the lock is currently not locked by another thread, the calling
	 * thread will hang until the lock is released.  If the lock is free, this method returns
	 * immediately.
	 */
	virtual void lock() = 0;

	/**
	 * Trys to aquire a lock. If the lock is not free, this method will return immediately and return
	 * false to the caller.  If the lock is aquired, the method will return and return true to the caller
	 *
	 * @returns true - the lock was aquired, false - it was not
	 */
	virtual bool tryLock() = 0;

	/**
	 * Releases the lock.
	 */
	virtual void unlock() = 0;
};

} /* namespace common */
} /* namespace zios */

#endif /* LOCK_H_ */
