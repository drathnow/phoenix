#include <bits/types/time_t.h>
#include <EventSchedule.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockClock.h>
#include <ctime>

namespace zios::foundation
{

using namespace std;
using namespace zios::test;

using testing::Mock;
using testing::Return;

TEST(WhenEventScheduleIsAskedNextDueTime, shouldReturnCurrentTimeIfStartTimeIsCurrent)
{
    MockClock systemClock;
    time_t now = ::time(nullptr);

    EXPECT_CALL(systemClock, currentTime())
        .WillOnce(Return(now))
        .WillOnce(Return(now+21));

    EventSchedule eventScheduleUnderTest(now+20, 10, 0, &systemClock);
    ASSERT_EQ(now+20, eventScheduleUnderTest.nextDueTime());
    ASSERT_EQ(now+30, eventScheduleUnderTest.nextDueTime());
}

TEST(WhenEventScheduleIsAskedNextDueTime, shouldReturnNextTimeIfCurrentTimeIsPassedInitialStartTime)
{
    MockClock systemClock;
    time_t now = ::time(nullptr);

    EXPECT_CALL(systemClock, currentTime())
        .WillOnce(Return(now+21));

    EventSchedule eventScheduleUnderTest(now+20, 10, 0, &systemClock);
    ASSERT_EQ(now+30, eventScheduleUnderTest.nextDueTime());
}

TEST(WhenEventScheduleIsAskedNextDueTime, shouldReturnZeroIfDurationHasExpired)
{
    MockClock systemClock;
    time_t now = ::time(nullptr);

    EXPECT_CALL(systemClock, currentTime())
        .WillOnce(Return(now+31));

    EventSchedule eventScheduleUnderTest(now, 10, 30, &systemClock);
    ASSERT_EQ(0, eventScheduleUnderTest.nextDueTime());
}

TEST(WhenEventScheduleIsAskedNextDueTime, shouldReturnNextTimeFromNowIfStartTimeIsInThePast)
{
    MockClock systemClock;
    time_t now = ::time(nullptr);

    EXPECT_CALL(systemClock, currentTime())
        .WillOnce(Return(now-10));

    EventSchedule eventScheduleUnderTest(now, 10, 0, &systemClock);
    ASSERT_EQ(now, eventScheduleUnderTest.nextDueTime());
}


}
