/** @file ByteBufferTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 6, 2013 by daver:
 * IncrDev Oct 02, 2013 by eganb: added more tests for U32 and added float test.
 * IncrDev Oct 03, 2013 by eganb: added in exception tests for over/underflow
 * @endhistory
 */

#include <BufferOverflowException.h>
#include <BufferUnderflowException.h>
#include <ByteBuffer.h>
#include <ByteBufferWrapper.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include "Utl.h"
#include "zcomtest.h"

namespace testing { namespace internal { class Secret; } }

using namespace zios::common;

class ByteBufferWrapperTest : public testing::Test {

};

static const int8_t S8_VALUE = 0x7f;
static const uint8_t U8_VALUE = 0xff;
static const int16_t S16_VALUE = 0x7fff;
static const uint16_t U16_VALUE = 0xffff;
static const int32_t S32_VALUE = 0x7fffffff;
static const uint32_t U32_VALUE = 0xffffffff;
static const uint64_t U64_VALUE =  1099511892096LL;
static const int64_t S64_VALUE = 0x7fffffffffffffffLL;
static const float FLOAT_VALUE = 4.2f;


TEST_F(ByteBufferWrapperTest, shouldWriteU64) {
    uint8_t buffer[10];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putU64(U64_VALUE);

    ASSERT_EQ(8, byteBuffer.position());
    ASSERT_EQ(0x00, buffer[0]);
    ASSERT_EQ(0x00, buffer[1]);
    ASSERT_EQ(0x01, buffer[2]);
    ASSERT_EQ(0x00, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
    ASSERT_EQ(0x04, buffer[5]);
    ASSERT_EQ(0x08, buffer[6]);
    ASSERT_EQ(0x80, buffer[7]);
}


TEST_F(ByteBufferWrapperTest, shouldWriteS64) {
    uint8_t buffer[10];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putS64(S64_VALUE);

    ASSERT_EQ(8, byteBuffer.position());
    ASSERT_EQ(0x7F, buffer[0]);
    ASSERT_EQ(0xFF, buffer[1]);
    ASSERT_EQ(0xFF, buffer[2]);
    ASSERT_EQ(0xFF, buffer[3]);
    ASSERT_EQ(0xFF, buffer[4]);
    ASSERT_EQ(0xFF, buffer[5]);
    ASSERT_EQ(0xFF, buffer[6]);
    ASSERT_EQ(0xFF, buffer[7]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteU32) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putU32(0xFFFFFFFF);

    ASSERT_EQ(4, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0xFF, buffer[1]);
    ASSERT_EQ(0xFF, buffer[2]);
    ASSERT_EQ(0xFF, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteS32) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putS32(0xFFFFFFFF);

    ASSERT_EQ(4, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0xFF, buffer[1]);
    ASSERT_EQ(0xFF, buffer[2]);
    ASSERT_EQ(0xFF, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteU16) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putU16(0xFFFF);

    ASSERT_EQ(2, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0xFF, buffer[1]);
    ASSERT_EQ(0x00, buffer[2]);
    ASSERT_EQ(0x00, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteS16) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putS16(0xFFFF);

    ASSERT_EQ(2, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0xFF, buffer[1]);
    ASSERT_EQ(0x00, buffer[2]);
    ASSERT_EQ(0x00, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteU8) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putU8(0xFF);

    ASSERT_EQ(1, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0x00, buffer[1]);
    ASSERT_EQ(0x00, buffer[2]);
    ASSERT_EQ(0x00, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteS8) {
    uint8_t buffer[5];
    ::memset(buffer, 0, sizeof(buffer));
    ByteBufferWrapper byteBuffer(buffer, sizeof(buffer));
    byteBuffer.putS8(0xFF);

    ASSERT_EQ(1, byteBuffer.position());
    ASSERT_EQ(0xFF, buffer[0]);
    ASSERT_EQ(0x00, buffer[1]);
    ASSERT_EQ(0x00, buffer[2]);
    ASSERT_EQ(0x00, buffer[3]);
    ASSERT_EQ(0x00, buffer[4]);
}

TEST_F(ByteBufferWrapperTest, shouldOnlyWriteAsManyBytesAsAvailableInUnderlyingByteStream) {
    uint8_t byteStream[2];
    int8_t bytesToWrite[4];

    bytesToWrite[0] = 0x00;
    bytesToWrite[1] = 0x01;
    bytesToWrite[2] = 0x02;
    bytesToWrite[3] = 0x03;

    ByteBufferWrapper byteBuffer(byteStream, sizeof(byteStream));
    ASSERT_EQ(2, byteBuffer.put(bytesToWrite, sizeof(bytesToWrite)));
    ASSERT_EQ(0x00, byteStream[0]);
    ASSERT_EQ(0x01, byteStream[1]);
}

TEST_F(ByteBufferWrapperTest, shouldWriteAsManyBytesAsAvailableUpToMaxInUnderlyingByteStream) {
    uint8_t byteStream[4];
    int8_t bytesToWrite[2];

    bytesToWrite[0] = 0x00;
    bytesToWrite[1] = 0x01;

    ByteBufferWrapper byteBuffer(byteStream, sizeof(byteStream));
    ASSERT_EQ(2, byteBuffer.put(bytesToWrite, sizeof(bytesToWrite)));
    ASSERT_EQ(0x00, byteStream[0]);
    ASSERT_EQ(0x01, byteStream[1]);
}

TEST_F(ByteBufferWrapperTest, shouldRead) {
    unsigned char PKT[] = {0x01, 0x00, 0x05, 0x00, 0x01, 0x00, 0x09, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    int8_t buffer[9];

    ByteBufferWrapper byteBuffer(PKT, 16);

    ASSERT_EQ(0x01, byteBuffer.getU8());
    ASSERT_EQ(0x05, byteBuffer.getU16());
    ASSERT_EQ(0x01, byteBuffer.getU16());
    ASSERT_EQ(0x09, byteBuffer.getU16());

    byteBuffer.get(buffer, sizeof(buffer));

    ASSERT_EQ(0x01, buffer[0]);
    ASSERT_EQ(0x02, buffer[1]);
    ASSERT_EQ(0x03, buffer[2]);
    ASSERT_EQ(0x04, buffer[3]);
    ASSERT_EQ(0x05, buffer[4]);
    ASSERT_EQ(0x06, buffer[5]);
    ASSERT_EQ(0x07, buffer[6]);
    ASSERT_EQ(0x08, buffer[7]);
    ASSERT_EQ(0x09, buffer[8]);
}

TEST_F(ByteBufferWrapperTest, shouldSliceBuffer) {
    uint8_t buf[100];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.setPosition(6);

    ByteBufferWrapper* otherBuffer = byteBuffer.slice();
    std::unique_ptr<ByteBufferWrapper> foo(otherBuffer);
    otherBuffer->setPosition(0);

    otherBuffer->put((const int8_t*)"Hello ", sizeof("Hello "));
    byteBuffer.put((const int8_t*)"World\0", sizeof("World\0"));
    ASSERT_EQ(0, ::strcmp("Hello World", (const char*)buf));
}

TEST_F(ByteBufferWrapperTest, shouldAssignBuffer) {
    uint8_t buf[100];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.setPosition(6);

    ByteBufferWrapper otherBuffer = byteBuffer;
    otherBuffer.setPosition(0);

    otherBuffer.put((const int8_t*)"Hello ", sizeof("Hello "));
    byteBuffer.put((const int8_t*)"World\0", sizeof("World\0"));
    ASSERT_EQ(0, ::strcmp("Hello World", (const char*)buf));
}

TEST_F(ByteBufferWrapperTest, shouldWriteString) {
    std::string STR("Hello World");
    unsigned char buf[100];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putString(STR);
    byteBuffer.flip();

    std::string result = byteBuffer.getString();

    EXPECT_EQ(STR, result);
}

TEST_F(ByteBufferWrapperTest, shouldWriteEmptyString) {
    std::string STR;
    unsigned char buf[100];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putString(STR);
    ASSERT_EQ(2, byteBuffer.position());
    byteBuffer.flip();

    std::string result = byteBuffer.getString();

    ASSERT_STREQ(STR.c_str(), result.c_str());
}


TEST_F(ByteBufferWrapperTest, shouldReadAndWriteU64) {
    unsigned char buf[sizeof(uint64_t)];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putU64(U64_VALUE);
    byteBuffer.flip();

    EXPECT_EQ(U64_VALUE, byteBuffer.getU64());
}

TEST_F(ByteBufferWrapperTest, shouldReadAndWriteS64) {
    unsigned char buf[sizeof(int64_t)];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putS64(S64_VALUE);
    byteBuffer.flip();

    EXPECT_EQ(S64_VALUE, byteBuffer.getS64());
}

TEST_F(ByteBufferWrapperTest, shouldReadString) {
    std::string STR("Hello World");
    unsigned char buf[100];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putU16(STR.length());
    byteBuffer.put((int8_t*)STR.c_str(), STR.length());
    byteBuffer.flip();

    std::string result = byteBuffer.getString();

    EXPECT_EQ(STR, result);
}

TEST_F(ByteBufferWrapperTest, shouldConvertU32) {
    uint32_t_value_t uint32_tValue;
    unsigned char buf[4];
    uint32_t value = 0xFF00FF00;
    ByteBufferWrapper byteBuffer(buf, 4);

    // simple read known value test
    ::memcpy(buf, &value, sizeof(uint32_t));
    uint32_tValue.value = byteBuffer.getU32();
    ASSERT_EQ((char)0xFF, uint32_tValue.svar.byte0);
    ASSERT_EQ(0x00, uint32_tValue.svar.byte1);
    ASSERT_EQ((char)0xFF, uint32_tValue.svar.byte2);
    ASSERT_EQ(0x00, uint32_tValue.svar.byte3);


    // simple write known value test
	byteBuffer.clear();				// reset our byteBuffer for reuse.
    EXPECT_NO_THROW(byteBuffer.putU32(value));
    EXPECT_EQ((unsigned char)0xFF, buf[0]);
    EXPECT_EQ(0x00, buf[1]);
    EXPECT_EQ((unsigned char)0xFF, buf[2]);
    EXPECT_EQ(0x00, buf[3]);

    // read back test.
	byteBuffer.clear();				// reset our byteBuffer for reuse. (read from beginning)
	uint32_tValue.value = 0;
	EXPECT_NO_THROW(uint32_tValue.value = byteBuffer.getU32());
    EXPECT_EQ(value,uint32_tValue.value);


}

TEST_F(ByteBufferWrapperTest, shouldConvertFloat) {
    float_value_t floatValue;
    unsigned char buf[4];
    float value = 1.24356e04;

    ByteBufferWrapper byteBuffer(buf, 4);

    // write a float out to buffer (in BE) ... simple write known value test.
    EXPECT_NO_THROW(byteBuffer.putFloat(value));
    EXPECT_EQ((char)0x46, buf[0]);
    EXPECT_EQ((char)0x42, buf[1]);
    EXPECT_EQ((char)0x4e, buf[2]);
    EXPECT_EQ((char)0x66, buf[3]);

    // get float back from buffer to be original floating point
    byteBuffer.clear();
    EXPECT_NO_THROW(floatValue.value = byteBuffer.getFloat());
    EXPECT_FLOAT_EQ(value, floatValue.value);
}


TEST_F(ByteBufferWrapperTest, Exceptions) {
    unsigned char buf[4];
    float value = 1.24356e04;

    ByteBufferWrapper byteBuffer(buf, 4);

    // try to write past end of buffer.... (2 floats)
    EXPECT_NO_THROW(byteBuffer.putFloat(value));
    EXPECT_THROW(byteBuffer.putFloat(value), BufferOverflowException);

    //  try to read past end of bytebuffer (didn't reset it)
    EXPECT_THROW(byteBuffer.getFloat(), BufferUnderflowException);
}


TEST_F(ByteBufferWrapperTest, shouldReadS8) {
    int8_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS8(S8_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(int8_t), byteBuffer.getS8(value));
    ASSERT_EQ(S8_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS8(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadU8) {
    uint8_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS8(U8_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(uint8_t), byteBuffer.getU8(value));
    ASSERT_EQ(U8_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU8(value));
}


TEST_F(ByteBufferWrapperTest, shouldReadS16) {
    int16_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS16(S16_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(int16_t), byteBuffer.getS16(value));
    ASSERT_EQ(S16_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS16(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadU16) {
    uint16_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS16(U16_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(uint16_t), byteBuffer.getU16(value));
    ASSERT_EQ(U16_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU16(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadS32) {
    int32_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS32(S32_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(int32_t), byteBuffer.getS32(value));
    ASSERT_EQ(S32_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS32(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadU32) {
    uint32_t value;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS32(U32_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(uint32_t), byteBuffer.getU32(value));
    ASSERT_EQ(U32_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU32(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadS64) {
    int64_t value;
    uint8_t buf[8];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS64(S64_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(int64_t), byteBuffer.getS64(value));
    ASSERT_EQ(S64_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getS64(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadU64) {
    uint64_t value;
    uint8_t buf[8];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS64(U64_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(uint64_t), byteBuffer.getU64(value));
    ASSERT_EQ(U64_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getU64(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadFloat) {
    float value;
    uint8_t buf[8];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putFloat(FLOAT_VALUE);
    byteBuffer.flip();

    ASSERT_EQ(sizeof(float), byteBuffer.getFloat(value));
    ASSERT_EQ(FLOAT_VALUE, value);
    ASSERT_EQ(ByteBuffer::BUFFER_UNDERFLOW, byteBuffer.getFloat(value));
}

TEST_F(ByteBufferWrapperTest, shouldReadAllUpToMaxBufferSizeIfMoreArePresent) {
    uint8_t buf[4];
    int8_t rcvBuf[2];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS8(0x01);
    byteBuffer.putS8(0x02);
    byteBuffer.putS8(0x03);
    byteBuffer.putS8(0x04);
    byteBuffer.flip();

    ASSERT_EQ(2, byteBuffer.getAll(rcvBuf, sizeof(rcvBuf)));
    ASSERT_EQ(0x01, rcvBuf[0]);
    ASSERT_EQ(0x02, rcvBuf[1]);
}

TEST_F(ByteBufferWrapperTest, shouldReadAllUpToWhatsAvailable) {
    uint8_t buf[2];
    int8_t rcvBuf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);
    byteBuffer.putS8(0x01);
    byteBuffer.putS8(0x02);
    byteBuffer.flip();

    ASSERT_EQ(2, byteBuffer.getAll(rcvBuf, sizeof(rcvBuf)));
    ASSERT_EQ(0x01, rcvBuf[0]);
    ASSERT_EQ(0x02, rcvBuf[1]);
}

TEST(WhenByteBufferWrapperIsToldToPutFloatAndThenGetFloat, shouldReturnSameValue) {
    float theValue = 1.2f;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putFloat(theValue);
    byteBuffer.flip();
    ASSERT_FLOAT_EQ(theValue, byteBuffer.getFloat());
}

TEST(WhenByteBufferWrapperIsToldToPutFloatAndThenGetFloatWithReturnReference, shouldReturnSameValue) {
    float theValue = 1.2f;
    uint8_t buf[4];
    ByteBufferWrapper byteBuffer(buf, sizeof(buf), ByteOrder::BigEndian);

    byteBuffer.putFloat(theValue);
    byteBuffer.flip();
    float result;
    ASSERT_EQ(4, byteBuffer.getFloat(result));
    ASSERT_FLOAT_EQ(theValue, result);
}
