/** @file EventScheduleTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 19, 2015 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <EventSchedule.h>
#include <ctime>
#include <Utl.h>

#include "MockMonotonicTime.h"

namespace zios {
namespace common {

using testing::Mock;
using testing::Return;

using namespace std;
using namespace zios::common;

class EventScheduleTest : public testing::Test {
public:
    ~EventScheduleTest() {}
};

TEST_F(EventScheduleTest, shouldCleanup) {
    MockMonotonicTime* systemTime = new MockMonotonicTime();

    EXPECT_CALL(*systemTime, destroyMockMonotonicTime());
    EXPECT_CALL(*systemTime, getTimeSec()).WillOnce(Return(0));
    {
        EventSchedule schedule(0, systemTime);
    }
    Mock::VerifyAndClearExpectations(systemTime);
}

const char* formatTime(time_t t, char* buf, int sizeOfBuf) {
    struct tm *tm = ::localtime(&t);
    ::strftime(buf, sizeOfBuf, "%X", tm);
    return buf;
}

TEST(WhenEventScheduleIsAskedToReturnDueTimes, shouldReturnSecondsUntilNextDueTime) {
    time_t currentTime = AbsoluteTime::absoluteTimeForString("2015-02-28T12:08:03");
    time_t startTime = AbsoluteTime::absoluteTimeForString("00:00:00");
    int32_t deltaTime = DeltaTime::secondsForDeltaTimeString("01:00:00");

    time_t expectedTime1 = AbsoluteTime::absoluteTimeForString("2015-02-28T13:00:00");
    time_t expectedTime2 = AbsoluteTime::absoluteTimeForString("2015-02-28T14:00:00");
    time_t expectedTime3 = AbsoluteTime::absoluteTimeForString("2015-02-28T15:00:00");

    MockMonotonicTime* systemTime = new MockMonotonicTime();
    systemTime->expectToBeDestroyed();

    EXPECT_CALL(*systemTime, getTimeSec())
        .WillOnce(Return(startTime))
        .WillOnce(Return(currentTime))
        .WillOnce(Return(expectedTime1))
        .WillOnce(Return(expectedTime2));

    EventSchedule eventSchedule(deltaTime, systemTime);
    ASSERT_EQ(expectedTime1, eventSchedule.nextDueTime());
    ASSERT_EQ(expectedTime2, eventSchedule.nextDueTime());
    ASSERT_EQ(expectedTime3, eventSchedule.nextDueTime());
}

TEST_F(EventScheduleTest, shouldReturnNextDueTimeInTheFutureIfStartTimeIsGTThanNow) {
    time_t now = ::time(NULL);
    time_t startTime = now + 3600;
    int32_t deltaTime = 60;
    MockMonotonicTime* systemTime = new MockMonotonicTime();

    EXPECT_CALL(*systemTime, destroyMockMonotonicTime());
    EXPECT_CALL(*systemTime, getTimeSec())
        .WillOnce(Return(startTime))
        .WillRepeatedly(Return(now));

    EventSchedule eventSchedule(deltaTime, systemTime);
    ASSERT_EQ(now + 3600 + 60, eventSchedule.nextDueTime());
}

TEST_F(EventScheduleTest, shouldReturnNextDueTimeInTheFutureIfStartTimeIsEqNow) {
    time_t now = 500;
    time_t startTime = now + 1;
    int32_t deltaTime = 60;
    MockMonotonicTime* systemTime = new MockMonotonicTime();

    EXPECT_CALL(*systemTime, destroyMockMonotonicTime());
    EXPECT_CALL(*systemTime, getTimeSec())
        .WillOnce(Return(startTime))
        .WillRepeatedly(Return(now));

    EventSchedule eventSchedule(deltaTime, systemTime);
    ASSERT_EQ(startTime + deltaTime, eventSchedule.nextDueTime());
}

TEST_F(EventScheduleTest, shouldCalculateNextDueTime) {
    time_t now = 502;
    time_t startTime = 400;
    int32_t interval = 50;
    time_t expectedTime = 550;

    MockMonotonicTime* systemTime = new MockMonotonicTime();
    EXPECT_CALL(*systemTime, destroyMockMonotonicTime());
    EXPECT_CALL(*systemTime, getTimeSec())
        .WillOnce(Return(startTime))
        .WillRepeatedly(Return(now));

    EventSchedule eventSchedule(interval, systemTime);
    ASSERT_EQ(expectedTime, eventSchedule.nextDueTime());
}

TEST(WhenEventScheduleIsDestroyed, shouldCleanUpMonotonicTimeObject) {
    MockMonotonicTime* systemTime = new MockMonotonicTime();

    systemTime->expectToBeDestroyed();
    {
        EventSchedule schedule(1, systemTime);
    }
    Mock::VerifyAndClearExpectations(systemTime);
}

TEST(WhenEventScheduleIsDestroyed, shouldCleanUpButNotDeleteMonotonicTimeIfGlobalTimeUsed) {
    EventSchedule schedule(1);
}

TEST(WhenEventScheduleIsAskedForNextDueTimeWhenStartTimeIsSpecified, shouldReturnNextDueTime) {
    time_t startTime = AbsoluteTime::absoluteTimeForString("2018-02-19T09:58:00");
    time_t currentTime = AbsoluteTime::absoluteTimeForString("2018-02-19T10:00:00");
    int32_t interval = 120; // 2 min

    MockMonotonicTime* systemTime = new MockMonotonicTime();
    systemTime->expectToBeDestroyed();

    EXPECT_CALL(*systemTime, getTimeSec()).WillOnce(Return(currentTime));
    EventSchedule schedule(startTime, interval, systemTime);

    ASSERT_EQ(currentTime+interval, schedule.nextDueTime());
};

}
}
