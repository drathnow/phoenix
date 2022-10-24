#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ctime>
#include <AlarmRange.h>
#include <MockClock.h>
#include <testutil.h>
#include <functional>
#include <AlarmConfiguration.h>

#include "MockAlarmRange.h"

namespace zios::domain
{

using namespace zios::test;

using testing::Return;
using testing::Eq;
using testing::ByRef;
using testing::InSequence;
using testing::Mock;
using testing::_;

static bool operator==(const IAlarmRange<int32_t> &lhs, const AlarmRange<int32_t> &rhs)
{
    return &lhs == &rhs;
}

class AlarmRangeTest: public testing::Test
{
public:
    static const time_t START_TIME;
    static const uint16_t SET_TIME;
    static const uint16_t CLEAR_TIME;

    AlarmRangeTest()
    {
    }
    ~AlarmRangeTest()
    {
    }

    char alarmStateFromAlarmRange(AlarmRange<int32_t> &alarmRange)
    {
        return alarmRange._alarmState & ALARM_STATE_MASK;
    }

    char alarmStateStructFromAlarmRange(AlarmRange<int32_t> &alarmRange)
    {
        alarm_state_t alarmState;
        alarmState.state = alarmRange._alarmState;
        return alarmState.alarmState();
    }

    int32_t _setLimit
    { 0 };
    int32_t _clearLimit
    { 0 };
    int32_t _theValue
    { 0 };
    MockClock systemTime;
    MockAlarmRange<int32_t> *higherRange
    { new MockAlarmRange<int32_t>() };
};

const time_t AlarmRangeTest::START_TIME = ::time(NULL);
const uint16_t AlarmRangeTest::SET_TIME = 10;
const uint16_t AlarmRangeTest::CLEAR_TIME = 15;

class WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsNotInAlarm: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsNotInAlarm()
    {
    }
    ~WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsNotInAlarm()
    {
    }

    void SetUp()
    {
        _theValue = 10;
        _setLimit = 11;
    }
};

TEST_F(WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsNotInAlarm, shouldNotPassValueToHigherRangeAndReturnOK)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(0, &compare_less_equal);

    InSequence seq;
    EXPECT_CALL(*higherRange, alarmStatusForValue(_theValue, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).Times(0);

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_FALSE(alarmRange.isActive());
}
class WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsInAlarm: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsInAlarm()
    {
    }
    ~WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsInAlarm()
    {
    }

    void SetUp()
    {
        _theValue = 10;
        _setLimit = 8;
    }
};

TEST_F(WhenAlarmRangeIsInClearStateWithNoSetTimeAndValueIsInAlarm, shouldPassValueToHigherRangeAndReturnCurrentAlarmStatus)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(0, &compare_less_equal);

    InSequence seq;
    EXPECT_CALL(*higherRange, alarmStatusForValue(_theValue, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

class WhenAlarmRangeIsInClearStateWithSetTimeAndValueIsInAlarm: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInClearStateWithSetTimeAndValueIsInAlarm()
    {
    }
    ~WhenAlarmRangeIsInClearStateWithSetTimeAndValueIsInAlarm()
    {
    }

    void SetUp()
    {
        _theValue = 10;
        _setLimit = 8;
    }
};

TEST_F(WhenAlarmRangeIsInClearStateWithSetTimeAndValueIsInAlarm, shouldPassValueToHigherRangeAndReturnHighestAlarmStatus)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(_clearLimit, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, 0)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME)).WillOnce(Return(START_TIME + SET_TIME));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(10, systemTime, SET_TIME, 0));
    ASSERT_TRUE(alarmRange.isActive());
    ASSERT_EQ(SET_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, SET_TIME, 0));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
}

TEST_F(WhenAlarmRangeIsInClearStateWithSetTimeAndValueIsInAlarm, shouldReturnHigherAlarmStatusIfHigherRangeIsInAlarm)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(0, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, 0)).WillOnce(Return(ALARM_STATUS_OK)).WillOnce(Return(ALARM_STATUS_HIGH_HIGH));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME)).WillOnce(Return(START_TIME + SET_TIME));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(10, systemTime, SET_TIME, 0));
    ASSERT_EQ(SET_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
    ASSERT_EQ(ALARM_STATUS_HIGH_HIGH, alarmRange.alarmStatusForValue(10, systemTime, SET_TIME, 0));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

class WhenAlarmRangeIsInSetPendingStateAndValueClears: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInSetPendingStateAndValueClears()
    {
    }
    ~WhenAlarmRangeIsInSetPendingStateAndValueClears()
    {
    }
};

