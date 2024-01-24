/** @file ReadOnlyByteBufferTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 27, 2014 by daver:
 * @endhistory
 */


#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdint.h>
#include <iostream>
#include "commonmocks.h"
#include "zcomtest.h"

#include <string>
#include <HexDumper.h>
#include <ReadOnlyByteBuffer.h>
#include <ByteBufferWrapper.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

class ReadOnlyByteBufferTest : public ::testing::Test {
public:

};


static const int8_t S8_VALUE = 0x7f;
static const uint8_t U8_VALUE = 0xff;
static const int16_t S16_VALUE = 0x7fff;
static const uint16_t U16_VALUE = 0xffff;
static const int32_t S32_VALUE = 0x7fffffff;
static const uint32_t U32_VALUE = 0xffffffff;
static const uint64_t U64_VALUE = 0x7fffffffffffffffLL;
static const int64_t S64_VALUE = 0x7fffffffffffffffLL;
static const float FLOAT_VALUE = 4.2f;

TEST_F(ReadOnlyByteBufferTest, shouldReadS8) {
    int8_t value;
    uint8_t buf[4];
    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putS8(S8_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int8_t));

    ASSERT_EQ(sizeof(int8_t), byteBuffer.getS8(value));
    ASSERT_EQ(S8_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS8(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadU8) {
    uint8_t value;
    uint8_t buf[4];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putS8(U8_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(uint8_t));

    ASSERT_EQ(sizeof(uint8_t), byteBuffer.getU8(value));
    ASSERT_EQ(U8_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU8(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadS16) {
    int16_t value;
    uint8_t buf[4];

    ByteBufferWrapper wbb(buf, sizeof(buf), ByteOrder::BigEndian);
    wbb.putS16(S16_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int16_t), ByteOrder::BigEndian);

    ASSERT_EQ(sizeof(int16_t), byteBuffer.getS16(value));
    ASSERT_EQ(S16_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS16(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadU16) {
    uint16_t value;
    uint8_t buf[4];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putU16(U16_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int16_t));

    ASSERT_EQ(sizeof(uint16_t), byteBuffer.getU16(value));
    ASSERT_EQ(U16_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU16(value));
}


TEST_F(ReadOnlyByteBufferTest, shouldReadS32) {
    int32_t value;
    uint8_t buf[4];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putU32(S32_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int32_t));

    ASSERT_EQ(sizeof(int32_t), byteBuffer.getS32(value));
    ASSERT_EQ(S32_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS32(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadU32) {
    uint32_t value;
    uint8_t buf[4];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putU32(U32_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int32_t));

    ASSERT_EQ(sizeof(uint32_t), byteBuffer.getU32(value));
    ASSERT_EQ(U32_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU32(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadS64) {
    int64_t value;
    uint8_t buf[8];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putU64(U64_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(uint64_t));

    ASSERT_EQ(sizeof(int64_t), byteBuffer.getS64(value));
    ASSERT_EQ(S64_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS64(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadU64) {
    int64_t value;
    uint8_t buf[8];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putS64(S64_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(int64_t));

    ASSERT_EQ(sizeof(int64_t), byteBuffer.getS64(value));
    ASSERT_EQ(S64_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS64(value));
}

TEST_F(ReadOnlyByteBufferTest, shouldReadFloat) {
    float value;
    uint8_t buf[8];

    ByteBufferWrapper wbb(buf, sizeof(buf));
    wbb.putFloat(FLOAT_VALUE);

    ReadOnlyByteBuffer byteBuffer(buf, sizeof(float));

    ASSERT_EQ(sizeof(float), byteBuffer.getFloat(value));
    ASSERT_EQ(FLOAT_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getFloat(value));
}


TEST(WhenReadOnlyByteBufferIsToldToGetFloatFromBufferWrittenByByteBufferWrapper, shouldReturnSameValue) {
    float theValue = 1.2f;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putFloat(theValue);
    ReadOnlyByteBuffer roByteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.flip();
    ASSERT_FLOAT_EQ(theValue, roByteBuffer.getFloat());
}


TEST(WhenReadOnlyByteBufferIsToldToGetFloatFromBufferWrittenByByteBufferWrapperWithFloatParamReturn, shouldReturnSameValue) {
    float theValue = 1.2f;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putFloat(theValue);
    ReadOnlyByteBuffer roByteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.flip();

    float result;
    ASSERT_EQ(4, roByteBuffer.getFloat(result));
    ASSERT_FLOAT_EQ(theValue, result);
}

}
}
