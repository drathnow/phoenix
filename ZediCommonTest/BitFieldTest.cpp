/** @file BitMaskTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 20, 2017 by daver:
 * @endhistory
 */
#include <BitField.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <stdint.h>

namespace zios {
namespace common {

using namespace std;

class BitFieldTest : public testing::Test {
public:
    static const uint16_t THE_VALUE = 5693;

    virtual ~BitFieldTest() {}
};

class WhenBitFieldIsAskedToCreateBitMaskForAnInvalidString : public BitFieldTest {
public:
    ~WhenBitFieldIsAskedToCreateBitMaskForAnInvalidString() {}
};

TEST_F(WhenBitFieldIsAskedToCreateBitMaskForAnInvalidString, shouldReturnNullIfStartBitPlusSizeIsGreaterThan16) {
    ASSERT_TRUE(NULL == BitField::bitFieldForString("0.17"));
    ASSERT_TRUE(NULL == BitField::bitFieldForString("4.14"));
    ASSERT_TRUE(NULL == BitField::bitFieldForString("8.9"));
    ASSERT_TRUE(NULL == BitField::bitFieldForString("15.2"));
    ASSERT_TRUE(NULL == BitField::bitFieldForString("16"));
    ASSERT_TRUE(NULL == BitField::bitFieldForString("16.0"));
}

class WhenBitFieldIsAskedForValueWithBitsSet : public BitFieldTest {
public:
    WhenBitFieldIsAskedForValueWithBitsSet() {}
    ~WhenBitFieldIsAskedForValueWithBitsSet() {}

    void doTest(const char* maskSpec, int bitsValue, int expectedValue) {
        BitField* bitMask = BitField::bitFieldForString(maskSpec);
        ASSERT_TRUE(NULL != bitMask) << "Mask spec: " << maskSpec;
        ASSERT_EQ(expectedValue, bitMask->valueWithBitsSet(bitsValue, 0))
                << "Mask spec: " << maskSpec
                << ", bitsValue: " << bitsValue
                << ", expectedValue: " << expectedValue;
        delete bitMask;
    }

};

TEST_F(WhenBitFieldIsAskedForValueWithBitsSet, shouldSlideValueToCorrectBitsLocation) {
    doTest("0", 1, 1);
    doTest("0.1", 1, 1);
    doTest("0.2", 2, 2);
    doTest("2.3", 7, 28);
    doTest("2.3", 9, 4);
    doTest("2.7", 15, 60);
    doTest("2.3", 15, 28);
    doTest("4.8", 99, 1584);
}


class WhenBitFieldIsAskedToReturnAValueFromAnInteger : public BitFieldTest {
public:
    ~WhenBitFieldIsAskedToReturnAValueFromAnInteger() {}

    void doTest(const char* maskSpec, int expectedValue) {
        BitField* bitMask = BitField::bitFieldForString(maskSpec);
        ASSERT_TRUE(NULL != bitMask) << "Mask spec: " << maskSpec;
        ASSERT_EQ(expectedValue, bitMask->bitsValueFromValue(THE_VALUE));
        delete bitMask;
    }
};

TEST_F(WhenBitFieldIsAskedToReturnAValueFromAnInteger, shouldApplyMaskAndReturnValue1) {
    doTest("0", 1);
    doTest("0.1", 1);
    doTest("0.2", 1);
    doTest("0.4", 13);
    doTest("0.6", 61);
    doTest("0.8", 61);
    doTest("0.16", THE_VALUE);
    doTest("2.3", 7);
    doTest("2.5", 15);
    doTest("2.7", 15);
    doTest("2.14", 1423);
    doTest("4.5", 3);
    doTest("4.8", 99);
    doTest("6", 0);
    doTest("6.1", 0);
    doTest("6.3", 0);
    doTest("6.4", 8);
    doTest("8", 0);
    doTest("8.1", 0);
    doTest("8.8", 22);
    doTest("15", 0);
    doTest("15.1", 0);
}

class WhenBitMaskIsAskedToSetBitsInValue : public BitFieldTest {
public:
    WhenBitMaskIsAskedToSetBitsInValue() {}
    ~WhenBitMaskIsAskedToSetBitsInValue()  {}
};

TEST_F(WhenBitMaskIsAskedToSetBitsInValue, shouldClearCurrentValueAndSetBitsLeavingOtherUneffected) {
    BitField bitField(3, 2);
    EXPECT_EQ(0x0010, bitField.valueWithBitsSet(0x0002, 0x0000));
    EXPECT_EQ(0x1010, bitField.valueWithBitsSet(0x0002, 0x1000));
    EXPECT_EQ(0x1018, bitField.valueWithBitsSet(0x0003, 0x1000));
    EXPECT_EQ(0x1090, bitField.valueWithBitsSet(0x0002, 0x1098));
}

TEST(WhenBitFieldIsAskedToReturnAStringForSingleBitField, shoulFormatAndReturnString) {
    BitField bitField(2, 0);
    ASSERT_STREQ("2", bitField.toString().c_str());
}

TEST(WhenBitFieldIsAskedToReturnAStringForMultiBitField, shoulFormatAndReturnString) {
    BitField bitField(4, 3);
    ASSERT_STREQ("4.3", bitField.toString().c_str());
}


}  // namespace common
}
