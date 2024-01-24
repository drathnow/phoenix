/** @file TimerQueueTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 25, 2015 by daver:
 * @endhistory
 */

#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <stdint.h>
#include <timerq.h>
#include <ctime>

#include "MockMonotonicTime.h"
#include "MockConditionVariable.h"
#include "MockTimerTask.h"

namespace zios {
namespace common {

using testing::Mock;
using testing::Return;
using testing::Sequence;
using testing::InSequence;

using namespace zios::common;

class TestableTimerQueue : public TimerQueue {
public:
    ~TestableTimerQueue() {}

    void processEntriesOrWait() { TimerQueue::processEntriesOrWait(); }
};

class TimerQueueTest : public testing::Test {
public:
    ~TimerQueueTest() {}

    void invokeProcessEntriesOrWaitInTimerQueue(TimerQueue& timerQueue) {
        timerQueue.processEntriesOrWait();
    }

    void setShutdownInTimerQueue(bool isShutdown, TimerQueue& timerQueue) {
        timerQueue._shutdown = isShutdown;
    }

    static void processEntriesOrWait(TimerQueue& timerQueue) {
        timerQueue.processEntriesOrWait();
    }

    static void replaceConditionVariableInTimerQueue(TimerQueue& timerQueue, ConditionVariable* conditionVariable) {
        delete timerQueue._conditionVariable;
        timerQueue._conditionVariable = conditionVariable;
    }

    static void setShutdownInTimerQueue(TimerQueue& timerQueue, bool shutdown) {
        timerQueue._shutdown = shutdown;
    }
};

class WhenTimerQueueMustWait : public TimerQueueTest {
public:
    WhenTimerQueueMustWait() :
        _conditionVariable(new MockConditionVariable()),
        _monotonicTime(new MockMonotonicTime()),
        _timerTask(new MockTimerTask()),
        _now(MonotonicTime::globalSystemTime.getTimeMsec()) {}
    ~WhenTimerQueueMustWait() {}

    MockConditionVariable* _conditionVariable;
    MockMonotonicTime* _monotonicTime;
    MockTimerTask* _timerTask;
    int64_t _now;
};

TEST_F(WhenTimerQueueMustWait, shouldWaitSeconds) {
    EXPECT_CALL(*_conditionVariable, lock()).Times(3);
    EXPECT_CALL(*_monotonicTime, getTimeMsec()).WillRepeatedly(Return(_now));
    EXPECT_CALL(*_conditionVariable, unlock()).Times(3);
    EXPECT_CALL(*_conditionVariable, wait(5000)).WillOnce(Return(ConditionVariable::OK));
    EXPECT_CALL(*_monotonicTime, destroyMockMonotonicTime());
    EXPECT_CALL(*_timerTask, run()).Times(0);
    EXPECT_CALL(*_timerTask, destroyMockTimerTask());
    EXPECT_CALL(*_conditionVariable, destroy());

    TimerQueue timerQueue(_monotonicTime, _conditionVariable);
    timerQueue.insertTQE(new TimerQueueEntry(_now+5000, _timerTask));
    processEntriesOrWait(timerQueue);
}

class WhenTimerQueueMustWaitThenExecuteTask : public TimerQueueTest {
public:
    WhenTimerQueueMustWaitThenExecuteTask() :
        _conditionVariable(new MockConditionVariable()),
        _monotonicTime(new MockMonotonicTime()),
        _timerTask(new MockTimerTask()),
        _now(MonotonicTime::globalSystemTime.getTimeMsec()) {}
    ~WhenTimerQueueMustWaitThenExecuteTask() {}

    MockConditionVariable* _conditionVariable;
    MockMonotonicTime* _monotonicTime;
    MockTimerTask* _timerTask;
    int64_t _now;
};

TEST_F(WhenTimerQueueMustWaitThenExecuteTask, shouldWaitSeconds) {
    EXPECT_CALL(*_conditionVariable, lock()).Times(4);
    EXPECT_CALL(*_monotonicTime, getTimeMsec()).WillRepeatedly(Return(_now));
    EXPECT_CALL(*_conditionVariable, unlock()).Times(4);
    EXPECT_CALL(*_conditionVariable, wait(0)).WillOnce(Return(ConditionVariable::OK));
    EXPECT_CALL(*_monotonicTime, destroyMockMonotonicTime());
    EXPECT_CALL(*_timerTask, run());
    EXPECT_CALL(*_timerTask, destroyMockTimerTask());
    EXPECT_CALL(*_conditionVariable, destroy());

    TimerQueue timerQueue(_monotonicTime, _conditionVariable);
    timerQueue.insertTQE(new TimerQueueEntry(_now, _timerTask));
    processEntriesOrWait(timerQueue);
}

}
}
