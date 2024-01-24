/** @file PollerTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 25, 2014 by daver:
 * @endhistory
 */

#include <gmock/gmock.h>
#include "MockEventHandler.h"

#include <poll.h>
#include <memory>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

namespace zios {
namespace common {

TEST(PollerTest, shouldCleanUp) {
    int socketFd = ::socket(AF_INET, SOCK_STREAM, 0);
    MockEventHandler handler;
    Poller* poller = Poller::open();

    PollKey* k1 = poller->registerDescriptor(socketFd, handler);
    ASSERT_TRUE(k1 != NULL);

    k1->close();
    delete poller;  // Pollkey will be cleaned up when poller is destroyed.
    ::close(socketFd);
}

}
}