TEST_F(WhenAlarmRangeIsInSetPendingStateAndValueClears, shouldNotPassValueToHigherRangeClearHigherRangeAndReturnToClearState)
{
    Threshold<int32_t> setLimitThreshold(0, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(0, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), SET_TIME, 0)).WillRepeatedly(Return(ALARM_STATUS_OK));
    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME));
    EXPECT_CALL(*higherRange, clear());

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(_setLimit + 1, systemTime, SET_TIME, 0));
    ASSERT_EQ(SET_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(_setLimit - 1, systemTime, SET_TIME, 0));
    ASSERT_EQ(CLEAR_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_FALSE(alarmRange.isActive());
}

class WhenAlarmRangeIsInAlarmStateWithNoClearTimeAndValueReturnsToOK: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInAlarmStateWithNoClearTimeAndValueReturnsToOK()
    {
    }
    ~WhenAlarmRangeIsInAlarmStateWithNoClearTimeAndValueReturnsToOK()
    {
        delete higherRange;
    }
};

TEST_F(WhenAlarmRangeIsInAlarmStateWithNoClearTimeAndValueReturnsToOK, shouldPassValueToNextRangeEnterClearStateAndReturnOkAlarmStatusIfHigherRangeReturnsOk)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    MockAlarmRange<int32_t> *higherRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, 0)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(*higherRange, clear());

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, 0));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(5, systemTime, 0, 0));
    ASSERT_EQ(CLEAR_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_FALSE(alarmRange.isActive());
}

class WhenAlarmRangeIsInAlarmStateWithClearTimeAndValueReturnsToOK: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInAlarmStateWithClearTimeAndValueReturnsToOK()
    {
    }
    ~WhenAlarmRangeIsInAlarmStateWithClearTimeAndValueReturnsToOK()
    {
    }
};

TEST_F(WhenAlarmRangeIsInAlarmStateWithClearTimeAndValueReturnsToOK, shouldPassValueToNextRangeEnterClearPendingStateAndReturnAlarmStatus)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

TEST_F(WhenAlarmRangeIsInAlarmStateWithClearTimeAndValueReturnsToOK, shouldPassValueToNextRangeEnterClearStateAndReturnHigherAlarmStatusIfHigherRangeReturnsHigherStatus)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME)).WillOnce(Return(START_TIME + CLEAR_TIME));
    EXPECT_CALL(*higherRange, clear());

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_OK, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_FALSE(alarmRange.isActive());
}

class WhenAlarmRangeIsInClearPendingStateAndValueExceedsSetLimit: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInClearPendingStateAndValueExceedsSetLimit()
    {
    }
    ~WhenAlarmRangeIsInClearPendingStateAndValueExceedsSetLimit()
    {
    }
};

TEST_F(WhenAlarmRangeIsInClearPendingStateAndValueExceedsSetLimit, shouldPassValueToHigherRangeAndReturnCurrentAlarmStatusIfHigherRangeOK)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

