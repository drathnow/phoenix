/** @file Lpc3250RTDTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 24, 2020 by daver:
 * @endhistory
 */
#include <Lpc3250RTD.h>
#include <gtest/gtest.h>

#define Lpc3250RTDTestSuite(test_suite_name, test_name)         \
  GTEST_TEST_(test_suite_name, test_name, Lpc3250RTDTest,  \
              ::testing::internal::GetTestTypeId())


namespace zios {
namespace lpc3250 {


class Lpc3250RTDTest : public testing::Test
{
public:
    ~Lpc3250RTDTest() {}
};

Lpc3250RTDTestSuite(WhenLpc3250RTDIsAskedForTempuratureFromResistenace, shouldCalculateTemperature)
{
    ASSERT_NEAR(0.0, Lpc3250RTD::temperatureForResistance(100.0), 0.001);
}

Lpc3250RTDTestSuite(WhenLpc3250RTDIsAskedForTempuratureFromResistenaceLessThanZero, shouldReturnError)
{
    ASSERT_EQ(9999, Lpc3250RTD::temperatureForResistance(-1.0));
}

Lpc3250RTDTestSuite(WhenLpc3250RTDIsAskedForTempuratureFromResistenaceCalculationResultInSquareRootOfNegativeNumber, shouldReturnError)
{
    ASSERT_EQ(9999, Lpc3250RTD::temperatureForResistance(13181769));
}

}
}


