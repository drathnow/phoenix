/** @file MockNotificationCenter.h
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 28, 2015 by eganb:
 * @endhistory
 */
#ifndef MOCKNOTIFICATIONCENTER_H_
#define MOCKNOTIFICATIONCENTER_H_


#include <stdint.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <NotificationCenter.h>

namespace zios {
namespace common {

class MockNotificationCenter : public zios::common::NotificationCenter {
public:
    MockNotificationCenter(){}
    virtual ~MockNotificationCenter(){}

    MOCK_METHOD2(postNotification, void(const std::string& notificationName,  zios::common::Notification* notification));
    MOCK_METHOD2(registerHandler, void(zios::common::NotificationHandler* handler, const std::string& notificationName));
    MOCK_METHOD2(removeHandler, void(zios::common::NotificationHandler* handler, const std::string& notificationName));

    bool operator==(const MockNotificationCenter& otherCenter) const {
        return this == &otherCenter;
    }
};

}
}

#endif /* MOCKNOTIFICATIONCENTER_H_ */
