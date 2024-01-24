/** @file NotificationCenterTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 9, 2013 by daver:
 * IncrDev Apr 03, 2014 by eb:  removed extraneous log4cplus init
 * @endhistory
 */
#include <iostream>
#include <log4cplus/configurator.h>

#include "MockNotification.h"
#include "MockNotificationHandler.h"
#include "gtest/gtest.h"
#include "NotificationCenter.h"
#include "NotificationHandler.h"

namespace zios {
namespace common {

using namespace zios::common;
using namespace std;

using ::testing::_;
using ::testing::Return;

const std::string NOTIFICATION_NAME("Hello World");
const std::string OTHER_NOTIFICATION_NAME("Spooge");

class NotificationCenterTest: public testing::Test {
protected:
};

class MyHandler : public NotificationHandler {
public:
	MyHandler() : _handlerWasCalled(false) {}
	~MyHandler() {}

	void handleNotification(const string& notificationName, Notification* notification) {
		_handlerWasCalled = true;
	}

	bool _handlerWasCalled;
};

TEST_F(NotificationCenterTest, shouldRegisterListenerAndHandleNotification) {
	NotificationCenter center;
	MyHandler handler;
	Notification notification("foo");

	center.registerHandler(&handler, NOTIFICATION_NAME);
	center.postNotification(NOTIFICATION_NAME, &notification);
	center.removeHandler(&handler, NOTIFICATION_NAME);

	ASSERT_TRUE(handler._handlerWasCalled);

	handler._handlerWasCalled = false;
	center.postNotification(NOTIFICATION_NAME, &notification);
	ASSERT_FALSE(handler._handlerWasCalled);
}

class TestableNotificationCenter : public NotificationCenter {
public:
    TestableNotificationCenter() {}
    ~TestableNotificationCenter() {}

    std::map<std::string, std::set<NotificationHandler*>*>& handlerMap() {return _handlerMap; }
};

class WhenNotificationCenterIsAskedToRegisterHandler : public testing::Test {
public:
    ~WhenNotificationCenterIsAskedToRegisterHandler() {}
};

TEST_F(WhenNotificationCenterIsAskedToRegisterHandler, shouldAddHandlerToMap) {
    MockNotificationHandler handler;

    TestableNotificationCenter notificationCenter;
    notificationCenter.registerHandler(&handler, NOTIFICATION_NAME);

    ASSERT_EQ(1, notificationCenter.handlerMap().size());
    std::set<NotificationHandler*>* set = notificationCenter.handlerMap()[NOTIFICATION_NAME];
    ASSERT_EQ(1, set->count(&handler));
}

class WhenNotificationCenterIsAskedToRemoveHandler : public testing::Test {
public:
    ~WhenNotificationCenterIsAskedToRemoveHandler() {}
};

TEST_F(WhenNotificationCenterIsAskedToRemoveHandler, shouldRemoveHandlerForNotificationNameFromMapWhenOnlyOneHandlerRegistered) {
    MockNotificationHandler handler;

    TestableNotificationCenter notificationCenter;
    notificationCenter.registerHandler(&handler, NOTIFICATION_NAME);

    ASSERT_EQ(1, notificationCenter.handlerMap().size());
    std::set<NotificationHandler*>* set = notificationCenter.handlerMap()[NOTIFICATION_NAME];
    ASSERT_EQ(1, set->count(&handler));

    notificationCenter.removeHandler(&handler, NOTIFICATION_NAME);

    ASSERT_EQ(0, notificationCenter.handlerMap().size());
}

TEST_F(WhenNotificationCenterIsAskedToRemoveHandler, shouldRemoveHandlerForNotificationNameFromMapWhenMoreThanOneHandlerRegistered) {
    MockNotificationHandler handler1;
    MockNotificationHandler handler2;

    TestableNotificationCenter notificationCenter;
    notificationCenter.registerHandler(&handler1, NOTIFICATION_NAME);
    notificationCenter.registerHandler(&handler2, NOTIFICATION_NAME);

    ASSERT_EQ(1, notificationCenter.handlerMap().size());
    std::set<NotificationHandler*>* set = notificationCenter.handlerMap()[NOTIFICATION_NAME];
    ASSERT_EQ(2, set->size());

    notificationCenter.removeHandler(&handler1, NOTIFICATION_NAME);

    ASSERT_EQ(1, notificationCenter.handlerMap().size());
    set = notificationCenter.handlerMap()[NOTIFICATION_NAME];
    ASSERT_EQ(1, set->count(&handler2));
}

class WhenNotificationCenterIsAskedToPostNotificaiton: public testing::Test {
public:
    ~WhenNotificationCenterIsAskedToPostNotificaiton() {}
};

TEST_F(WhenNotificationCenterIsAskedToPostNotificaiton, shouldPostNotificationToAllHandlers) {
    MockNotificationHandler handler1;
    MockNotificationHandler handler2;
    MockNotificationHandler handler3;

    EXPECT_CALL(handler1, handleNotification(NOTIFICATION_NAME, NULL));
    EXPECT_CALL(handler2, handleNotification(NOTIFICATION_NAME, NULL));
    EXPECT_CALL(handler3, handleNotification(_, NULL)).Times(0);

    TestableNotificationCenter notificationCenter;
    notificationCenter.registerHandler(&handler1, NOTIFICATION_NAME);
    notificationCenter.registerHandler(&handler2, NOTIFICATION_NAME);
    notificationCenter.registerHandler(&handler3, OTHER_NOTIFICATION_NAME);

    notificationCenter.postNotification(NOTIFICATION_NAME);
}

}
}
