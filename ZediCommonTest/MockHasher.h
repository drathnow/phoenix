/** @file MockHasher.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 20, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKHASHER_H_
#define MOCKHASHER_H_
#include <gmock/gmock.h>
#include <Hasher.h>

namespace zios {
namespace common {

class MockHasher : public zios::common::Hasher {
public:
    MockHasher() {}
    ~MockHasher() { destroyMockHasher(); }

    MOCK_METHOD0(destroyMockHasher, void());
    MOCK_METHOD0(reset, void());
    MOCK_METHOD2(update, void(const uint8_t* value, uint32_t length));
    MOCK_METHOD2(hashedValue, uint32_t(uint8_t* outputBuffer, uint32_t bufferLength));
};

}
}

#endif /* MOCKHASHER_H_ */
