#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <Measurement.h>
#include <Deadband.h>
#include <AlarmMeister.h>
#include <AlarmRange.h>
#include <MockClock.h>
#include <MeasurementFacade.h>
#include <Clock.h>
#include <ctime>

#include "MockAlarmMeister.h"
#include "MockDeadband.h"
#include "MockMeasurement.h"

namespace dios::domain
{
    using namespace std;
    using namespace dios::foundation;

    using testing::Mock;
    using testing::Return;
    using testing::InSequence;

    static const int32_t CURRENT_VALUE = 100;
    static const int32_t NEW_VALUE = 105;

    class MeasurementFacadeTest : public testing::Test
    {
    public:
        MeasurementFacadeTest()
        {
        }
        ~MeasurementFacadeTest() = default;
    };

    TEST_F(MeasurementFacadeTest, shouldCleanUp)
    {
        MockAlarmMeister<int32_t>* alarmMeister = new MockAlarmMeister<int32_t>(true);
        MockMeasurement<int32_t>* measurement = new MockMeasurement<int32_t>(true);
        MockDeadband<int32_t>* deadband = new MockDeadband<int32_t>(true);

        {
            MeasurementFacade<int32_t> measurementFacade(measurement, alarmMeister, deadband);
        }

        Mock::VerifyAndClearExpectations(alarmMeister);
        Mock::VerifyAndClearExpectations(measurement);
        Mock::VerifyAndClearExpectations(deadband);

    }

    class MeasurementFacadeUpdateValueTest : public MeasurementFacadeTest
    {
    public:
        MeasurementFacadeUpdateValueTest() :
            alarmMeister(nullptr),
            measurement(nullptr),
            deadband(nullptr)
        {
        }
        ~MeasurementFacadeUpdateValueTest() = default;

        void SetUp()
        {
            alarmMeister = new MockAlarmMeister<int32_t>();
            measurement = new MockMeasurement<int32_t>();
            deadband = new MockDeadband<int32_t>();
        }

        MockAlarmMeister<int32_t>* alarmMeister;
        MockMeasurement<int32_t>* measurement;
        MockDeadband<int32_t>* deadband;
    };

    TEST_F(MeasurementFacadeUpdateValueTest, shouldReturnCorrectStatusWhenUpdateCurrentValueOnly)
    {
        time_t now = ::time(nullptr);
        MeasurementFacade<int32_t> measurementFacade(measurement, alarmMeister, deadband);

        InSequence seq;
        EXPECT_CALL(*measurement, currentValue()).WillOnce(Return(CURRENT_VALUE));
        EXPECT_CALL(*deadband, currentValueHasChanged(CURRENT_VALUE, NEW_VALUE)).WillOnce(Return(true));
        EXPECT_CALL(*alarmMeister, alarmStatusForValue(NEW_VALUE)).WillOnce(Return(AlarmStatus::ALARM_STATUS_OK));
        EXPECT_CALL(*measurement, alarmStatus()).WillOnce(Return(AlarmStatus::ALARM_STATUS_OK));
        EXPECT_CALL(*measurement, updateCurrentValue(NEW_VALUE, AlarmStatus::ALARM_STATUS_OK));
        EXPECT_CALL(*measurement, lastUpdateTime()).WillOnce(Return(now));

        auto updateStatus = measurementFacade.updateValue(NEW_VALUE);

        ASSERT_EQ(UpdateAction::VALUE_UPDATE, updateStatus.updateAction);
        ASSERT_EQ(NEW_VALUE, updateStatus.currentValue);
        ASSERT_EQ(now, updateStatus.lastUpdateTime);
        ASSERT_EQ(AlarmStatus::ALARM_STATUS_OK, updateStatus.alarmStatus);
    }

    TEST_F(MeasurementFacadeUpdateValueTest, shouldReturnCorrectStatusWhenUpdateAlarmStatus)
    {
        time_t now = ::time(nullptr);
        MeasurementFacade<int32_t> measurementFacade(measurement, alarmMeister, deadband);

        InSequence seq;
        EXPECT_CALL(*measurement, currentValue()).WillOnce(Return(CURRENT_VALUE));
        EXPECT_CALL(*deadband, currentValueHasChanged(CURRENT_VALUE, NEW_VALUE)).WillOnce(Return(true));
        EXPECT_CALL(*alarmMeister, alarmStatusForValue(NEW_VALUE)).WillOnce(Return(AlarmStatus::ALARM_STATUS_HIGH));
        EXPECT_CALL(*measurement, alarmStatus()).WillOnce(Return(AlarmStatus::ALARM_STATUS_OK));
        EXPECT_CALL(*measurement, updateCurrentValue(NEW_VALUE, AlarmStatus::ALARM_STATUS_HIGH));
        EXPECT_CALL(*measurement, lastUpdateTime()).WillOnce(Return(now));

        auto updateStatus = measurementFacade.updateValue(NEW_VALUE);

        ASSERT_EQ(UpdateAction::ALARM_CHANGE, updateStatus.updateAction);
        ASSERT_EQ(NEW_VALUE, updateStatus.currentValue);
        ASSERT_EQ(now, updateStatus.lastUpdateTime);
        ASSERT_EQ(AlarmStatus::ALARM_STATUS_HIGH, updateStatus.alarmStatus);
    }
}
