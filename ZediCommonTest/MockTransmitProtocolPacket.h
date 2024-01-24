/** @file MockTransmitProtocolPacket.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 20, 2018 by daver:
 * @endhistory
 */
#ifndef MOCKTRANSMITPROTOCOLPACKET_H_
#define MOCKTRANSMITPROTOCOLPACKET_H_

#include <gmock/gmock.h>
#include <ProtocolPacket.h>

namespace zios {
namespace common {

class MockTransmitProtocolPacket : public zios::common::TransmitProtocolPacket {
public:
    MockTransmitProtocolPacket() : TransmitProtocolPacket(0, 0, 0) {}
    virtual ~MockTransmitProtocolPacket() { destroyMockTransmitProtocolPacket(); }

    MOCK_METHOD0(destroyMockTransmitProtocolPacket, void());
    MOCK_METHOD1(addHeader, void(uint32_t length));
    MOCK_METHOD1(addTrailer, void(uint32_t length));
    MOCK_METHOD1(setBodyLength, void(uint32_t length));
    MOCK_CONST_METHOD0(header, uint8_t*());
    MOCK_CONST_METHOD0(headerLength, uint32_t());
    MOCK_CONST_METHOD0(body, uint8_t*());
    MOCK_CONST_METHOD0(bodyLength, uint32_t());
    MOCK_CONST_METHOD0(trailer, uint8_t*());
    MOCK_CONST_METHOD0(trailerLength, uint32_t());
    MOCK_METHOD0(merge, void());
    MOCK_METHOD1(reset, void(bool zeroBuffer));
    MOCK_METHOD1(resizeBody, bool(int toSize));

    bool operator==(const MockTransmitProtocolPacket& otherPacket) const {
        return this == &otherPacket;
    }

};

}
}



#endif /* MOCKTRANSMITPROTOCOLPACKET_H_ */
