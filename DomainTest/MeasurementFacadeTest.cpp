#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <Measurement.h>
#include <Deadband.h>
#include <AlarmMeister.h>
#include <AlarmRange.h>
#include <MockClock.h>
#include <MeasurementFacade.h>

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
        MeasurementFacadeTest() :
            alarmMeister(nullptr),
            measurement(nullptr),
            deadband(nullptr)
        {
        }
        ~MeasurementFacadeTest() = default;

        void SetUp()
        {
            alarmMeister = new MockAlarmMeister<int32_t>(true);
            measurement = new MockMeasurement<int32_t>(true);
            deadband = new MockDeadband<int32_t>(true);
        }

        MockAlarmMeister<int32_t>* alarmMeister;
        MockMeasurement<int32_t>* measurement;
        MockDeadband<int32_t>* deadband;
    };

    TEST_F(MeasurementFacadeTest, shouldReturnCorrectStatus)
    {
        MeasurementFacade<int32_t> measurementFacade(measurement, alarmMeister, deadband);

        InSequence seq;
        EXPECT_CALL(*measurement, currentValue()).WillOnce(Return(CURRENT_VALUE));
        EXPECT_CALL(*deadband, currentValueHasChanged(CURRENT_VALUE, NEW_VALUE)).WillOnce(Return(true));
        EXPECT_CALL(*measurement, updateCurrentValue(NEW_VALUE));

        measurementFacade.updateValue(NEW_VALUE);

        Mock::VerifyAndClearExpectations(alarmMeister);
        Mock::VerifyAndClearExpectations(measurement);
        Mock::VerifyAndClearExpectations(deadband);
    }
}
