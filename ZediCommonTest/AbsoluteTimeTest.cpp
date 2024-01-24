/** @file AbsoluteTimeTest.cpp
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
#include <ctime>

#include "EventSchedule.h"

namespace zios {
namespace common_tests {

using namespace zios::common;

class AbsoluteTimeTest : public testing::Test {
public:
    ~AbsoluteTimeTest() {}
};

TEST_F(AbsoluteTimeTest, foo) {
    time_t now = 1439229211L;
    struct tm *localTm = ::localtime(&now);
    char fmtBuf[100];
    ASSERT_TRUE(::strftime(fmtBuf, sizeof(fmtBuf), "%FT%X", localTm) > 0);
}

TEST_F(AbsoluteTimeTest, shouldParseTime) {
    time_t now = ::time(NULL);
    const char* oneOclock = "01:00:00";
    struct tm *localTm = ::localtime(&now);
    if (localTm == NULL)
        FAIL();
    struct tm tm = *localTm;
    tm.tm_year = 70;
    tm.tm_mon = 0;
    tm.tm_yday = 0;
    tm.tm_wday = 0;
    tm.tm_mday = 1;
    tm.tm_isdst = -1;
    ASSERT_TRUE(::strptime(oneOclock, "%X", &tm) != NULL);
    time_t expectedTime = ::mktime(&tm);
    time_t result = AbsoluteTime::absoluteTimeForString(oneOclock);
    ASSERT_EQ(expectedTime, result);

    struct tm* tmPtr = ::localtime(&result);
    if (tmPtr == NULL)
        FAIL();
    ASSERT_EQ(tm.tm_hour, tmPtr->tm_hour);
    char fmtBuf[100];
    ASSERT_TRUE(::strftime(fmtBuf, sizeof(fmtBuf), "%FT%X", tmPtr) > 0);
    ASSERT_STREQ("1970-01-01T01:00:00", fmtBuf);
}

TEST_F(AbsoluteTimeTest, shouldParseDateAndTime) {
    const char* threeOclock = "2015-03-20T03:00:00";
    time_t now = ::time(NULL);
    struct tm tm;
    struct tm *localTm = ::localtime(&now);
    if (localTm == NULL)
        FAIL();
    tm = *localTm;
    ASSERT_TRUE(::strptime(threeOclock, "%FT%X", &tm) != NULL);

    tm.tm_zone = localTm->tm_zone;
    tm.tm_isdst = 1;
    time_t expectedTime = ::mktime(&tm);

    time_t result = AbsoluteTime::absoluteTimeForString(threeOclock);
    ASSERT_EQ(expectedTime, result);

    struct tm* tmPtr = ::localtime(&result);
    if (tmPtr == NULL)
        FAIL();
    size_t size = ::strlen(threeOclock)+1;
    char fmtBuf[size];
    ::strftime(fmtBuf, size, "%FT%X", tmPtr);
    ASSERT_STREQ(threeOclock, fmtBuf);
}

TEST_F(AbsoluteTimeTest, shouldParseDateOnly) {
    const char* expectedTimeStr = "2015-03-20T00:00:00";
    const char* dateString = "2015-03-20";
    time_t now = ::time(NULL);
    struct tm tm;
    struct tm *localTm = ::localtime(&now);
    if (localTm == NULL)
        FAIL();
    tm = *localTm;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    ASSERT_TRUE(::strptime(dateString, "%F", &tm) != NULL);

    tm.tm_zone = localTm->tm_zone;
    tm.tm_isdst = 1;
    time_t expectedTime = ::mktime(&tm);

    time_t result = AbsoluteTime::absoluteTimeForString(dateString);
    ASSERT_EQ(expectedTime, result);

    struct tm* tmPtr = ::localtime(&result);
    if (tmPtr == NULL)
        FAIL();
    size_t size = ::strlen(expectedTimeStr)+1;
    char fmtBuf[size];
    ::strftime(fmtBuf, size, "%FT%X", tmPtr);
    ASSERT_STREQ(expectedTimeStr, fmtBuf);
}

TEST_F(AbsoluteTimeTest, shouldParseTimeOnly) {
    time_t now = ::time(NULL);
    const char* threeOclock = "03:00:00";
    struct tm *localTm = ::localtime(&now);
    if (localTm == NULL)
        FAIL();
    struct tm tm = *localTm;
    tm.tm_year = 70;
    tm.tm_mon = 0;
    tm.tm_yday = 0;
    tm.tm_wday = 0;
    tm.tm_mday = 1;
    tm.tm_isdst = -1;
    ASSERT_TRUE(::strptime(threeOclock, "%X", &tm) != NULL);
    time_t expectedTime = ::mktime(&tm);
    time_t result = AbsoluteTime::absoluteTimeForString(threeOclock);
    ASSERT_EQ(expectedTime, result);
}

}
}