TEST_F(WhenAlarmRangeIsInClearPendingStateAndValueExceedsSetLimit, shouldPassValueToHigherRangeAndReturnCurrentAlarmStatusIfHigherRangeStatusIfNotOk)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));
    EXPECT_CALL(*higherRange, alarmStatusForValue(6, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));

    EXPECT_CALL(*higherRange, alarmStatusForValue(12, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_HIGH_HIGH));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH_HIGH, alarmRange.alarmStatusForValue(12, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

class WhenAlarmRangeIsInClearPendingStateAndValueExceedsClearLimitButClearTimeHasNotExpired: public AlarmRangeTest
{
public:
    WhenAlarmRangeIsInClearPendingStateAndValueExceedsClearLimitButClearTimeHasNotExpired()
    {
    }
    ~WhenAlarmRangeIsInClearPendingStateAndValueExceedsClearLimitButClearTimeHasNotExpired()
    {
    }
};

TEST_F(WhenAlarmRangeIsInClearPendingStateAndValueExceedsClearLimitButClearTimeHasNotExpired, shouldPassValueToHigherRangeAndReturnCurrentAlarmStatusIfHigherRangeOK)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillRepeatedly(Return(ALARM_STATUS_OK));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME)).WillOnce(Return(START_TIME + CLEAR_TIME - 1));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(10, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(7, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH, alarmRange.alarmStatusForValue(7, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

TEST_F(WhenAlarmRangeIsInClearPendingStateAndValueExceedsClearLimitButClearTimeHasNotExpired, shouldPassValueToHigherRangeAndReturnHigherRangeStatusIfNotOk)
{
    Threshold<int32_t> setLimitThreshold(8, &compare_greater_equal);
    Threshold<int32_t> clearLimitThreshold(7, &compare_less_equal);

    EXPECT_CALL(*higherRange, alarmStatusForValue(_, Eq(ByRef(systemTime)), 0, CLEAR_TIME)).WillRepeatedly(Return(ALARM_STATUS_HIGH_HIGH));

    EXPECT_CALL(systemTime, currentTime()).WillOnce(Return(START_TIME)).WillOnce(Return(START_TIME + CLEAR_TIME - 1));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, higherRange);

    ASSERT_EQ(ALARM_STATUS_HIGH_HIGH, alarmRange.alarmStatusForValue(20, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(ALARM_STATE, alarmStateStructFromAlarmRange(alarmRange));

    ASSERT_EQ(ALARM_STATUS_HIGH_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());

    ASSERT_EQ(ALARM_STATUS_HIGH_HIGH, alarmRange.alarmStatusForValue(6, systemTime, 0, CLEAR_TIME));
    ASSERT_EQ(CLEAR_PENDING_STATE, alarmStateStructFromAlarmRange(alarmRange));
    ASSERT_TRUE(alarmRange.isActive());
}

class AlarmRangeHighComparisonTest: public testing::Test
{
public:
    AlarmRangeHighComparisonTest() :
            highHighSetLimit(0), highHighClearLimit(0), highSetLimit(0), highClearLimit(0)
    {
    }
    ~AlarmRangeHighComparisonTest()
    {
    }

    int32_t highHighSetLimit;
    int32_t highHighClearLimit;
    int32_t highSetLimit;
    int32_t highClearLimit;
};

class WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange: public AlarmRangeHighComparisonTest
{
public:
    WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange()
    {
    }
    ~WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange()
    {
    }
};

TEST_F(WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange, DISABLED_shouldReturnFalseIfOtherRangeSetLimitIsNotGreaterThanSetLimit)
{
    highHighSetLimit = 10;
    highSetLimit = 8;

    Threshold<int32_t> highHighSetLimitThreshold(highHighSetLimit, &compare_greater_equal);
    Threshold<int32_t> highHighClearLimitThreshold(0, &compare_greater_equal);
    Threshold<int32_t> highSetLimitThreshold(highSetLimit, &compare_greater_equal);
    Threshold<int32_t> highClearLimitThreshold(0, &compare_greater_equal);


    AlarmRange<int32_t> highHighAlarmRange(highHighSetLimitThreshold, highHighClearLimitThreshold, ALARM_TYPE_HIGH);
    AlarmRange<int32_t> highAlarmRange(highSetLimitThreshold, highClearLimitThreshold, ALARM_TYPE_HIGH);

    ASSERT_FALSE(highHighAlarmRange.isGreaterThan(highAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange, shouldReturnFalseIfOtherRangeClearLimitIsNotGreaterThanSetLimit)
{
    highHighSetLimit = 10;
    highSetLimit = 8;
    highClearLimit = 12;

    Threshold<int32_t> highHighSetLimitThreshold(highHighSetLimit, &compare_greater_equal);
    Threshold<int32_t> highHighClearLimitThreshold(0, &compare_greater_equal);
    Threshold<int32_t> highSetLimitThreshold(highSetLimit, &compare_greater_equal);
    Threshold<int32_t> highClearLimitThreshold(highClearLimit, &compare_greater_equal);

    ASSERT_TRUE(highHighSetLimit > highSetLimit);
    ASSERT_FALSE(highHighSetLimit > highClearLimit);

    AlarmRange<int32_t> highHighAlarmRange(highHighSetLimitThreshold, highHighClearLimitThreshold, ALARM_TYPE_HIGH_HIGH);
    AlarmRange<int32_t> highAlarmRange(highSetLimitThreshold, highClearLimitThreshold, ALARM_TYPE_HIGH);

    ASSERT_FALSE(highHighAlarmRange.isGreaterThan(highAlarmRange));
    ASSERT_FALSE(highAlarmRange.isValid());
}

TEST_F(WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange, shouldReturnFalseIfOtherRangeSetLimitIsNotGreaterEqualToThanClearLimit)
{
    highHighSetLimit = 10;
    highHighClearLimit = 7;
    highSetLimit = 8;
    highClearLimit = 6;

    Threshold<int32_t> highHighSetLimitThreshold(highHighSetLimit, &compare_greater_equal);
    Threshold<int32_t> highHighClearLimitThreshold(0, &compare_greater_equal);
    Threshold<int32_t> highSetLimitThreshold(highSetLimit, &compare_greater_equal);
    Threshold<int32_t> highClearLimitThreshold(highClearLimit, &compare_greater_equal);

    ASSERT_TRUE(highHighSetLimit > highSetLimit);
    ASSERT_TRUE(highHighSetLimit > highClearLimit);
    ASSERT_FALSE(highHighClearLimit >= highSetLimit);

    AlarmRange<int32_t> highHighAlarmRange(highHighSetLimitThreshold, highHighClearLimitThreshold, ALARM_TYPE_HIGH_HIGH);
    AlarmRange<int32_t> highAlarmRange(highSetLimitThreshold, highClearLimitThreshold, ALARM_TYPE_HIGH);

    ASSERT_TRUE(highAlarmRange.isValid());
    ASSERT_TRUE(highHighAlarmRange.isValid());
    ASSERT_FALSE(highHighAlarmRange.isGreaterThan(highAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsGreaterThanAnotherHighAlarmRange, shouldReturnFalseIfOtherRangeClearLimitIsNotGreaterThanClearLimit)
{
    highHighSetLimit = 20;
    highHighClearLimit = 15;
    highSetLimit = 16;
    highClearLimit = 14;

    Threshold<int32_t> highHighSetLimitThreshold(highHighSetLimit, &compare_greater_equal);
    Threshold<int32_t> highHighClearLimitThreshold(0, &compare_greater_equal);
    Threshold<int32_t> highSetLimitThreshold(highSetLimit, &compare_greater_equal);
    Threshold<int32_t> highClearLimitThreshold(highClearLimit, &compare_greater_equal);

    ASSERT_TRUE(highHighSetLimit > highSetLimit);
    ASSERT_TRUE(highHighSetLimit > highClearLimit);
    ASSERT_FALSE(highHighClearLimit >= highSetLimit);

    AlarmRange<int32_t> highHighAlarmRange(highHighSetLimitThreshold, highHighClearLimitThreshold, ALARM_TYPE_HIGH);
    AlarmRange<int32_t> highAlarmRange(highSetLimitThreshold, highClearLimitThreshold, ALARM_TYPE_HIGH);

    ASSERT_TRUE(highAlarmRange.isValid());
    ASSERT_TRUE(highHighAlarmRange.isValid());
    ASSERT_FALSE(highHighAlarmRange.isGreaterThan(highAlarmRange));
}

class AlarmRangeLowComparisonTest: public testing::Test
{
public:
    AlarmRangeLowComparisonTest() :
            lowLowSetLimit(0), lowLowClearLimit(0), lowSetLimit(0), lowClearLimit(0)
    {
    }
    ~AlarmRangeLowComparisonTest()
    {
    }

    int32_t lowLowSetLimit;
    int32_t lowLowClearLimit;
    int32_t lowSetLimit;
    int32_t lowClearLimit;

};

class WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange: public AlarmRangeLowComparisonTest
{
public:
    WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange()
    {
    }
    ~WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange()
    {
    }
};

TEST_F(WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange, shouldReturnTrueIfLimitsBoundsAreLessThanOtherLimitBounds)
{
    lowLowSetLimit = 5;
    lowLowClearLimit = 10;
    lowSetLimit = 15;
    lowClearLimit = 20;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_greater_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(lowLowClearLimit, &compare_greater_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_greater_equal);
    Threshold<int32_t> lowClearLimitThreshold(lowClearLimit, &compare_greater_equal);

    ASSERT_TRUE(lowLowSetLimit < lowSetLimit);
    ASSERT_TRUE(lowLowSetLimit < lowClearLimit);
    ASSERT_TRUE(lowLowClearLimit <= lowSetLimit);
    ASSERT_TRUE(lowLowClearLimit <= lowClearLimit);

    AlarmRange<int32_t> lowLowAlarmRange(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW_LOW);
    AlarmRange<int32_t> lowAlarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);

    ASSERT_TRUE(lowLowAlarmRange.isLessThan(lowAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange, shouldReturnFalseIfOtherRangeSetLimitIsNotLessThanSetLimit)
{
    lowLowSetLimit = 15;
    lowSetLimit = 5;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(0, &compare_less_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(0, &compare_less_equal);

    ASSERT_FALSE(lowLowSetLimit < lowSetLimit);

    AlarmRange<int32_t> lowLowAlarmRange(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW_LOW);
    AlarmRange<int32_t> lowAlarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);

    ASSERT_FALSE(lowLowAlarmRange.isLessThan(lowAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange, shouldReturnFalseIfOtherRangeClearLimitIsNotLessThanSetLimit)
{
    lowLowSetLimit = 5;
    lowLowClearLimit = 10;
    lowSetLimit = 15;
    lowClearLimit = 4;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(0, &compare_less_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(lowClearLimit, &compare_less_equal);

    ASSERT_TRUE(lowLowSetLimit < lowSetLimit);
    ASSERT_FALSE(lowLowSetLimit < lowClearLimit);

    AlarmRange<int32_t> lowLowAlarmRange(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW_LOW);
    AlarmRange<int32_t> lowAlarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);

    ASSERT_FALSE(lowLowAlarmRange.isLessThan(lowAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange, shouldReturnFalseIfOtherRangeSetLimitIsNotLessThanClearLimit)
{
    lowLowSetLimit = 5;
    lowLowClearLimit = 10;
    lowSetLimit = 9;
    lowClearLimit = 20;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(lowLowClearLimit, &compare_less_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(lowClearLimit, &compare_less_equal);

    ASSERT_TRUE(lowLowSetLimit < lowSetLimit);
    ASSERT_TRUE(lowLowSetLimit < lowClearLimit);
    ASSERT_FALSE(lowLowClearLimit < lowSetLimit);

    AlarmRange<int32_t> lowLowAlarmRange(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW);
    AlarmRange<int32_t> lowAlarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);

    ASSERT_FALSE(lowLowAlarmRange.isLessThan(lowAlarmRange));
}

TEST_F(WhenAlarmRangeIsAskedIfItIsLessThanAnotherLowAlarmRange, shouldReturnFalseIfOtherRangeClearLimitIsNotLessThanClearLimit)
{
    lowLowSetLimit = 5;
    lowLowClearLimit = 10;
    lowSetLimit = 15;
    lowClearLimit = 6;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(lowLowClearLimit, &compare_less_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(lowClearLimit, &compare_less_equal);

    ASSERT_TRUE(lowLowSetLimit < lowSetLimit);
    ASSERT_TRUE(lowLowSetLimit < lowClearLimit);
    ASSERT_TRUE(lowLowClearLimit <= lowSetLimit);
    ASSERT_FALSE(lowLowClearLimit < lowClearLimit);

    AlarmRange<int32_t> lowLowAlarmRange(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW);
    AlarmRange<int32_t> lowAlarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);

    ASSERT_FALSE(lowLowAlarmRange.isLessThan(lowAlarmRange));
}

class WhenHighAlarmRangeIsAskedIfItIsValidAndHasNoNextRange: public AlarmRangeTest
{
public:
    WhenHighAlarmRangeIsAskedIfItIsValidAndHasNoNextRange()
    {
    }
    ~WhenHighAlarmRangeIsAskedIfItIsValidAndHasNoNextRange()
    {
    }
};

TEST_F(WhenHighAlarmRangeIsAskedIfItIsValidAndHasNoNextRange, shouldReturnTrueIfSetLimitIsGreaterThanClearLimit)
{

    ASSERT_TRUE(AlarmRange<int32_t>(Threshold<int32_t>(10, &compare_greater_equal), Threshold<int32_t>(9, &compare_greater_equal), ALARM_TYPE_HIGH).isValid());
    ASSERT_TRUE(AlarmRange<int32_t>(Threshold<int32_t>(10, &compare_greater_equal), Threshold<int32_t>(10, &compare_greater_equal), ALARM_TYPE_HIGH).isValid());
}

TEST_F(WhenHighAlarmRangeIsAskedIfItIsValidAndHasNoNextRange, shouldReturnFalseIfSetLimitIsNotGreaterThanClearLimit)
{
    Threshold<int32_t> setLimit(10, &compare_greater_equal);
    Threshold<int32_t> clearLimit(11, &compare_greater_equal);

    ASSERT_FALSE(AlarmRange<int32_t>(setLimit, clearLimit, ALARM_TYPE_HIGH).isValid());
}

class WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange: public AlarmRangeTest
{
public:
    WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange()
    {
    }
    ~WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange()
    {
    }
};

TEST_F(WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnTrueIfSetLimitIsGreaterThanClearLimitAndRangeIsLessThanNextRange)
{
    Threshold<int32_t> setLimit(10, &compare_greater_equal);
    Threshold<int32_t> clearLimit(9, &compare_greater_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_HIGH, higherRange);

    EXPECT_CALL(*higherRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*higherRange, isGreaterThan(Eq(ByRef(alarmRange)))).WillOnce(Return(true));

    ASSERT_TRUE(alarmRange.isValid());
}

TEST_F(WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnFalseIfSetLimitIsGreaterThanClearLimitAndRangeIsNotValid)
{
    Threshold<int32_t> setLimit(10, &compare_greater_equal);
    Threshold<int32_t> clearLimit(9, &compare_greater_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_HIGH, higherRange);

    EXPECT_CALL(*higherRange, isValid()).WillOnce(Return(false));
    EXPECT_CALL(*higherRange, isGreaterThan(Eq(ByRef(alarmRange)))).Times(0);

    ASSERT_FALSE(alarmRange.isValid());
}

TEST_F(WhenHighAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnFalseIfSetLimitIsGreaterThanClearLimitAndRangeIsNotLessThanNextRange)
{
    Threshold<int32_t> setLimit(10, &compare_greater_equal);
    Threshold<int32_t> clearLimit(9, &compare_greater_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_HIGH, higherRange);

    EXPECT_CALL(*higherRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*higherRange, isGreaterThan(Eq(ByRef(alarmRange)))).WillOnce(Return(false));

    ASSERT_FALSE(alarmRange.isValid());
}

class WhenLowAlarmRangeIsAskedIfItIsValidAndHasNoNextRange: public AlarmRangeTest
{
public:
    WhenLowAlarmRangeIsAskedIfItIsValidAndHasNoNextRange()
    {
    }
    ~WhenLowAlarmRangeIsAskedIfItIsValidAndHasNoNextRange()
    {
    }
};

TEST_F(WhenLowAlarmRangeIsAskedIfItIsValidAndHasNoNextRange, shouldReturnTrueIfSetLimitIsLessThanClearLimit)
{
    Threshold<int32_t> lowSetLimitThreshold(10, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(15, &compare_less_equal);
    AlarmRange<int32_t> ar(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW);
    ASSERT_TRUE(ar.isValid());
}

TEST_F(WhenLowAlarmRangeIsAskedIfItIsValidAndHasNoNextRange, shouldReturnFalseIfSetLimitIsNotLessThanClearLimit)
{
    Threshold<int32_t> setLimit(10, &compare_less_equal);
    Threshold<int32_t> clearLimit(9, &compare_less_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_HIGH, higherRange);
    AlarmRange<int32_t> ar(Threshold<int32_t>(10, &compare_greater_equal), Threshold<int32_t>(5, &compare_greater_equal), ALARM_TYPE_LOW);
    ASSERT_FALSE(ar.isValid());
}

class WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange: public AlarmRangeTest
{
public:
    WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange()
    {
    }
    ~WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange()
    {
    }
};

TEST_F(WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnTrueIfSetLimitIsLessThanClearLimitAndRangeIsGreaterThanNextRange)
{
    MockAlarmRange<int32_t> *lowerRange = new MockAlarmRange<int32_t>();

    _setLimit = 10;
    _clearLimit = 15;

    Threshold<int32_t> setLimit(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimit(_clearLimit, &compare_less_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_LOW, lowerRange);

    ASSERT_TRUE(_setLimit <= _clearLimit);
    EXPECT_CALL(*lowerRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*lowerRange, isLessThan(Eq(ByRef(alarmRange)))).WillOnce(Return(true));

    ASSERT_TRUE(alarmRange.isValid());
    delete higherRange;
}

TEST_F(WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnFalseIfSetLimitIsLessThanClearLimitAndRangeIsNotValid)
{
    MockAlarmRange<int32_t> *lowerRange = new MockAlarmRange<int32_t>();

    _setLimit = 20;
    _clearLimit = 25;

    Threshold<int32_t> setLimit(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimit(_clearLimit, &compare_less_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_LOW, lowerRange);

    ASSERT_TRUE(_setLimit <= _clearLimit);
    EXPECT_CALL(*lowerRange, isValid()).WillOnce(Return(false));
    EXPECT_CALL(*lowerRange, isLessThan(Eq(ByRef(alarmRange)))).Times(0);

    ASSERT_FALSE(alarmRange.isValid());
    delete higherRange;
}

TEST_F(WhenLowAlarmRangeIsAskedIfItIsValidAndHasNextRange, shouldReturnFalseIfSetLimitIsLessThanClearLimitAndRangeIsNotLessThanNextRange)
{
    MockAlarmRange<int32_t> *lowerRange = new MockAlarmRange<int32_t>();

    _setLimit = 20;
    _clearLimit = 25;

    Threshold<int32_t> setLimit(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimit(_clearLimit, &compare_less_equal);

    AlarmRange<int32_t> alarmRange(setLimit, clearLimit, ALARM_TYPE_LOW, lowerRange);

    ASSERT_TRUE(_setLimit <= _clearLimit);
    EXPECT_CALL(*lowerRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*lowerRange, isLessThan(Eq(ByRef(alarmRange)))).WillOnce(Return(false));

    ASSERT_FALSE(alarmRange.isValid());
    delete higherRange;
}

TEST(WhenAlarmRangeIsAskedIfHighTypeIsValid, shouldReturnTrueIfRangesAreGE)
{
    int32_t highHighSetLimit = 108;
    int32_t highHighClearLimit = 96;
    int32_t highSetLimit = 96;
    int32_t highClearLimit = 84;

    Threshold<int32_t> highHighSetLimitThreshold(highHighSetLimit, &compare_greater_equal);
    Threshold<int32_t> highHighClearLimitThreshold(highHighClearLimit, &compare_greater_equal);
    Threshold<int32_t> highSetLimitThreshold(highSetLimit, &compare_greater_equal);
    Threshold<int32_t> highClearLimitThreshold(highClearLimit, &compare_greater_equal);

    AlarmRange<int32_t> *highHighAlarmRange = new AlarmRange<int32_t>(highHighSetLimitThreshold, highHighClearLimitThreshold, ALARM_TYPE_HIGH);
    ASSERT_TRUE(AlarmRange<int32_t>(highSetLimitThreshold, highClearLimitThreshold, ALARM_TYPE_HIGH, highHighAlarmRange).isValid());
}

TEST(WhenAlarmRangeIsAskedIfLowTypeIsValid, shouldReturnTrueIfRangesAreGE)
{
    int32_t lowLowSetLimit = 70;
    int32_t lowLowClearLimit = 80;
    int32_t lowSetLimit = 80;
    int32_t lowClearLimit = 85;

    Threshold<int32_t> lowLowSetLimitThreshold(lowLowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowLowClearLimitThreshold(lowLowClearLimit, &compare_less_equal);
    Threshold<int32_t> lowSetLimitThreshold(lowSetLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(lowClearLimit, &compare_less_equal);

    auto *lowLowAlarmRange = new AlarmRange<int32_t>(lowLowSetLimitThreshold, lowLowClearLimitThreshold, ALARM_TYPE_LOW);
    AlarmRange<int32_t> ar(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW, lowLowAlarmRange);
    ASSERT_TRUE(ar.isValid());
}

class WhenAlarmRangeIsAskedToAddValuesToConfiguration: public AlarmRangeTest
{
public:
    ~WhenAlarmRangeIsAskedToAddValuesToConfiguration()
    {
        delete higherRange;
    }

    void SetUp()
    {
        _setLimit = 10;
        _clearLimit = 5;
    }
};

TEST_F(WhenAlarmRangeIsAskedToAddValuesToConfiguration, shouldAddLowValuesToConfiguration)
{
    Threshold<int32_t> lowSetLimitThreshold(_setLimit, &compare_less_equal);
    Threshold<int32_t> lowClearLimitThreshold(_clearLimit, &compare_less_equal);

    AlarmConfiguration<int32_t> alarmConfiguration;
    auto *lowerRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*lowerRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));

    AlarmRange<int32_t> alarmRange(lowSetLimitThreshold, lowClearLimitThreshold, ALARM_TYPE_LOW, lowerRange);
    alarmRange.addValuesToAlarmConfiguration(alarmConfiguration);

    ASSERT_EQ(_setLimit, alarmConfiguration.lowSetLimit());
    ASSERT_EQ(_clearLimit, alarmConfiguration.lowClearLimit());
}

TEST_F(WhenAlarmRangeIsAskedToAddValuesToConfiguration, shouldAddLowLowValuesToConfiguration)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimitThreshold(_clearLimit, &compare_less_equal);

    AlarmConfiguration<int32_t> alarmConfiguration;
    MockAlarmRange<int32_t> *lowerRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*lowerRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_LOW_LOW, lowerRange);
    alarmRange.addValuesToAlarmConfiguration(alarmConfiguration);

    ASSERT_EQ(_setLimit, alarmConfiguration.lowLowSetLimit());
    ASSERT_EQ(_clearLimit, alarmConfiguration.lowLowClearLimit());
}

TEST_F(WhenAlarmRangeIsAskedToAddValuesToConfiguration, shouldAddHighValuesToConfiguration)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimitThreshold(_clearLimit, &compare_less_equal);

    AlarmConfiguration<int32_t> alarmConfiguration;
    auto *lowerRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*lowerRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH, lowerRange);
    alarmRange.addValuesToAlarmConfiguration(alarmConfiguration);

    ASSERT_EQ(_setLimit, alarmConfiguration.highSetLimit());
    ASSERT_EQ(_clearLimit, alarmConfiguration.highClearLimit());
}

TEST_F(WhenAlarmRangeIsAskedToAddValuesToConfiguration, shouldAddHighHighValuesToConfiguration)
{
    Threshold<int32_t> setLimitThreshold(_setLimit, &compare_less_equal);
    Threshold<int32_t> clearLimitThreshold(_clearLimit, &compare_less_equal);

    AlarmConfiguration<int32_t> alarmConfiguration;
    auto *lowerRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*lowerRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));

    AlarmRange<int32_t> alarmRange(setLimitThreshold, clearLimitThreshold, ALARM_TYPE_HIGH_HIGH, lowerRange);
    alarmRange.addValuesToAlarmConfiguration(alarmConfiguration);

    ASSERT_EQ(_setLimit, alarmConfiguration.highHighSetLimit());
    ASSERT_EQ(_clearLimit, alarmConfiguration.highHighClearLimit());
}

}
