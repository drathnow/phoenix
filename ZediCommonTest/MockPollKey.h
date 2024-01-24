/** @file MockPollKey.h 
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
#ifndef MOCKPOLLKEY_H_
#define MOCKPOLLKEY_H_

#include <gmock/gmock.h>
#include <io.h>

namespace zios {
namespace common {

class MockPollKey : public PollKey {
public:
    MockPollKey() : PollKey(NULL, NULL, 0) {}
    ~MockPollKey() { destroyMockPollKey(); }

    MOCK_METHOD0(destroyMockPollKey, void());
    MOCK_METHOD1(changeInterestEvents, int(uint32_t opts));
    MOCK_CONST_METHOD0(isReadable, bool());
    MOCK_CONST_METHOD0(isWritable, bool());
    MOCK_CONST_METHOD0(hasError, bool());
    MOCK_CONST_METHOD0(isClosed, bool());
    MOCK_CONST_METHOD0(isValid, bool());
    MOCK_METHOD0(close, void());
    MOCK_METHOD1(setEventHandler, void(EventHandler& eventHandler));
    MOCK_METHOD0(eventHandler,EventHandler*());
};

}
}




#endif /* MOCKPOLLKEY_H_ */
