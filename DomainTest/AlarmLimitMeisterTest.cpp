/** @file AlarmLimitMeisterTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Dec 10, 2015 by daver:
 * @endhistory
 */
#include <map>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockClock.h>
#include <AlarmLimitMeister.h>
#include <AlarmConfiguration.h>
#include <testutil.h>

#include "MockAlarmRange.h"

namespace zios::domain
{

using namespace std;
using namespace zios::foundation;
using namespace zios::test;

using testing::Mock;
using testing::InSequence;
using testing::Eq;
using testing::ByRef;
using testing::Return;
using testing::Return;

TEST(JustNeedSomethingToTest, thenJustDoIt)
{
    AlarmConfiguration<float> alarmConfiguration;
    alarmConfiguration.setLowLimits(100.3, 105.5);
    auto *theMeister = AlarmLimitMeister<float>::alarmLimitMeisterForAlarmConfiguration(alarmConfiguration);
    ASSERT_TRUE(nullptr != theMeister);
    float value(14.5);
    AlarmStatus alarmStatus = theMeister->alarmStatusForValue(value);
    ASSERT_EQ(ALARM_STATUS_LOW, alarmStatus);
    delete theMeister;
}

class AlarmLimitMeisterTest: public testing::Test
{
public:
    AlarmLimitMeisterTest() = default;
    ~AlarmLimitMeisterTest() = default;


    static const uint16_t SET_TIME;
    static const uint16_t CLEAR_TIME;
};

const uint16_t AlarmLimitMeisterTest::SET_TIME = 10;
const uint16_t AlarmLimitMeisterTest::CLEAR_TIME = 15;

class WhenAlarmLimitMeisterIsDestroyed: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterIsDestroyed() = default;
    ~WhenAlarmLimitMeisterIsDestroyed() = default;
};

TEST_F(WhenAlarmLimitMeisterIsDestroyed, shouldCleanUpObjects)
{
    auto *secondaryRange = new MockAlarmRange<int32_t>(true);
    auto *primaryRange = new MockAlarmRange<int32_t>(true);

    {
        AlarmLimitMeister<int32_t> meister(*secondaryRange, primaryRange, 0, 0);
    }

    Mock::VerifyAndClearExpectations(secondaryRange);
    Mock::VerifyAndClearExpectations(primaryRange);
}

class WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValue: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValue()
    {
    }
    ~WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValue()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValue, shouldPassValueToPrimaryRangeFirstAndThenToLowRangeIfPrimaryRangeIsInactive)
{
    auto *secondaryRange = new MockAlarmRange<int32_t>();
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    InSequence seq;
    EXPECT_CALL(*primaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*secondaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*primaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));
    EXPECT_CALL(*primaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*secondaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME, systemTime);

    ASSERT_EQ(ALARM_STATUS_OK, meister.alarmStatusForValue(10));

}

class WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValueThatMakesPrimaryRangeActive: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValueThatMakesPrimaryRangeActive() = default;
    ~WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValueThatMakesPrimaryRangeActive() = default;
};

TEST_F(WhenAlarmLimitMeisterHasNoActiveRangesAndIsAskedToDetermineTheAlarmSatusOfAValueThatMakesPrimaryRangeActive, shouldPassValueToPrimarRangeAndNoOtherRange)
{
    auto *secondaryRange = new MockAlarmRange<int32_t>();
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    InSequence seq;
    EXPECT_CALL(*primaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*secondaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*primaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));
    EXPECT_CALL(*primaryRange, isActive()).WillOnce(Return(true));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME, systemTime);

    ASSERT_EQ(ALARM_STATUS_OK, meister.alarmStatusForValue(10));

}

class WhenAlarmLimitMeisterHasActiveSecondaryRangesAndIsAskedToDetermineTheAlarmSatusOfAValue: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasActiveSecondaryRangesAndIsAskedToDetermineTheAlarmSatusOfAValue()
    {
    }
    ~WhenAlarmLimitMeisterHasActiveSecondaryRangesAndIsAskedToDetermineTheAlarmSatusOfAValue()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterHasActiveSecondaryRangesAndIsAskedToDetermineTheAlarmSatusOfAValue, shouldPassValueToLowerRangeAndNoOtherRange)
{
    auto *secondaryRange = new MockAlarmRange<int32_t>();
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    InSequence seq;
    EXPECT_CALL(*primaryRange, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*secondaryRange, isActive()).WillOnce(Return(true));
    EXPECT_CALL(*secondaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));
    EXPECT_CALL(*secondaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).Times(0);

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME, systemTime);

    ASSERT_EQ(ALARM_STATUS_OK, meister.alarmStatusForValue(10));
}

