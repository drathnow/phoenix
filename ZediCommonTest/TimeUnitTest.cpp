/** @file TimeUnitTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 25, 2015 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <TimeUnit.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

class WhenTimeUnitIsAskedToConvert : public testing::Test {
public:
};

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertNanoSeconds) {
    ASSERT_EQ(1000, TimeUnit::NANOSECONDS.toMillis(1000000000LL));
    ASSERT_EQ(1, TimeUnit::NANOSECONDS.toSeconds(1000000000LL));
    ASSERT_EQ(0, TimeUnit::NANOSECONDS.toMinutes(1000000000LL));
    ASSERT_EQ(0, TimeUnit::NANOSECONDS.toHours(1000000000LL));
    ASSERT_EQ(0, TimeUnit::NANOSECONDS.toDays(1000000000LL));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertMicroSeconds) {
    ASSERT_EQ(1000000LL, TimeUnit::MICROSECONDS.toMillis(1000000000LL));
    ASSERT_EQ(1000, TimeUnit::MICROSECONDS.toSeconds(1000000000LL));
    ASSERT_EQ(16, TimeUnit::MICROSECONDS.toMinutes(1000000000LL));
    ASSERT_EQ(0, TimeUnit::MICROSECONDS.toHours(1000000000LL));
    ASSERT_EQ(0, TimeUnit::MICROSECONDS.toDays(1000000000LL));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertMilliseconds) {
    ASSERT_EQ(1000000000, TimeUnit::MILLISECONDS.toMillis(1000000000LL));
    ASSERT_EQ(1000000, TimeUnit::MILLISECONDS.toSeconds(1000000000LL));
    ASSERT_EQ(16666, TimeUnit::MILLISECONDS.toMinutes(1000000000LL));
    ASSERT_EQ(277, TimeUnit::MILLISECONDS.toHours(1000000000LL));
    ASSERT_EQ(11, TimeUnit::MILLISECONDS.toDays(1000000000LL));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertSeconds) {
    ASSERT_EQ(1000, TimeUnit::SECONDS.toMillis(1));
    ASSERT_EQ(10000000, TimeUnit::SECONDS.toSeconds(10000000L));
    ASSERT_EQ(166666, TimeUnit::SECONDS.toMinutes(10000000L));
    ASSERT_EQ(2777, TimeUnit::SECONDS.toHours(10000000L));
    ASSERT_EQ(115, TimeUnit::SECONDS.toDays(10000000L));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertMinutes) {
    ASSERT_EQ(60000000, TimeUnit::MINUTES.toMillis(1000));
    ASSERT_EQ(60000, TimeUnit::MINUTES.toSeconds(1000));
    ASSERT_EQ(1000, TimeUnit::MINUTES.toMinutes(1000));
    ASSERT_EQ(16, TimeUnit::MINUTES.toHours(1000));
    ASSERT_EQ(0, TimeUnit::MINUTES.toDays(1000));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertHours) {
    ASSERT_EQ(360000000, TimeUnit::HOURS.toMillis(100));
    ASSERT_EQ(360000, TimeUnit::HOURS.toSeconds(100));
    ASSERT_EQ(6000, TimeUnit::HOURS.toMinutes(100));
    ASSERT_EQ(100, TimeUnit::HOURS.toHours(100));
    ASSERT_EQ(4, TimeUnit::HOURS.toDays(100));
}

TEST_F(WhenTimeUnitIsAskedToConvert, shouldConvertDayss) {
    ASSERT_EQ(8640000000LL, TimeUnit::DAYS.toMillis(100));
    ASSERT_EQ(8640000, TimeUnit::DAYS.toSeconds(100));
    ASSERT_EQ(144000, TimeUnit::DAYS.toMinutes(100));
    ASSERT_EQ(2400, TimeUnit::DAYS.toHours(100));
    ASSERT_EQ(100, TimeUnit::DAYS.toDays(100));
}


}
}
