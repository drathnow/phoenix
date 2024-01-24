/** @file ProtocolPacketTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 20, 2014 by daver:
 * @endhistory
 */

#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <stdint.h>
#include <ProtocolPacket.h>
#include <ByteBufferWrapper.h>

#include "BasePacketTest.h"

namespace zios {
namespace common {

class RecieveProtocolPacketTest: public testing::Test
{
public:
};

TEST_F(RecieveProtocolPacketTest, shouldExtractBytesFromHeaderAndTrailer)
{
    uint8_t buffer[10];

    ReceiveProtocolPacket packet(buffer, 10);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[0], packet.body());
    ASSERT_EQ(10U, packet.bodyLength());
    ASSERT_EQ(&buffer[10], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(8);
    packet.extractTrailer(2);
    packet.extractHeader(3);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(3U, packet.headerLength());
    ASSERT_EQ(&buffer[3], packet.body());
    ASSERT_EQ(3U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.trim();

    ASSERT_EQ(&buffer[3], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[3], packet.body());
    ASSERT_EQ(3U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

}

TEST_F(RecieveProtocolPacketTest, shouldExtractBytesFromTrailer)
{
    uint8_t buffer[10];

    ReceiveProtocolPacket packet(buffer, 10);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[0], packet.body());
    ASSERT_EQ(10U, packet.bodyLength());
    ASSERT_EQ(&buffer[10], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(8);
    packet.extractTrailer(2);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[0], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.trim();

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[0], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

TEST_F(RecieveProtocolPacketTest, shouldExtractBytesFromHeader)
{
    uint8_t buffer[10];

    ReceiveProtocolPacket packet(buffer, 10);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[0], packet.body());
    ASSERT_EQ(10U, packet.bodyLength());
    ASSERT_EQ(&buffer[10], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(8);
    packet.extractHeader(2);

    ASSERT_EQ(&buffer[0], packet.header());
    ASSERT_EQ(2U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.trim();

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

class TransmitProtocolPacketTest: public testing::Test
{
public:
    uint8_t* bufferFromTransmitProtocolPatcket(TransmitProtocolPacket& packet)
    {
        return packet._buffer;
    }
};

class WhenTransmitProtocolPacketIsUsingAWrappedBuffer: public TransmitProtocolPacketTest
{
public:

};

TEST_F(WhenTransmitProtocolPacketIsUsingAWrappedBuffer, shouldAddBytesToHeaderAndTrailer)
{
    uint8_t buffer[10];

    TransmitProtocolPacket packet(buffer, 10, 4, 4);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addHeader(2);
    packet.addTrailer(2);

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(2U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

TEST_F(WhenTransmitProtocolPacketIsUsingAWrappedBuffer, shouldAddBytesToTrailer)
{
    uint8_t buffer[10];

    TransmitProtocolPacket packet(buffer, 10, 4, 4);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addTrailer(2);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

TEST_F(WhenTransmitProtocolPacketIsUsingAWrappedBuffer, shouldAddBytesToHeader)
{
    uint8_t buffer[10];

    TransmitProtocolPacket packet(buffer, 10, 4, 4);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addHeader(2);

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(2U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    ASSERT_FALSE(packet.resizeBody(1000));
}

class WhenTransmitProtocolPacketIsUsingAnInternalBuffer: public TransmitProtocolPacketTest
{
public:

};

TEST_F(WhenTransmitProtocolPacketIsUsingAnInternalBuffer, shouldAddBytesToHeaderAndTrailer)
{
    TransmitProtocolPacket packet(10, 4, 4);
    uint8_t* buffer = bufferFromTransmitProtocolPatcket(packet);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addHeader(2);
    packet.addTrailer(2);

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(2U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(6U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

TEST_F(WhenTransmitProtocolPacketIsUsingAnInternalBuffer, shouldAddBytesToTrailer)
{
    TransmitProtocolPacket packet(10, 4, 4);
    uint8_t* buffer = bufferFromTransmitProtocolPatcket(packet);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addTrailer(2);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(2U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());
}

TEST_F(WhenTransmitProtocolPacketIsUsingAnInternalBuffer, shouldAddBytesToHeader)
{
    TransmitProtocolPacket packet(10, 4, 4);
    uint8_t* buffer = bufferFromTransmitProtocolPatcket(packet);

    ASSERT_EQ(&buffer[4], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[8], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.setBodyLength(2);
    packet.addHeader(2);

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(2U, packet.headerLength());
    ASSERT_EQ(&buffer[4], packet.body());
    ASSERT_EQ(2U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    packet.merge();

    ASSERT_EQ(&buffer[2], packet.header());
    ASSERT_EQ(0U, packet.headerLength());
    ASSERT_EQ(&buffer[2], packet.body());
    ASSERT_EQ(4U, packet.bodyLength());
    ASSERT_EQ(&buffer[6], packet.trailer());
    ASSERT_EQ(0U, packet.trailerLength());

    ASSERT_TRUE(packet.resizeBody(1000));
}


TEST_F(WhenTransmitProtocolPacketIsUsingAnInternalBuffer, shouldWriteBytesInCorrectLocation)
{
    TransmitProtocolPacket packet(10, 4, 4);

    ByteBufferWrapper byteBuffer1(packet.body(), 4);

    byteBuffer1.putU8(0x01);
    byteBuffer1.putU8(0x02);
    byteBuffer1.putU8(0x03);
    byteBuffer1.putU8(0x04);

    packet.merge();

    packet.addHeader(2);
    packet.header()[0] = 0x11;
    packet.header()[1] = 0x12;

    packet.trailer()[0] = 0x21;
    packet.trailer()[1] = 0x22;
    packet.merge();

    uint8_t* buffer = bufferFromTransmitProtocolPatcket(packet);

    ASSERT_EQ(0x11, buffer[2]);
    ASSERT_EQ(0x12, buffer[3]);
    ASSERT_EQ(0x01, buffer[4]);
    ASSERT_EQ(0x02, buffer[5]);
    ASSERT_EQ(0x03, buffer[6]);
    ASSERT_EQ(0x04, buffer[7]);
    ASSERT_EQ(0x21, buffer[8]);
    ASSERT_EQ(0x22, buffer[9]);
}



class WhenTransmitProtocolPacketIsAskedToResizeToSize: public TransmitProtocolPacketTest
{
public:
    WhenTransmitProtocolPacketIsAskedToResizeToSize()
    {
    }
    ~WhenTransmitProtocolPacketIsAskedToResizeToSize()
    {
    }
};

TEST_F(WhenTransmitProtocolPacketIsAskedToResizeToSize, shouldResizeInternalBuffer)
{
    TransmitProtocolPacket packet(10, 4, 4);

    EXPECT_TRUE(packet.resizeBody(1000));

    EXPECT_EQ(1000, packet.bodyLength());

    EXPECT_EQ(1006, BasePacketTest::bufferLengthFromPacket(packet));
    EXPECT_EQ(4, BasePacketTest::headerRangeFromPacket(packet).offset);
    EXPECT_EQ(0, BasePacketTest::headerRangeFromPacket(packet).length);

    EXPECT_EQ(4, BasePacketTest::bodyRangeFromPacket(packet).offset);
    EXPECT_EQ(1000, BasePacketTest::bodyRangeFromPacket(packet).length);

    EXPECT_EQ(1004, BasePacketTest::trailerRangeFromPacket(packet).offset);
    EXPECT_EQ(0, BasePacketTest::trailerRangeFromPacket(packet).length);
}

class WhenTransmitProtocolPacketIsAskedToResetAfterResizeToSize: public TransmitProtocolPacketTest
{
public:
    WhenTransmitProtocolPacketIsAskedToResetAfterResizeToSize()
    {
    }
    ~WhenTransmitProtocolPacketIsAskedToResetAfterResizeToSize()
    {
    }
};

TEST_F(WhenTransmitProtocolPacketIsAskedToResetAfterResizeToSize, shouldRestoreOriginalSettings)
{
    TransmitProtocolPacket packet(10, 4, 4);

    EXPECT_TRUE(packet.resizeBody(1000));

    packet.reset(false);

    EXPECT_EQ(4, packet.bodyLength());

    EXPECT_EQ(10, BasePacketTest::bufferLengthFromPacket(packet));
    EXPECT_EQ(4, BasePacketTest::headerRangeFromPacket(packet).offset);
    EXPECT_EQ(0, BasePacketTest::headerRangeFromPacket(packet).length);

    EXPECT_EQ(4, BasePacketTest::bodyRangeFromPacket(packet).offset);
    EXPECT_EQ(4, BasePacketTest::bodyRangeFromPacket(packet).length);

    EXPECT_EQ(8, BasePacketTest::trailerRangeFromPacket(packet).offset);
    EXPECT_EQ(0, BasePacketTest::trailerRangeFromPacket(packet).length);
}

}
}
