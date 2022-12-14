#include <gtest/gtest.h>
#include <cstdint>
#include <domain.h>
#include <Measurement.h>

namespace dios::domain
{

class MeasurementTest: public testing::Test
{
public:
    const index_id_t IOID = 1;
    const int32_t INT32_VALUE = 10;

    MeasurementTest() {}
    ~MeasurementTest() {}
};

TEST_F(MeasurementTest, shouldUpdateTimeWhenValueIsUpdated)
{
    Measurement<int32_t> measurement(DataType::INT32, IOID);

    ASSERT_EQ(0, measurement.currentValue());
    ASSERT_EQ(0, measurement.lastUpdateTime());
    ASSERT_EQ(DataType::INT32, measurement.dataType());
    ASSERT_EQ(IOID, measurement.ioPointId());

    measurement.updateCurrentValue(INT32_VALUE);
    ASSERT_EQ(INT32_VALUE, measurement.currentValue());
    ASSERT_NE(0, measurement.lastUpdateTime());
}

TEST_F(MeasurementTest, shouldUseMoveConstructor)
{
    Measurement<int32_t> measurement1(DataType::INT32, IOID);
    measurement1.updateCurrentValue(INT32_VALUE);

    Measurement<int32_t> measurement2(std::move(measurement1));

    ASSERT_EQ(INT32_VALUE, measurement2.currentValue());
    ASSERT_NE(0, measurement2.lastUpdateTime());
    ASSERT_EQ(DataType::INT32, measurement2.dataType());
    ASSERT_EQ(IOID, measurement2.ioPointId());

    ASSERT_EQ(0, measurement1.currentValue());
    ASSERT_NE(0, measurement1.lastUpdateTime());
    ASSERT_EQ(DataType::UNKNOWN, measurement1.dataType());
    ASSERT_EQ(0, measurement1.ioPointId());
}

TEST_F(MeasurementTest, shouldUseMoveAssignOperator)
{
    Measurement<int32_t> measurement1(DataType::INT32, IOID);
    Measurement<int32_t> measurement2;

    measurement1.updateCurrentValue(INT32_VALUE);
    measurement2 = std::move(measurement1);

    ASSERT_EQ(INT32_VALUE, measurement2.currentValue());
    ASSERT_NE(0, measurement2.lastUpdateTime());
    ASSERT_EQ(DataType::INT32, measurement2.dataType());
    ASSERT_EQ(IOID, measurement2.ioPointId());

    ASSERT_EQ(INT32_VALUE, measurement1.currentValue());
    ASSERT_NE(0, measurement1.lastUpdateTime());
    ASSERT_EQ(DataType::INT32, measurement1.dataType());
    ASSERT_EQ(IOID, measurement1.ioPointId());
}


}