class WhenAlarmLimitMeisterHasOnlyOneRange: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasOnlyOneRange()
    {
    }
    ~WhenAlarmLimitMeisterHasOnlyOneRange()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterHasOnlyOneRange, shouldPassValueToRange)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    EXPECT_CALL(*primaryRange, alarmStatusForValue(10, Eq(ByRef(systemTime)), SET_TIME, CLEAR_TIME)).WillOnce(Return(ALARM_STATUS_OK));

    AlarmLimitMeister<int32_t> meister(*primaryRange, nullptr, SET_TIME, CLEAR_TIME, systemTime);

    ASSERT_EQ(ALARM_STATUS_OK, meister.alarmStatusForValue(10));
}

class WhenAlarmLimitMeisterHasOnlyOneRangeAndIsAskedToClear: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasOnlyOneRangeAndIsAskedToClear()
    {
    }
    ~WhenAlarmLimitMeisterHasOnlyOneRangeAndIsAskedToClear()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterHasOnlyOneRangeAndIsAskedToClear, shouldClearOnlySingleRange)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    EXPECT_CALL(*primaryRange, clear());

    AlarmLimitMeister<int32_t> meister(*primaryRange, nullptr, SET_TIME, CLEAR_TIME, systemTime);
    meister.clear();
}

class WhenAlarmLimitMeisterHasTwoRangesAndIsAskedToClear: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterHasTwoRangesAndIsAskedToClear()
    {
    }
    ~WhenAlarmLimitMeisterHasTwoRangesAndIsAskedToClear()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterHasTwoRangesAndIsAskedToClear, shouldClearBothRanges)
{
    auto *secondaryRange = new MockAlarmRange<int32_t>();
    auto *primaryRange = new MockAlarmRange<int32_t>();
    MockClock systemTime;

    EXPECT_CALL(*primaryRange, clear());
    EXPECT_CALL(*secondaryRange, clear());

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME, systemTime);
    meister.clear();
}

class WhenAlarmLimitMeisterIsAskedValidWithOnlyOneRange: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterIsAskedValidWithOnlyOneRange()
    {
    }
    ~WhenAlarmLimitMeisterIsAskedValidWithOnlyOneRange()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterIsAskedValidWithOnlyOneRange, shouldDetectNoConfiguration)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(true));

    AlarmLimitMeister<int32_t> meister(*primaryRange, nullptr, SET_TIME, CLEAR_TIME);
    ASSERT_TRUE(meister.isValid());
}

class WhenAlarmLimitMeisterIsAskedValidWithTwoRanges: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterIsAskedValidWithTwoRanges()
    {
    }
    ~WhenAlarmLimitMeisterIsAskedValidWithTwoRanges()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterIsAskedValidWithTwoRanges, shouldDetectValidRangeBoundariesWhenPrimaryIsHigher)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();

    int32_t primarySetLimit(100);
    int32_t primaryClearLimit(80);
    int32_t secondarySetLimit(30);
    int32_t secondaryClearLimit(40);

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*secondaryRange, isValid()).WillOnce(Return(true));

    EXPECT_CALL(*primaryRange, highestSetLimit()).WillOnce(Return(primarySetLimit));
    EXPECT_CALL(*primaryRange, lowestClearLimit()).WillOnce(Return(primaryClearLimit));
    EXPECT_CALL(*secondaryRange, highestSetLimit()).WillOnce(Return(secondarySetLimit));
    EXPECT_CALL(*secondaryRange, lowestClearLimit()).WillOnce(Return(secondaryClearLimit));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    ASSERT_TRUE(meister.isValid());
}

