/** @file MockTimerTask.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 16, 2018 by daver:
 * @endhistory
 */
#ifndef MOCKTIMERTASK_H_
#define MOCKTIMERTASK_H_

#include <timerq.h>

class MockTimerTask : public zios::common::TimerTask {
public:
    ~MockTimerTask() { destroyMockTimerTask(); }
    MOCK_METHOD0(destroyMockTimerTask, void());
    MOCK_METHOD0(run, void());
};

#endif /* MOCKTIMERTASK_H_ */
