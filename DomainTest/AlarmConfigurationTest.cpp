/** @file AlarmConfigurationTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Dec 17, 2015 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <map>
#include <memory>
#include <AlarmConfiguration.h>
#include <AlarmConfigurationSerializer.h>

namespace dios
{
namespace domain
{

using namespace std;
using namespace dios::foundation;

using testing::Mock;

class AlarmConfigurationTest: public testing::Test
{
public:
    AlarmConfigurationTest()
    {
    }
    ~AlarmConfigurationTest()
    {
    }

    static const uint16_t SET_TIME;
    static const uint16_t CLEAR_TIME;
    static const uint32_t LOW_LOW_SET_LIMIT;
    static const uint32_t LOW_LOW_CLEAR_LIMIT;
    static const uint32_t LOW_SET_LIMIT;
    static const uint32_t LOW_CLEAR_LIMIT;
    static const uint32_t HIGH_HIGH_SET_LIMIT;
    static const uint32_t HIGH_HIGH_CLEAR_LIMIT;
    static const uint32_t HIGH_SET_LIMIT;
    static const uint32_t HIGH_CLEAR_LIMIT;

    static const string MASK_STR;
    static const string SET_TIME_STR;
    static const string CLEAR_TIME_STR;
    static const string LOW_LOW_SET_LIMIT_STR;
    static const string LOW_LOW_CLEAR_LIMIT_STR;
    static const string LOW_SET_LIMIT_STR;
    static const string LOW_CLEAR_LIMIT_STR;
    static const string HIGH_HIGH_SET_LIMIT_STR;
    static const string HIGH_HIGH_CLEAR_LIMIT_STR;
    static const string HIGH_SET_LIMIT_STR;
    static const string HIGH_CLEAR_LIMIT_STR;

    uint32_t alarmMaskFromAlarmConfiguration(AlarmConfiguration<int32_t> &alarmConfiguration)
    {
        return alarmConfiguration._alarmMask;
    }

};

const uint16_t AlarmConfigurationTest::SET_TIME = 100;
const uint16_t AlarmConfigurationTest::CLEAR_TIME = 50;
const uint32_t AlarmConfigurationTest::LOW_LOW_SET_LIMIT = 30;
const uint32_t AlarmConfigurationTest::LOW_LOW_CLEAR_LIMIT = 40;
const uint32_t AlarmConfigurationTest::LOW_SET_LIMIT = 50;
const uint32_t AlarmConfigurationTest::LOW_CLEAR_LIMIT = 60;
const uint32_t AlarmConfigurationTest::HIGH_HIGH_SET_LIMIT = 100;
const uint32_t AlarmConfigurationTest::HIGH_HIGH_CLEAR_LIMIT = 90;
const uint32_t AlarmConfigurationTest::HIGH_SET_LIMIT = 80;
const uint32_t AlarmConfigurationTest::HIGH_CLEAR_LIMIT = 70;

const string AlarmConfigurationTest::MASK_STR = "15";
const string AlarmConfigurationTest::SET_TIME_STR = "100";
const string AlarmConfigurationTest::CLEAR_TIME_STR = "50";
const string AlarmConfigurationTest::LOW_LOW_SET_LIMIT_STR = "30";
const string AlarmConfigurationTest::LOW_LOW_CLEAR_LIMIT_STR = "40";
const string AlarmConfigurationTest::LOW_SET_LIMIT_STR = "50";
const string AlarmConfigurationTest::LOW_CLEAR_LIMIT_STR = "60";
const string AlarmConfigurationTest::HIGH_HIGH_SET_LIMIT_STR = "100";
const string AlarmConfigurationTest::HIGH_HIGH_CLEAR_LIMIT_STR = "90";
const string AlarmConfigurationTest::HIGH_SET_LIMIT_STR = "80";
const string AlarmConfigurationTest::HIGH_CLEAR_LIMIT_STR = "70";

class WhenAlarmConfigurationIsAskedIfAnyLimitAlarmIsActive: public AlarmConfigurationTest
{
public:
    ~WhenAlarmConfigurationIsAskedIfAnyLimitAlarmIsActive()
    {
    }
};

TEST_F(WhenAlarmConfigurationIsAskedIfAnyLimitAlarmIsActive, shouldReturnFalseIfNoAlarmIsEnabled)
{
    AlarmConfiguration<int32_t> alarmConfiguration;
    ASSERT_FALSE(alarmConfiguration.isAnyLimitAlarmActive());
}

TEST_F(WhenAlarmConfigurationIsAskedIfAnyLimitAlarmIsActive, shouldReturnFalseIfOnlyNoDataAlarmIsEnabled)
{
    AlarmConfiguration<int32_t> alarmConfiguration;
    alarmConfiguration.setNoDataEnabled(true);
    ASSERT_FALSE(alarmConfiguration.isAnyLimitAlarmActive());
}

TEST_F(WhenAlarmConfigurationIsAskedIfAnyLimitAlarmIsActive, shouldReturnTrueIfNoDataAlarmIsEnabledAndAtLeastOneLmitAlarmIsEnabled)
{
    AlarmConfiguration<int32_t> alarmConfiguration;
    alarmConfiguration.setNoDataEnabled(true);
    alarmConfiguration.setHighLimits(HIGH_SET_LIMIT, HIGH_CLEAR_LIMIT);
    ASSERT_TRUE(alarmConfiguration.isAnyLimitAlarmActive());
}

class WhenAlarmConfigurationIsAskedToDisableOrDisableNoDataAlarm: public AlarmConfigurationTest
{
public:
    ~WhenAlarmConfigurationIsAskedToDisableOrDisableNoDataAlarm()
    {
    }
};

TEST_F(WhenAlarmConfigurationIsAskedToDisableOrDisableNoDataAlarm, shouldClearOrSetBitInAlarmMask)
{
    AlarmConfiguration<int32_t> alarmConfiguration;
    alarmConfiguration.setNoDataEnabled(true);

    ASSERT_EQ(0x10, alarmMaskFromAlarmConfiguration(alarmConfiguration));
    ASSERT_TRUE(alarmConfiguration.isNoDataEnabled());
    alarmConfiguration.setNoDataEnabled(false);
    ASSERT_EQ(0, alarmMaskFromAlarmConfiguration(alarmConfiguration));
    ASSERT_FALSE(alarmConfiguration.isNoDataEnabled());
}

TEST_F(WhenAlarmConfigurationIsAskedToDisableOrDisableNoDataAlarm, shouldNoEffectOtherBitsInAlarmMask)
{
    AlarmConfiguration<int32_t> alarmConfiguration;
    alarmConfiguration.setHighHighLimits(0, 1);
    alarmConfiguration.setNoDataEnabled(true);

    ASSERT_TRUE(alarmConfiguration.isHighHighAlarmActive());
    ASSERT_TRUE(alarmConfiguration.isNoDataEnabled());
    alarmConfiguration.setNoDataEnabled(false);
    ASSERT_TRUE(alarmConfiguration.isHighHighAlarmActive());
    ASSERT_FALSE(alarmConfiguration.isNoDataEnabled());
}

class WhenAlarmConfigurationIsAskedToAssignToAnotherAlarmConfiguration: public AlarmConfigurationTest
{
public:
    ~WhenAlarmConfigurationIsAskedToAssignToAnotherAlarmConfiguration()
    {
    }

    void SetUp()
    {
        rhsAlarmConfiguration.setClearTime(CLEAR_TIME);
        rhsAlarmConfiguration.setSetTime(SET_TIME);

        rhsAlarmConfiguration.setLowLimits(LOW_SET_LIMIT, LOW_CLEAR_LIMIT);
        rhsAlarmConfiguration.setLowLowLimits(LOW_LOW_SET_LIMIT, LOW_LOW_CLEAR_LIMIT);
        rhsAlarmConfiguration.setHighLimits(HIGH_SET_LIMIT, HIGH_CLEAR_LIMIT);
        rhsAlarmConfiguration.setHighHighLimits(HIGH_HIGH_SET_LIMIT, HIGH_HIGH_CLEAR_LIMIT);

    }

    AlarmConfiguration<int32_t> rhsAlarmConfiguration;
};

TEST_F(WhenAlarmConfigurationIsAskedToAssignToAnotherAlarmConfiguration, shouldAssignValuesAndLeaveOtherObjectAlone)
{
    AlarmConfiguration<int32_t> lhsAlarmConfiguration;

    lhsAlarmConfiguration = rhsAlarmConfiguration;

    ASSERT_EQ(rhsAlarmConfiguration.lowClearLimit(), lhsAlarmConfiguration.lowClearLimit());
    ASSERT_EQ(rhsAlarmConfiguration.lowSetLimit(), lhsAlarmConfiguration.lowSetLimit());

    ASSERT_EQ(rhsAlarmConfiguration.lowLowClearLimit(), lhsAlarmConfiguration.lowLowClearLimit());
    ASSERT_EQ(rhsAlarmConfiguration.lowLowSetLimit(), lhsAlarmConfiguration.lowLowSetLimit());

    ASSERT_EQ(rhsAlarmConfiguration.highClearLimit(), lhsAlarmConfiguration.highClearLimit());
    ASSERT_EQ(rhsAlarmConfiguration.highSetLimit(), lhsAlarmConfiguration.highSetLimit());

    ASSERT_EQ(rhsAlarmConfiguration.highHighClearLimit(), lhsAlarmConfiguration.highHighClearLimit());
    ASSERT_EQ(rhsAlarmConfiguration.highHighSetLimit(), lhsAlarmConfiguration.highHighSetLimit());
}

class WhenAlarmConfigurationIsAskedToClear: public AlarmConfigurationTest
{
public:
    ~WhenAlarmConfigurationIsAskedToClear()
    {
    }
};

TEST_F(WhenAlarmConfigurationIsAskedToClear, shouldDeleteAllValuesObjectsAndSetAlarmMaskToZero)
{
    int32_t hhSetLimit = 0;
    int32_t hhClearLimit = 0;
    int32_t hSetLimit = 0;
    int32_t hClearLimit = 0;
    int32_t llSetLimit = 0;
    int32_t llClearLimit = 0;
    int32_t lSetLimit = 0;
    int32_t lClearLimit = 0;

    AlarmConfiguration<int32_t> alarmConfig;
    alarmConfig.setNoDataEnabled(true);
    alarmConfig.setClearTime(CLEAR_TIME);
    alarmConfig.setSetTime(SET_TIME);
    alarmConfig.setLowLimits(lSetLimit, lClearLimit);
    alarmConfig.setLowLowLimits(llSetLimit, llClearLimit);
    alarmConfig.setHighLimits(hSetLimit, hClearLimit);
    alarmConfig.setHighHighLimits(hhSetLimit, hhClearLimit);

    ASSERT_TRUE(alarmConfig.isAnyLimitAlarmActive());
    ASSERT_TRUE(alarmConfig.isHighAlarmActive());
    ASSERT_TRUE(alarmConfig.isHighHighAlarmActive());
    ASSERT_TRUE(alarmConfig.isLowAlarmActive());
    ASSERT_TRUE(alarmConfig.isLowLowAlarmActive());
    ASSERT_TRUE(alarmConfig.isNoDataEnabled());

    alarmConfig.clear();

    ASSERT_FALSE(alarmConfig.isAnyLimitAlarmActive());
    ASSERT_FALSE(alarmConfig.isHighAlarmActive());
    ASSERT_FALSE(alarmConfig.isHighHighAlarmActive());
    ASSERT_FALSE(alarmConfig.isLowAlarmActive());
    ASSERT_FALSE(alarmConfig.isLowLowAlarmActive());
    ASSERT_FALSE(alarmConfig.isNoDataEnabled());
}

}
}

