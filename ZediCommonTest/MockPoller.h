/** @file MockPoller.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 23, 2019 by daver:
 * @endhistory
 */
#ifndef MOCKPOLLER_H_
#define MOCKPOLLER_H_

#include <gmock/gmock.h>
#include <io.h>

namespace zios {
namespace common {

class MockPoller : public Poller {
public:
    MockPoller() {}
    ~MockPoller() {}

    MOCK_METHOD1(poll, int(int timeoutMillesecons));
    MOCK_METHOD3(registerDescriptor, PollKey* (int fd, EventHandler& eventHandler, int events));
    MOCK_METHOD1(closePollKey, void(PollKey* pollKey));

    MOCK_METHOD2(forgetDescriptor, int(int fd, struct epoll_event* event));
    MOCK_METHOD2(changeInterestEvents, int(int fd, struct epoll_event*));
    MOCK_METHOD0(readyKeys, PollKey**());

};

}
}

#endif /* MOCKPOLLER_H_ */