TEST_F(WhenAlarmLimitMeisterIsAskedValidWithTwoRanges, shouldReturnFalseIfPrimaryRangeIsInvalid)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(false));
    EXPECT_CALL(*secondaryRange, isValid()).Times(0);

    EXPECT_CALL(*primaryRange, highestSetLimit()).Times(0);
    EXPECT_CALL(*primaryRange, lowestClearLimit()).Times(0);
    EXPECT_CALL(*secondaryRange, highestSetLimit()).Times(0);
    EXPECT_CALL(*secondaryRange, lowestClearLimit()).Times(0);

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    ASSERT_FALSE(meister.isValid());
}

TEST_F(WhenAlarmLimitMeisterIsAskedValidWithTwoRanges, shouldReturnFalseIfSecondaryRangeIsInvalid)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*secondaryRange, isValid()).WillOnce(Return(false));

    EXPECT_CALL(*primaryRange, highestSetLimit()).Times(0);
    EXPECT_CALL(*primaryRange, lowestClearLimit()).Times(0);
    EXPECT_CALL(*secondaryRange, highestSetLimit()).Times(0);
    EXPECT_CALL(*secondaryRange, lowestClearLimit()).Times(0);

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    ASSERT_FALSE(meister.isValid());
}
TEST_F(WhenAlarmLimitMeisterIsAskedValidWithTwoRanges, shouldDetectValidRangeBoundariesWhenPrimaryIsLower)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();

    int32_t primarySetLimit(30);
    int32_t primaryClearLimit(40);
    int32_t secondarySetLimit(100);
    int32_t secondaryClearLimit(80);

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*secondaryRange, isValid()).WillOnce(Return(true));

    EXPECT_CALL(*primaryRange, highestSetLimit()).WillOnce(Return(primarySetLimit));
    EXPECT_CALL(*primaryRange, lowestClearLimit()).WillOnce(Return(primaryClearLimit));
    EXPECT_CALL(*secondaryRange, highestSetLimit()).WillOnce(Return(secondarySetLimit));
    EXPECT_CALL(*secondaryRange, lowestClearLimit()).WillOnce(Return(secondaryClearLimit));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    ASSERT_TRUE(meister.isValid());
}

TEST_F(WhenAlarmLimitMeisterIsAskedValidWithTwoRanges, shouldDetectOverLappingBoundariesHigh)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();

    int32_t primarySetLimit(100);
    int32_t primaryClearLimit(30);
    int32_t secondarySetLimit(80);
    int32_t secondaryClearLimit(40);

    EXPECT_CALL(*primaryRange, isValid()).WillOnce(Return(true));
    EXPECT_CALL(*secondaryRange, isValid()).WillOnce(Return(true));

    EXPECT_CALL(*primaryRange, highestSetLimit()).WillOnce(Return(primarySetLimit));
    EXPECT_CALL(*primaryRange, lowestClearLimit()).WillOnce(Return(primaryClearLimit));
    EXPECT_CALL(*secondaryRange, highestSetLimit()).WillOnce(Return(secondarySetLimit));
    EXPECT_CALL(*secondaryRange, lowestClearLimit()).WillOnce(Return(secondaryClearLimit));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    ASSERT_FALSE(meister.isValid());
}

class WhenAlarmLimitMeisterIsAskedToConstructAlarmLimitMeisterAndNoActiveLimitAlarms: public AlarmLimitMeisterTest
{
public:
    WhenAlarmLimitMeisterIsAskedToConstructAlarmLimitMeisterAndNoActiveLimitAlarms()
    {
    }
    ~WhenAlarmLimitMeisterIsAskedToConstructAlarmLimitMeisterAndNoActiveLimitAlarms()
    {
    }
};

TEST_F(WhenAlarmLimitMeisterIsAskedToConstructAlarmLimitMeisterAndNoActiveLimitAlarms, shouldReturnNullWithSuccessCode)
{
    AlarmConfiguration<int32_t> alarmConfig;

    ASSERT_TRUE(nullptr == AlarmLimitMeister<int32_t>::alarmLimitMeisterForAlarmConfiguration(alarmConfig));
}

class WhenAlarmLimitMeitsterIsAskedToAddValuesToAlarmConfiguration: public AlarmLimitMeisterTest
{
public:
    ~WhenAlarmLimitMeitsterIsAskedToAddValuesToAlarmConfiguration()
    {
    }
};

