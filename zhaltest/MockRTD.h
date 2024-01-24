/** @file MockRTD.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 18, 2018 by daver:
 * @endhistory
 */
#ifndef MOCKRTD_H_
#define MOCKRTD_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <zhal.h>
#include "IHardwareIO.h"

namespace zios {
namespace hal {

class MockRTD : public RTD {
public:
    MockRTD()
    {
    }

    ~MockRTD()
    {
    }

    MOCK_CONST_METHOD0(name, const char*());
    MOCK_CONST_METHOD0(label, const char*());
    MOCK_CONST_METHOD0(displayOrder, uint32_t());
    MOCK_METHOD1(updateDisplayOrder, void(uint32_t order));
    MOCK_METHOD0(getInt, uint32_t());
    MOCK_METHOD0(getFloat, float());
    MOCK_CONST_METHOD0(getResponseType, IHardwareIO::DATATYPE());
    MOCK_CONST_METHOD0(timestamp, long_timestamp_t());
    MOCK_CONST_METHOD0(isReadOnly, bool());
    MOCK_METHOD1(writeBool, bool(bool value));
    MOCK_METHOD0(convertTemperature, double());

};

}
}

#endif /* MOCKRTD_H_ */
