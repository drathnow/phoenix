/** @file MockTransceiver.h
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 20, 2015 by eganb:
 * @endhistory
 */
#ifndef MOCKTRANSCEIVER_H_
#define MOCKTRANSCEIVER_H_

#include <stdint.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <zhal.h>

namespace zios {
namespace hal {

class MockTransceiver: public ITransceiver {
public:
    MockTransceiver(){}
    ~MockTransceiver(){}

    MOCK_METHOD1(setDefaultMode, void(ITransceiver::Mode mode));
    MOCK_METHOD0(getDefaultMode, ITransceiver::Mode());
    MOCK_METHOD1(setTxMode, void(bool on));
    MOCK_METHOD0(off, void());

};

}
}


#endif /* MOCKTRANSCEIVER_H_ */
