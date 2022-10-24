#include <deadband.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>


namespace zios::domain
{

class PercentageDeadbandTest: public testing::Test
{
public:
    PercentageDeadbandTest() = default;
    ~PercentageDeadbandTest() = default;
};


TEST_F(PercentageDeadbandTest, shouldReturnTrueIfValueChangesByPercentage)
{
    percentage_deadband<uint32_t> deadbandUnderTest(10);

    ASSERT_FALSE(deadbandUnderTest.currentValueHasChanged(200, 190));
    ASSERT_FALSE(deadbandUnderTest.currentValueHasChanged(200, 181));
    ASSERT_FALSE(deadbandUnderTest.currentValueHasChanged(200, 210));
    ASSERT_FALSE(deadbandUnderTest.currentValueHasChanged(200, 219));

    ASSERT_TRUE(deadbandUnderTest.currentValueHasChanged(200, 175));
    ASSERT_TRUE(deadbandUnderTest.currentValueHasChanged(200, 180));
    ASSERT_TRUE(deadbandUnderTest.currentValueHasChanged(200, 220));
    ASSERT_TRUE(deadbandUnderTest.currentValueHasChanged(200, 240));
}
}
