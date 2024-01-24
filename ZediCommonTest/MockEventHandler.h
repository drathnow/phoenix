/** @file MockEventHandler.h 
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
#ifndef MOCKEVENTHANDLER_H_
#define MOCKEVENTHANDLER_H_

#include <gmock/gmock.h>
#include <io.h>

namespace zios {
namespace common {

class MockEventHandler : public EventHandler {
public:
    ~MockEventHandler() {}

    MOCK_METHOD0(handleRead, int());
    MOCK_METHOD0(handleWrite, int());
    MOCK_METHOD0(handleError, int());
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* MOCKEVENTHANDLER_H_ */
