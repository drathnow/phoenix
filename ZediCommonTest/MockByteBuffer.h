/** @file MockByteBuffer.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 2, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKBYTEBUFFER_H_
#define MOCKBYTEBUFFER_H_

#include <gmock/gmock.h>
#include <ByteBuffer.h>
#include <istream>

namespace zios {
namespace common {

class MockByteBuffer : public ByteBuffer {
public:
    MockByteBuffer() : ByteBuffer(1) {}
    MockByteBuffer(const MockByteBuffer& otherBuffer) : ByteBuffer(otherBuffer) {}
    virtual ~MockByteBuffer() {}

    MOCK_CONST_METHOD0(slice, ByteBuffer*());
    MOCK_METHOD1(setPosition, void(int newPosition));
    MOCK_METHOD1(putString, void(const std::string& str));
    MOCK_METHOD1(put, void(const std::string& str));
    MOCK_METHOD1(putS8, void(int8_t value) );
    MOCK_METHOD1(putU8, void(uint8_t value) );
    MOCK_METHOD1(putS16, void(int16_t value) );
    MOCK_METHOD1(putU16, void(uint16_t value) );
    MOCK_METHOD1(putS32, void(int32_t value) );
    MOCK_METHOD1(putU32, void(uint32_t value) );
    MOCK_METHOD1(putS64, void(int64_t value) );
    MOCK_METHOD1(putU64, void(uint64_t value) );
    MOCK_METHOD1(putFloat, void(float value) );
    MOCK_METHOD1(putDouble, void(double value) );
    MOCK_METHOD2(put, int(const int8_t* buffer, int bufferLength));
    MOCK_METHOD2(putAll, int(std::istream&, int));

    MOCK_METHOD0(getS8, int8_t());
    MOCK_METHOD0(getU8, uint8_t());
    MOCK_METHOD0(getS16, int16_t());
    MOCK_METHOD0(getU16, uint16_t());
    MOCK_METHOD0(getS32, int32_t());
    MOCK_METHOD0(getU32,  uint32_t());
    MOCK_METHOD0(getS64,  int64_t());
    MOCK_METHOD0(getU64,  uint64_t());
    MOCK_METHOD0(getFloat, float());
    MOCK_METHOD0(getDouble, double());
    MOCK_METHOD0(getString, std::string());
    MOCK_METHOD1(getString, int(std::string& returnValue));

    MOCK_METHOD1(getS8, int(int8_t& returnValue));
    MOCK_METHOD1(getU8, int(uint8_t& returnValue));
    MOCK_METHOD1(getS16, int(int16_t& returnValue));
    MOCK_METHOD1(getU16, int(uint16_t& returnValue));
    MOCK_METHOD1(getS32, int(int32_t& returnValue));
    MOCK_METHOD1(getU32, int(uint32_t& returnValue));
    MOCK_METHOD1(getS64, int(int64_t& returnValue));
    MOCK_METHOD1(getU64, int(uint64_t& returnValue));
    MOCK_METHOD1(getFloat, int(float& returnValue));
    MOCK_METHOD1(getDouble, int(double& returnValue));

    MOCK_CONST_METHOD0(buffer, const unsigned char*());
    MOCK_METHOD2(get, int(int8_t* buffer, int bufferLength));
    MOCK_METHOD2(getAll, int(int8_t* buffer, int bufferLength));

    bool operator==(const MockByteBuffer& otherBuffer) const {
        return this == &otherBuffer;
    }
};

}
}

#endif /* MOCKBYTEBUFFER_H_ */
