/** @file MockNotificationHandler.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 22, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKNOTIFICATIONHANDLER_H_
#define MOCKNOTIFICATIONHANDLER_H_
#include <gmock/gmock.h>
#include <string>
#include <Notification.h>
#include <NotificationHandler.h>


class MockNotificationHandler : public zios::common::NotificationHandler {
public:
    ~MockNotificationHandler() {}

    MOCK_METHOD2(handleNotification, void(const std::string& notificationName, zios::common::Notification* notification));
};

#endif /* MOCKNOTIFICATIONHANDLER_H_ */
