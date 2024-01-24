/** @file mutexbase.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Dec 12, 2013 by daver:
 * @endhistory
 */

#ifndef MUTEXBASE_H_
#define MUTEXBASE_H_

#include "Mutex.h"

namespace zios {
namespace common {

enum lock_policy {
    _no_lock_, _mutex_lock_
};

static const lock_policy _default_lock_policy = _no_lock_;

template<lock_policy lp>
class MutexBase {
protected:
    enum {
        needs_lock = 0
    };
};

template<>
class MutexBase<_mutex_lock_> {
protected:
    // This policy is used when atomic builtins are not available.
    // The replacement atomic operations might not have the necessary
    // memory barriers.
    enum {
        needs_lock = 1
    };

    void lock() { _mutex.lock(); }
    void unlock()  { _mutex.unlock(); }
    bool tryLock() {return _mutex.tryLock();}


private:
    Mutex _mutex;
};

template<>
class MutexBase<_no_lock_> : public Lock {
protected:
    // This policy is used when atomic builtins are not available.
    // The replacement atomic operations might not have the necessary
    // memory barriers.
    enum {
        needs_lock = 1
    };

    void lock() {}
    void unlock() {}
    bool tryLock() {return true;}
};



}
}

#endif /* MUTEXBASE_H_ */