TEST_F(WhenAlarmLimitMeitsterIsAskedToAddValuesToAlarmConfiguration, shouldAddClearAndSetTimesAndThenPassToRanges)
{
    auto *primaryRange = new MockAlarmRange<int32_t>();
    auto *secondaryRange = new MockAlarmRange<int32_t>();
    AlarmConfiguration<int32_t> alarmConfiguration;

    EXPECT_CALL(*primaryRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));
    EXPECT_CALL(*secondaryRange, addValuesToAlarmConfiguration(Eq(ByRef(alarmConfiguration))));

    AlarmLimitMeister<int32_t> meister(*primaryRange, secondaryRange, SET_TIME, CLEAR_TIME);
    meister.addValuesToAlarmConfiguration(alarmConfiguration);

    ASSERT_EQ(SET_TIME, alarmConfiguration.setTimeSeconds());
    ASSERT_EQ(CLEAR_TIME, alarmConfiguration.clearTimeSeconds());
}

TEST(WhenAlarmLimitMeisterIsAskedToCreateAlarmLimitMeisterFromAlarmConfigurationWithHighInvalidRange, shouldNotReturnMeisterAndReturnInvalidInErrorCode)
{
    AlarmConfiguration<int32_t> alarmConfiguration;

    alarmConfiguration.setClearTime(100);
    alarmConfiguration.setSetTime(100);
    alarmConfiguration.setHighHighLimits(108, 96);
    alarmConfiguration.setHighLimits(96, 84);

    auto *theMeister = AlarmLimitMeister<int32_t>::alarmLimitMeisterForAlarmConfiguration(alarmConfiguration);
    ASSERT_TRUE(nullptr != theMeister);
    delete theMeister;
}

TEST(WhenAlarmLimitMeisterIsAskedToCreateAlarmLimitMeisterFromAlarmConfigurationWithLowInvalidRange, shouldNotReturnMeisterAndReturnInvalidInErrorCode)
{
    AlarmConfiguration<int32_t> alarmConfiguration;

    alarmConfiguration.setClearTime(100);
    alarmConfiguration.setSetTime(100);
    alarmConfiguration.setLowLowLimits(70, 80);
    alarmConfiguration.setLowLimits(80, 85);

    auto *theMeister = AlarmLimitMeister<int32_t>::alarmLimitMeisterForAlarmConfiguration(alarmConfiguration);
    ASSERT_TRUE(nullptr != theMeister);
    delete theMeister;
}

TEST(WhenAlarmLimitMeisterIsAskedToCreateAlarmLimitMeisterFromAlarmConfigurationWithLowAndHighInvalidRange, shouldNotReturnMeisterAndReturnInvalidInErrorCode)
{
    AlarmConfiguration<int32_t> alarmConfiguration;

    alarmConfiguration.setClearTime(100);
    alarmConfiguration.setSetTime(100);
    alarmConfiguration.setHighHighLimits(208, 196);
    alarmConfiguration.setHighLimits(196, 184);
    alarmConfiguration.setLowLowLimits(70, 80);
    alarmConfiguration.setLowLimits(80, 85);

    auto *theMeister = AlarmLimitMeister<int32_t>::alarmLimitMeisterForAlarmConfiguration(alarmConfiguration);
    ASSERT_TRUE(nullptr != theMeister);
    delete theMeister;
}

TEST(WhenAlarmLimitMeisterIsAskedToCreateAlarmLimitMeisterFromAlarmConfigurationWithLowAndHighValidRanges, shouldReturnMeisterAndReturnSuccessInErrorCode)
{
    AlarmConfiguration<int32_t> alarmConfiguration;

    alarmConfiguration.setClearTime(100);
    alarmConfiguration.setSetTime(100);
    alarmConfiguration.setHighHighLimits(208, 197);
    alarmConfiguration.setHighLimits(196, 184);
    alarmConfiguration.setLowLimits(84, 96);
    alarmConfiguration.setLowLowLimits(80, 83);

    auto *meister = AlarmLimitMeister<int32_t>::alarmLimitMeisterForAlarmConfiguration(alarmConfiguration);
    ASSERT_TRUE(nullptr != meister);
    delete meister;
}

}
