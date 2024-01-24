/** @file BasePacketTest.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 31, 2018 by daver:
 * @endhistory
 */
#ifndef BASEPACKETTEST_H_
#define BASEPACKETTEST_H_

#include <ProtocolPacket.h>

namespace zios {
namespace common {


class BasePacketTest {
public:

    static int bufferLengthFromPacket(BasePacket&packet) {
        return packet._currentbufferLength;
    }

    static Region headerRangeFromPacket(BasePacket&packet) {
        return packet._header;
    }

    static Region bodyRangeFromPacket(BasePacket&packet) {
        return packet._body;
    }

    static Region trailerRangeFromPacket(BasePacket&packet) {
        return packet._trailer;
    }

};

}
}


#endif /* BASEPACKETTEST_H_ */
