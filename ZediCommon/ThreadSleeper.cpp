/** @file ThreadSleeper.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 27, 2013 by daver:
 * @endhistory
 */

#include "ThreadSleeper.h"

namespace zios {
namespace common {

ThreadSleeper::ThreadSleeper() {
}

ThreadSleeper::~ThreadSleeper() {
}

bool ThreadSleeper::sleep(long milliseconds) {
    return _conditionVariable.lockAndWait(milliseconds) == ConditionVariable::OK;
}

void ThreadSleeper::interrupt() {
    _conditionVariable.signal();
}

} /* namespace common */
} /* namespace zios */
