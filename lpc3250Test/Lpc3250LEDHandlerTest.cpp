/** @file Lpc3250LEDHandlerTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 22, 2019 by daver:
 * @endhistory
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <zhal.h>
#include <NotificationCenter.h>
#include <Lpc3250LEDHandler.h>

#include "MockLED.h"

namespace zios {
namespace lpc3250 {

using namespace zios::hal;
using namespace zios::common;

using testing::Return;

TEST(WhenLpc3250LEDIsAskedToHandleConnectionConnectedNotification, shouldTurnOnServerLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOn()).WillOnce(Return(1));
    EXPECT_CALL(*serverLed, turnOff()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::CONNECTION_CONNECTED_NOTIFICATION, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleConnectionListNotification, shouldTurnOffServerLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).WillOnce(Return(1));
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::CONNECTION_LOST_NOTIFICATION, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleConnectionShuttingDownNotification, shouldTurnOffServerLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).WillOnce(Return(1));
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::CONNECTION_SHUTTING_DOWN, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleTurnOnPollVoteNotification, shouldTurnOnPollLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).Times(0);
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).WillOnce(Return(1));
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::TURN_ON_POLL_VOTE_LED, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleTurnOffPollVoteNotification, shouldTurnOffPollLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).Times(0);
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).WillOnce(Return(1));

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::TURN_OFF_POLL_VOTE_LED, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleTurnOnNetworkNotification, shouldTurnOnNetworkLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).Times(0);
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).WillOnce(Return(1));
    EXPECT_CALL(*networkLed, turnOff()).Times(0);
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::TURN_ON_NETWORK_LED, NULL);
}

TEST(WhenLpc3250LEDIsAskedToHandleTurnOffNetworkNotification, shouldTurnOffNetworkLED)
{
    MockLED *serverLed = new MockLED();
    MockLED *networkLed = new MockLED();
    MockLED *pollLed = new MockLED();

    EXPECT_CALL(*serverLed, turnOff()).Times(0);
    EXPECT_CALL(*serverLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOn()).Times(0);
    EXPECT_CALL(*networkLed, turnOff()).WillOnce(Return(1));
    EXPECT_CALL(*pollLed, turnOn()).Times(0);
    EXPECT_CALL(*pollLed, turnOff()).Times(0);

    EXPECT_CALL(*serverLed, destroyMockLED());
    EXPECT_CALL(*networkLed, destroyMockLED());
    EXPECT_CALL(*pollLed, destroyMockLED());

    Lpc3250LEDHandler ledHandler(*networkLed, *serverLed, *pollLed);
    ledHandler.handleNotification(NotificationDescriptions::TURN_OFF_NETWORK_LED, NULL);
}

}
}
