/** @file ThreadSleeper.h
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

#ifndef THREADSEEPER_H_
#define THREADSEEPER_H_
#include "ConditionVariable.h"

namespace zios {
namespace common {

class ThreadSleeper {
public:
    ThreadSleeper();
    virtual ~ThreadSleeper();

    virtual bool sleep(long milliseconds);
    virtual void interrupt();

private:
    ConditionVariable _conditionVariable;
};

} /* namespace common */
} /* namespace zios */
#endif /* THREADSEEPER_H_ */
