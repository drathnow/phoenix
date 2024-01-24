/** @file MockNotification.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 1, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKNOTIFICATION_H_
#define MOCKNOTIFICATION_H_
#include <map>
#include <string>
#include <gmock/gmock.h>

#include <Notification.h>

namespace zios {
namespace common {


class MockNotification : public zios::common::Notification {
public:
    MockNotification() : Notification("foo") {}
    ~MockNotification() {}

    MOCK_METHOD2(addProperty, void(const std::string& propertyName, const std::string& value));
    MOCK_METHOD1(valueForPropertyName, const std::string*(const std::string& propertyName));
    MOCK_CONST_METHOD0(tag, const void*());
    MOCK_CONST_METHOD0(name, const std::string&());
};


}  // namespace common
}  // namespace zios

#endif /* MOCKNOTIFICATION_H_ */
