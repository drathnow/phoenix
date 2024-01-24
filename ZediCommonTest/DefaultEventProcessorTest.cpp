/** @file DefaultEventProcessorTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 26, 2014 by daver:
 * @endhistory
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ctime>
#include <io.h>

#include "MockPoller.h"
#include "MockPollKey.h"
#include "MockEventHandler.h"

namespace zios {
namespace common {

using ::testing::Return;
using ::testing::_;

class DefaultEventProcessorTest : public ::testing::Test {
public:

};
TEST_F(DefaultEventProcessorTest, shouldReturnErrorIfPollHasError) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1)).WillOnce(Return(-1));
    EXPECT_CALL(poller, readyKeys()).Times(0);
    EXPECT_CALL(pollKey, isValid()).Times(0);
    EXPECT_CALL(pollKey, isReadable()).Times(0);
    EXPECT_CALL(pollKey, isWritable()).Times(0);
    EXPECT_CALL(pollKey, hasError()).Times(0);
    EXPECT_CALL(pollKey, eventHandler()).Times(0);
    EXPECT_CALL(eventHandler, handleRead()).Times(0);
    EXPECT_CALL(eventHandler, handleWrite()).Times(0);
    EXPECT_CALL(eventHandler, handleError()).Times(0);

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(-1, eventProcessor.processEvents());
}

TEST_F(DefaultEventProcessorTest, shouldProcessReadButNotWriteEventIfKeyIsInvalidAfterRead) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;
    PollKey* readyKeys[1] = {&pollKey};

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1))
        .WillOnce(Return(1));
    EXPECT_CALL(poller, readyKeys()).WillOnce(Return(readyKeys));
    EXPECT_CALL(pollKey, isValid())
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(pollKey, isReadable()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, isWritable()).Times(0);
    EXPECT_CALL(pollKey, hasError()).Times(0);
    EXPECT_CALL(pollKey, eventHandler()).WillRepeatedly(Return(&eventHandler));
    EXPECT_CALL(eventHandler, handleRead());
    EXPECT_CALL(eventHandler, handleWrite()).Times(0);
    EXPECT_CALL(eventHandler, handleError()).Times(0);

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(1, eventProcessor.processEvents());
}

TEST_F(DefaultEventProcessorTest, shouldProcessReadAndWriteEvent) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;
    PollKey* readyKeys[1] = {&pollKey};

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1))
        .WillOnce(Return(1));
    EXPECT_CALL(poller, readyKeys()).WillOnce(Return(readyKeys));
    EXPECT_CALL(pollKey, isValid()).WillRepeatedly(Return(true));
    EXPECT_CALL(pollKey, isReadable()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, isWritable()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, hasError()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, eventHandler()).WillRepeatedly(Return(&eventHandler));
    EXPECT_CALL(eventHandler, handleRead());
    EXPECT_CALL(eventHandler, handleWrite());
    EXPECT_CALL(eventHandler, handleError()).Times(0);

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(1, eventProcessor.processEvents());
}

TEST_F(DefaultEventProcessorTest, shouldProcessErrorEvent) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;
    PollKey* readyKeys[1] = {&pollKey};

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1))
        .WillOnce(Return(1));
    EXPECT_CALL(poller, readyKeys()).WillOnce(Return(readyKeys));
    EXPECT_CALL(pollKey, isValid()).WillRepeatedly(Return(true));
    EXPECT_CALL(pollKey, isReadable()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, isWritable()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, hasError()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, eventHandler()).WillOnce(Return(&eventHandler));
    EXPECT_CALL(eventHandler, handleRead()).Times(0);
    EXPECT_CALL(eventHandler, handleWrite()).Times(0);
    EXPECT_CALL(eventHandler, handleError());

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(1, eventProcessor.processEvents());
}

TEST_F(DefaultEventProcessorTest, shouldProcessWriteEvent) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;
    PollKey* readyKeys[1] = {&pollKey};

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1))
        .WillOnce(Return(1));
    EXPECT_CALL(poller, readyKeys()).WillOnce(Return(readyKeys));
    EXPECT_CALL(pollKey, isValid()).WillRepeatedly(Return(true));
    EXPECT_CALL(pollKey, isReadable()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, isWritable()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, hasError()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, eventHandler()).WillOnce(Return(&eventHandler));
    EXPECT_CALL(eventHandler, handleRead()).Times(0);
    EXPECT_CALL(eventHandler, handleWrite());
    EXPECT_CALL(eventHandler, handleError()).Times(0);

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(1, eventProcessor.processEvents());
}

TEST_F(DefaultEventProcessorTest, shouldProcessReadEvent) {
    MockPoller poller;
    MockPollKey pollKey;
    MockEventHandler eventHandler;
    PollKey* readyKeys[1] = {&pollKey};

    EXPECT_CALL(pollKey, destroyMockPollKey());

    EXPECT_CALL(poller, poll(-1))
        .WillOnce(Return(1));
    EXPECT_CALL(poller, readyKeys()).WillOnce(Return(readyKeys));
    EXPECT_CALL(pollKey, isValid()).WillRepeatedly(Return(true));
    EXPECT_CALL(pollKey, isReadable()).WillOnce(Return(true));
    EXPECT_CALL(pollKey, isWritable()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, hasError()).WillOnce(Return(false));
    EXPECT_CALL(pollKey, eventHandler()).WillOnce(Return(&eventHandler));
    EXPECT_CALL(eventHandler, handleRead());
    EXPECT_CALL(eventHandler, handleWrite()).Times(0);
    EXPECT_CALL(eventHandler, handleError()).Times(0);

    DefaultEventProcessor eventProcessor(&poller);
    ASSERT_EQ(1, eventProcessor.processEvents());
}

}
}
