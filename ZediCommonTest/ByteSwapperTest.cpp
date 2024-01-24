/** @file ByteSwapperTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 20, 2016 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ByteOrder.h>
#include <ByteSwapper.h>

namespace zios {
namespace common_tests {

using namespace zios::common;

static uint8_t U64_NORM_VALUE_ARRAY[] = {0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
static uint8_t U64_REVERSED_VALUE_ARRAY[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
static uint8_t U64_LONGSWAPPED_VALUE_ARRAY[] = {0xCC, 0xDD, 0xEE, 0xFF, 0x88, 0x99, 0xAA, 0xBB};
static uint8_t U64_WORDSWAPPED_VALUE_ARRAY[] = {0xEE, 0xFF, 0xCC, 0xDD, 0xAA, 0xBB, 0x88, 0x99};

static uint64_t U64_NORM_VALUE = *((uint64_t*)U64_NORM_VALUE_ARRAY);
static uint64_t U64_REVERSED_VALUE = *((uint64_t*)U64_REVERSED_VALUE_ARRAY);
static uint64_t U64_LONGSWAPPED_VALUE = *((uint64_t*)U64_LONGSWAPPED_VALUE_ARRAY);
static uint64_t U64_WORDSWAPPED_VALUE = *((uint64_t*)U64_WORDSWAPPED_VALUE_ARRAY);

static uint8_t U32_NORM_VALUE_ARRAY[] = {0xAA, 0xBB, 0xCC, 0xDD};
static uint8_t U32_REVERSED_VALUE_ARRAY[] = {0xDD, 0xCC, 0xBB, 0xAA};
static uint8_t U32_BYTESWAPPED_VALUE_ARRAY[] = {0xBB, 0xAA, 0xDD, 0xCC};
static uint8_t U32_WORDSWAPPED_VALUE_ARRAY[] = {0xCC, 0xDD, 0xAA, 0xBB};

static uint32_t U32_NORM_VALUE = *((uint32_t*)U32_NORM_VALUE_ARRAY);
static uint32_t U32_REVERSED_VALUE = *((uint32_t*)U32_REVERSED_VALUE_ARRAY);
static uint32_t U32_BYTESWAPPED_VALUE = *((uint32_t*)U32_BYTESWAPPED_VALUE_ARRAY);
static uint32_t U32_WORDSWAPPED_VALUE = *((uint32_t*)U32_WORDSWAPPED_VALUE_ARRAY);

TEST(WhenByteSwapperIsToldToSwapToLittleEndian, DISABLED_shouldReverseByteOrder) {
    uint32_t value = U32_NORM_VALUE;
    uint32_t expectedValue = htobe32(value);
    expectedValue = (expectedValue >> 16) | (expectedValue << 16);

    ByteSwapper::valueToByteSwapOrder(value, ByteSwapper::BYTE1234_LE);
    ASSERT_EQ(expectedValue, value);
}

/**
 * The results of this test depend on what the native order of the platform
 * is.  If the native order is BE, then we must get the same value back.  If the
 * native value is LE then we should get the reversed order back
 */
TEST(WhenByteSwapperIsToldToSwapToBigEndianSwap, DISABLED_shouldReverseByteOrder) {
    uint32_t value = U32_NORM_VALUE;
    uint32_t expectedValue = (value >> 16) | (value << 16);
    ByteSwapper::valueToByteSwapOrder(value, ByteSwapper::BYTE4321_BE);
    ASSERT_EQ(expectedValue, value);
}

TEST(WhenByteSwapperIsToldToSwapToByteSwap, shouldReturnWordSwappedByteOrder) {
    uint32_t value = U32_NORM_VALUE;
    uint16_t hi = U32_NORM_VALUE >> 16;
    uint16_t low = 0x0000ffff & U32_NORM_VALUE;
    uint32_t expectedValue = htobe16(hi) << 16 | htobe16(low);
    ByteSwapper::valueToByteSwapOrder(value, ByteSwapper::BYTE3412_BS);
    ASSERT_EQ(expectedValue, value);
}

TEST(WhenByteSwapperIsToldToDoWordSwap, shouldReturnLittleEndianByteOrder) {
    uint32_t value = U32_NORM_VALUE;
    uint32_t expectedValue = U32_NORM_VALUE >> 16 | (U32_NORM_VALUE << 16 & 0xffff0000);
    ByteSwapper::valueToByteSwapOrder(value, ByteSwapper::BYTE2143_WS);
    ASSERT_EQ(expectedValue, value);
}

TEST(WhenByteSwapperIsAskedToReturnByteSwapperByteOrderForName, shouldReturnByteSwapperOrder) {
    ASSERT_EQ(ByteSwapper::NONE, ByteSwapper::byteOrderForName("NONE"));
    ASSERT_EQ(ByteSwapper::NONE, ByteSwapper::byteOrderForName("none"));
    ASSERT_EQ(ByteSwapper::NONE, ByteSwapper::byteOrderForName("NoNe"));
    ASSERT_EQ(ByteSwapper::BYTE1234_LE, ByteSwapper::byteOrderForName("1234"));
    ASSERT_EQ(ByteSwapper::BYTE2143_WS, ByteSwapper::byteOrderForName("2143"));
    ASSERT_EQ(ByteSwapper::BYTE4321_BE, ByteSwapper::byteOrderForName("4321"));
    ASSERT_EQ(ByteSwapper::BYTE3412_BS, ByteSwapper::byteOrderForName("3412"));
    ASSERT_EQ(ByteSwapper::UNKNOWN, ByteSwapper::byteOrderForName("Foo"));
}

}
}


