#include <Deadband.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>

namespace dios::domain
{

class AbsoluteDeadbandTest: public testing::Test
{
public:
    AbsoluteDeadbandTest() = default;
    ~AbsoluteDeadbandTest() = default;
};


TEST_F(AbsoluteDeadbandTest, shouldReturnTrueWhenValueIsOutsideOfDeadband)
{
    AbsoluteDeadband<uint32_t> absoluteDeadband(5);

    ASSERT_FALSE(absoluteDeadband.currentValueHasChanged(20, 16));
    ASSERT_FALSE(absoluteDeadband.currentValueHasChanged(20, 24));

    ASSERT_TRUE(absoluteDeadband.currentValueHasChanged(20, 25));
    ASSERT_TRUE(absoluteDeadband.currentValueHasChanged(20, 26));
    ASSERT_TRUE(absoluteDeadband.currentValueHasChanged(20, 15));
    ASSERT_TRUE(absoluteDeadband.currentValueHasChanged(20, 14));
}

}
