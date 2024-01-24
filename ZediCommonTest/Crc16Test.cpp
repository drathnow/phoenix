/** @file Crc16Test.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 30, 2016 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include <Crc16.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

TEST(WhenCrc16IsAskedToCalculateRunningBytes, shouldReturnSameCalculation) {
    const uint8_t bytes[] = {0x11, 0x03, 0x00, 0x6B, 0x00, 0x03};
    uint16_t expectedCrc = 0x8776;
    ASSERT_EQ(expectedCrc, Crc16::calculate(bytes, sizeof(bytes)));
}

TEST(WhenCrc16IsAskedToCalculateRunningBytes1, shouldReturnSameCalculation) {
    const uint8_t bytes[] = {0x01, 0x03, 0x1b, 0x5d, 0x00, 0x01};
    uint16_t expectedCrc = 0x3c13;
    ASSERT_EQ(expectedCrc, Crc16::calculate(bytes, sizeof(bytes)));
}

}
}
