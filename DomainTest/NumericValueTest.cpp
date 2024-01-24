#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockByteBuffer.h>

#include <Value.h>

namespace zios::domain
{

using zios::common::ByteBuffer;

#define NumericValueTestSuite(test_suite_name, test_name)    \
  GTEST_TEST_(test_suite_name, test_name, NumericValueTest,  \
              ::testing::internal::GetTestTypeId())


class NumericValueTest : public testing::Test
{
public:
    NumericValueTest() = default;
    virtual ~NumericValueTest() = default;
};


template<typename T>
void compareTest()
{
    NumericValue<T> value1(1);

    ASSERT_TRUE(value1 < NumericValue<int8_t>(2));
    ASSERT_TRUE(value1 < NumericValue<uint8_t>(2));
    ASSERT_TRUE(value1 < NumericValue<int16_t>(2));
    ASSERT_TRUE(value1 < NumericValue<uint16_t>(2));
    ASSERT_FALSE(value1 > NumericValue<int32_t>(2));
    ASSERT_TRUE(value1 <= NumericValue<uint32_t>(1));
    ASSERT_TRUE(value1 <= NumericValue<int64_t>(2));
    ASSERT_FALSE(value1 >= NumericValue<uint64_t>(2));
}

NumericValueTestSuite(WhenNumberValuesAreCompared, shouldReturnCorrectComparisonsForAllTypes)
{
    compareTest<int8_t>();
    compareTest<uint8_t>();
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignValue, shouldAssignValue)
{
    NumericValue<uint32_t> value1(1);
    NumericValue<uint32_t> value2(2);

    value1 = value2;

    ASSERT_EQ(2, (uint32_t)value1);
    ASSERT_EQ(2, (uint32_t)value2);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignPrimativeValue, shouldAssignUint32Value)
{
    NumericValue<uint32_t> value1(1);

    ASSERT_TRUE(value1.assign((uint32_t)2));
    ASSERT_EQ(2, (uint32_t)value1);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForUint8)
{
    NumericValue<uint8_t> value(1);

    ASSERT_FALSE(value.assign(256));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-1));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForInt8)
{
    NumericValue<int8_t> value(1);

    ASSERT_FALSE(value.assign(129));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-129));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForUint16)
{
    NumericValue<uint16_t> value(1);

    ASSERT_FALSE(value.assign(65536));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-1));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForInt16)
{
    NumericValue<int16_t> value(1);

    ASSERT_FALSE(value.assign(32768));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-32769));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForUint32)
{
    NumericValue<uint32_t> value(1);

    ASSERT_FALSE(value.assign(4294967296));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-1));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForInt32)
{
    NumericValue<int32_t> value(1);

    ASSERT_FALSE(value.assign(2147483648));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign(-2147483649));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToAssignOutOfRangeValue, shouldReturnFalseForFloat)
{
    NumericValue<float> value(1);

    ASSERT_FALSE(value.assign((double)3.40282E39));
    ASSERT_EQ(1, (int64_t)value);
    ASSERT_FALSE(value.assign((double)-3.40282E39));
    ASSERT_EQ(1, (int64_t)value);
}

NumericValueTestSuite(WhenNumericValueIsAskedToSerializeToByteBuffer, shouldSerialize)
{
    zios::common::MockByteBuffer mockByteBuffer;

    EXPECT_CALL(mockByteBuffer, putU16(123));

    NumericValue<uint16_t>(123).serialize(mockByteBuffer);
}

} /* namespace zios::domain */
