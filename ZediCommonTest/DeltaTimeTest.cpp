/** @file DeletaTimeTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 20, 2015 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "EventSchedule.h"

namespace zios {
namespace common_tests {

using namespace zios::common;

class DeltaTimeTest : public testing::Test {
public:
    ~DeltaTimeTest() {}
};

TEST_F(DeltaTimeTest, shouldParseSomeDuration) {
    const char* someDuration = "1 01:20:00";
    uint32_t seconds = DeltaTime::secondsForDeltaTimeString(someDuration);
    ASSERT_EQ(91200, seconds);
}

TEST_F(DeltaTimeTest, shouldParseOneDayDuration) {
    const char* oneHourDuration = "1 00:00:00";
    uint32_t seconds = DeltaTime::secondsForDeltaTimeString(oneHourDuration);
    ASSERT_EQ(3600*24, seconds);
}

TEST_F(DeltaTimeTest, shouldParseOneDayDuration2) {
    const char* oneHourDuration = "1   00:00:00";
    uint32_t seconds = DeltaTime::secondsForDeltaTimeString(oneHourDuration);
    ASSERT_EQ(3600*24, seconds);
}

TEST_F(DeltaTimeTest, shouldParseOneHourDuration) {
    const char* oneHourDuration = "01:00:00";
    uint32_t seconds = DeltaTime::secondsForDeltaTimeString(oneHourDuration);
    ASSERT_EQ(3600, seconds);
}

TEST_F(DeltaTimeTest, shouldParse60SecondDuration) {
    const char* oneHourDuration = "00:00:60";
    uint32_t seconds = DeltaTime::secondsForDeltaTimeString(oneHourDuration);
    ASSERT_EQ(60, seconds);
}

class WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString : public DeltaTimeTest {
public:
};

TEST_F(WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString, shouldConvertIntWithDaysHoursMinAndSeconds) {
    ASSERT_STREQ("1 01:20:00", DeltaTime::deltaTimeStringForSeconds(91200).c_str());
}

TEST_F(WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString, shouldConvertIntWithDays) {
    ASSERT_STREQ("1 00:00:00", DeltaTime::deltaTimeStringForSeconds(3600*24).c_str());
}

TEST_F(WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString, shouldConvertIntWithHours) {
    ASSERT_STREQ("01:00:00", DeltaTime::deltaTimeStringForSeconds(3600).c_str());
}

TEST_F(WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString, shouldConvertIntWithMinutes) {
    ASSERT_STREQ("00:01:00", DeltaTime::deltaTimeStringForSeconds(60).c_str());
}

TEST_F(WhenDeltaTimeIsAskedToConvertIntToDeltaTimeString, shouldConvertIntWithSeconds) {
    ASSERT_STREQ("00:00:34", DeltaTime::deltaTimeStringForSeconds(34).c_str());
}

}
}


