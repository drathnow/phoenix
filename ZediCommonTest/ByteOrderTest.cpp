/** @file ByteOrderTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 24, 2016 by daver:
 * @endhistory
 */
#include "zcomtest.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <IllegalArgumentException.h>
#include <ByteOrder.h>
#include <HexDumper.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

class ByteOrderTest : public testing::Test {
public:
    ~ByteOrderTest() {}

    static const uint16_t U16_NORM_VALUE;
    static const uint16_t U16_SWAP_VALUE;
    static const uint32_t U32_NORM_VALUE;
    static const uint32_t U32_SWAP_VALUE;
    static const uint32_t U32_WORD_SWAPPED_VALUE;
    static const uint32_t U32_BYTE_SWAPPED_VALUE;
    static const uint64_t U64_NORM_VALUE;
    static const uint64_t U64_SWAP_VALUE;
};

const uint16_t ByteOrderTest::U16_NORM_VALUE = 0xAABB;
const uint16_t ByteOrderTest::U16_SWAP_VALUE = 0xBBAA;
const uint32_t ByteOrderTest::U32_NORM_VALUE = 0xAABBCCDD;
const uint32_t ByteOrderTest::U32_SWAP_VALUE = 0xDDCCBBAA;
const uint32_t ByteOrderTest::U32_WORD_SWAPPED_VALUE = 0xCCDDAABB;
const uint32_t ByteOrderTest::U32_BYTE_SWAPPED_VALUE = 0xBBAADDCC;
const uint64_t ByteOrderTest::U64_NORM_VALUE = ((uint64_t(0x8899AABB) << 32) + 0xCCDDEEFF);
const uint64_t ByteOrderTest::U64_SWAP_VALUE = ((uint64_t(0xFFEEDDCC) << 32) + 0xBBAA9988);


class WhenByteOrderIsAskedToReturnByteOrderForByteOrderNumber : public ByteOrderTest {
public:
    ~WhenByteOrderIsAskedToReturnByteOrderForByteOrderNumber() {}
};

TEST_F(WhenByteOrderIsAskedToReturnByteOrderForByteOrderNumber, shouldReturnLittleEndianForLE) {
    const ByteOrder* bo = ByteOrder::byteOrderForByteOrderNumber(ByteOrder::LittleEndian.byteOrderNumber());
    ASSERT_EQ(&ByteOrder::LittleEndian, bo);
    ASSERT_EQ(ByteOrder::LittleEndian.byteOrderNumber(), bo->byteOrderNumber());
}

TEST_F(WhenByteOrderIsAskedToReturnByteOrderForByteOrderNumber, shouldReturnBigEndianForBE) {
    const ByteOrder* bo = ByteOrder::byteOrderForByteOrderNumber(ByteOrder::BigEndian.byteOrderNumber());
    ASSERT_EQ(&ByteOrder::BigEndian, bo);
    ASSERT_EQ(ByteOrder::BigEndian.byteOrderNumber(), bo->byteOrderNumber());
}

TEST_F(WhenByteOrderIsAskedToReturnByteOrderForByteOrderNumber, shouldThrowExceptionIfBogusByteOrderNumberPassed) {
    ASSERT_TRUE(NULL == ByteOrder::byteOrderForByteOrderNumber(99));
}

class WhenByteOrderIsToldToConvertToSpecificByteOrder : public ByteOrderTest {
public:
    ~WhenByteOrderIsToldToConvertToSpecificByteOrder() {}
};

TEST_F(WhenByteOrderIsToldToConvertToSpecificByteOrder, shouldConvertU16ToTargetByteOrder) {
    ASSERT_EQ(U16_NORM_VALUE, ByteOrder::LittleEndian.toByteOrder(U16_NORM_VALUE, ByteOrder::LittleEndian));
    ASSERT_EQ(U16_SWAP_VALUE, ByteOrder::LittleEndian.toByteOrder(U16_NORM_VALUE, ByteOrder::BigEndian));

    ASSERT_EQ(U32_NORM_VALUE, ByteOrder::LittleEndian.toByteOrder(U32_NORM_VALUE, ByteOrder::LittleEndian));
    ASSERT_EQ(U32_SWAP_VALUE, ByteOrder::LittleEndian.toByteOrder(U32_NORM_VALUE, ByteOrder::BigEndian));

    ASSERT_EQ(U64_NORM_VALUE, ByteOrder::LittleEndian.toByteOrder(U64_NORM_VALUE, ByteOrder::LittleEndian));
    ASSERT_EQ(U64_SWAP_VALUE, ByteOrder::LittleEndian.toByteOrder(U64_NORM_VALUE, ByteOrder::BigEndian));
}

}
}


