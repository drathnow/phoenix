/** @file TimerQueueEntryTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 25, 2014 by daver:
 * @endhistory
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ctime>

#include <timerq.h>
#include "commonmocks.h"
#include "MockTimerTask.h"

namespace zios {
namespace common_tests {

using testing::Mock;
using namespace common;

TEST(WhenTimerQueueEntryIsDestroyed, shouldDeleteTimerTaskIfItDidNotRun) {
    time_t now = ::time(NULL);
    MockTimerTask* timerTask = new MockTimerTask();

    EXPECT_CALL(*timerTask, run()).Times(0);
    EXPECT_CALL(*timerTask, destroyMockTimerTask());

    {
        TimerQueueEntry entry(now, timerTask);
    }

    Mock::VerifyAndClearExpectations(timerTask);
}

TEST(WhenTimerQueueEntryIsAskedToExecute, shouldDeleteTimerTaskWhenFinished) {
    time_t now = ::time(NULL);
    MockTimerTask* timerTask = new MockTimerTask();

    EXPECT_CALL(*timerTask, run());
    EXPECT_CALL(*timerTask, destroyMockTimerTask());

    TimerQueueEntry entry(now, timerTask);
    entry.execute();

    Mock::VerifyAndClearExpectations(timerTask);
}

}
}

