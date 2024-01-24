/** @file MockHardwareIO.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 11, 2017 by daver:
 * @endhistory
 */
#ifndef MOCKHARDWAREIO_H_
#define MOCKHARDWAREIO_H_

#include <gmock/gmock.h>
#include <zhal.h>
#include <stdint.h>

using namespace zios::hal;

namespace zios {
namespace hal {

class MockHardwareIO : public IHardwareIO {
public:
    MockHardwareIO(IHardwareIO::PORTTYPE portType) : IHardwareIO(portType) {}
    MockHardwareIO() : IHardwareIO(IHardwareIO::AI_T) {}
    ~MockHardwareIO() {}

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
};

}
}

#endif /* MOCKHARDWAREIO_H_ */
