/** @file SystemTimeAdjusterTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 27, 2015 by daver:
 * IncrDev May 27, 2015 by eb: changed naming on notification as part of unification
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include <cstdlib>
#include <string>
#include <map>
#include "MockSystemTime.h"
#include "MockSystemTimeService.h"
#include <SystemTimeAdjuster.h>
#include "MockNotificationCenter.h"
#include <Utl.h>
#include <NotificationCenter.h>
#include <Notification.h>

namespace zios {
namespace common_tests {

using namespace zios::common;
using namespace std;

using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::MakeMatcher;
using ::testing::Matcher;
using testing::Mock;
using testing::Return;
using testing::_;

static const int32_t ROUND_TRIP_THRESHOLD = 10; // Seconds

class SystemTimeAdjusterTest : public testing::Test {
public:
    SystemTimeAdjusterTest() {}
    ~SystemTimeAdjusterTest() {}
};

class NotificationMatcher : public MatcherInterface<Notification*> {
public:
    explicit NotificationMatcher(Notification* notification) :
        _notification(notification) {
    }

    virtual bool MatchAndExplain(Notification* notification, MatchResultListener* listener) const {
        if (notification->name() != _notification->name())
            return false;
        if (notification->properties().size() == _notification->properties().size())
            for (std::map<string, string>::const_iterator iter = notification->properties().begin();
                        iter != notification->properties().end();
                            ++iter) {
                std::string key = iter->first;
                std::map<string, string>::const_iterator otherIter = _notification->properties().find(key);
                if (otherIter == _notification->properties().end())
                    return false;
                if (iter->second != otherIter->second)
                    return false;
            }
        return true;
    }

    virtual void DescribeTo(::std::ostream* os) const {
        *os << "Expected the right timval but it was wrong";
    }

    virtual void DescribeNegationTo(::std::ostream* os) const {
        *os << "Expected the right timval but it was wrong";
    }

private:
    Notification* _notification;
};

inline Matcher<Notification*> notificationMatching(Notification* notification) {
    return MakeMatcher(new NotificationMatcher(notification));
}

TEST_F(SystemTimeAdjusterTest, shouldNotSetTimeWhenRoundTripTimeExceedsRoundTripThreshold) {
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    int32_t adjustTimeThreshold = 15; // Seconds
    int32_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "00:49:59";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);
    time_t currentTime = deviceTime + ROUND_TRIP_THRESHOLD + 1;

    char buf[20];
    int32_t diffTime = serverTime-deviceTime;
    ::sprintf(buf, "%d", (int)diffTime);
    std::string param(buf);
    Notification expectedNotifiction(NotificationDescriptions::TIME_CHANGED_NOTIFICATION);
    expectedNotifiction.addProperty(NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM, param);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(currentTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(serverTime)).Times(0);
    EXPECT_CALL(notificationCenter, postNotification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION,
            notificationMatching(&expectedNotifiction))).Times(0);

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(1, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldCleanUp) {
    MockSystemTime* systemTime = new MockSystemTime();
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockNotificationCenter notificationCenter;
    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    {
        SystemTimeAdjuster adjuster(0, ROUND_TRIP_THRESHOLD, 0, systemTimeService, &notificationCenter, systemTime);
    }
    Mock::VerifyAndClearExpectations(systemTime);
    Mock::VerifyAndClearExpectations(systemTimeService);
}


TEST_F(SystemTimeAdjusterTest, shouldAdjustServerTimeByHalfRoundTripTime) {
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    int32_t adjustTimeThreshold = 15; // Seconds
    int32_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "00:49:59";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);
    time_t currentTime = deviceTime + 2;
    time_t expectedAdjustement = serverTime + 1; // Half the rountrip time.

    char buf[20];
    int32_t diffTime = expectedAdjustement-deviceTime-2;
    ::sprintf(buf, "%d", (int)diffTime);
    std::string param(buf);
    Notification expectedNotifiction(NotificationDescriptions::TIME_CHANGED_NOTIFICATION);
    expectedNotifiction.addProperty(NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM, param);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(currentTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(expectedAdjustement)).WillOnce(Return(0));
    EXPECT_CALL(notificationCenter, postNotification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION,
            notificationMatching(&expectedNotifiction)));

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldUpdateTimeIfDeltaIsGTAdjustTimeThresholdAndGEMaxTimeThresholdBackward) {
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    int32_t adjustTimeThreshold = 15; // Seconds
    int32_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "00:49:59";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);

    char buf[20];
    int32_t diffTime = serverTime-deviceTime;
    ::sprintf(buf, "%d", (int)diffTime);
    std::string param(buf);
    Notification expectedNotifiction(NotificationDescriptions::TIME_CHANGED_NOTIFICATION);
    expectedNotifiction.addProperty(NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM, param);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(serverTime)).WillOnce(Return(0));
    EXPECT_CALL(notificationCenter, postNotification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION,
            notificationMatching(&expectedNotifiction)));

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldUpdateTimeIfDeltaIsGTAdjustTimeThresholdAndGEMaxTimeThresholdForward) {
    MockSystemTime* systemTime = new MockSystemTime();
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockNotificationCenter notificationCenter;
    int32_t adjustTimeThreshold = 15; // Seconds
    int32_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "01:10:01";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);

    char buf[20];
    int32_t diffTime = serverTime-deviceTime;
    ::sprintf(buf, "%d", (int)diffTime);
    std::string param(buf);
    Notification expectedNotifiction(NotificationDescriptions::TIME_CHANGED_NOTIFICATION);
    expectedNotifiction.addProperty(NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM, param);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(serverTime)).WillOnce(Return(0));
    EXPECT_CALL(notificationCenter, postNotification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION,
            notificationMatching(&expectedNotifiction)));

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldAdjustTimeIfDeltaIsEQAdjustTimeThresholdAndLTMaxTimeThresholdBackward) {
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    time_t adjustTimeThreshold = 15; // Seconds
    time_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "01:09:59";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);
    time_t expectedAdjustement = serverTime - deviceTime;

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(expectedAdjustement)).WillOnce(Return(0));

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldAdjustTimeIfDeltaIsEQAdjustTimeThresholdAndLTMaxTimeThresholdForward) {
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    time_t adjustTimeThreshold = 15; // Seconds
    time_t maxTimeThreshold = 600;   // 10 min.
    const char* serverTimeStr = "01:09:59";
    const char* deviceTimeStr = "01:00:00";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);
    time_t expectedAdjustement = serverTime - deviceTime;

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock()).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, adjustTime(expectedAdjustement)).WillOnce(Return(0));

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldNotAdjustTimeIfDeltaIsLessThanAdjustTimeThresholdBackward) {
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    time_t adjustTimeThreshold = 15; // Seconds
    time_t maxTimeThreshold = 600;
    const char* serverTimeStr = "01:00:00";
    const char* deviceTimeStr = "01:00:14";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock());
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(_)).Times(0);

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

TEST_F(SystemTimeAdjusterTest, shouldNotAdjustTimeIfDeltaIsLessThanAdjustTimeThresholdForward) {
    MockSystemTimeService* systemTimeService = new MockSystemTimeService();
    MockSystemTime* systemTime = new MockSystemTime();
    MockNotificationCenter notificationCenter;
    time_t adjustTimeThreshold = 15; // Seconds
    time_t maxTimeThreshold = 600;
    const char* serverTimeStr = "01:00:14";
    const char* deviceTimeStr = "01:00:00";
    time_t serverTime = AbsoluteTime::absoluteTimeForString(serverTimeStr);
    time_t deviceTime = AbsoluteTime::absoluteTimeForString(deviceTimeStr);

    EXPECT_CALL(*systemTime, destroyMockSystemTime());
    EXPECT_CALL(*systemTimeService, destroyMockSystemTimeService());
    EXPECT_CALL(*systemTime, currentTime()).WillOnce(Return(deviceTime));
    EXPECT_CALL(*systemTimeService, updateSystemTimeToHardwareClock());
    EXPECT_CALL(*systemTimeService, adjustTime(_)).Times(0);
    EXPECT_CALL(*systemTimeService, updateTime(_)).Times(0);

    SystemTimeAdjuster timeAdjuster(adjustTimeThreshold, maxTimeThreshold, ROUND_TRIP_THRESHOLD, systemTimeService, &notificationCenter, systemTime);
    ASSERT_EQ(0, timeAdjuster.adjustTimeWithServerTime(deviceTime, serverTime));
}

}
}
