/** @file ByteUtilTest.cpp
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
 * IncrDev Apr 03, 2014 by eb:  removed extraneous log4cplus init
 * @endhistory
 */
#include <iostream>
#include <log4cplus/configurator.h>
#include <sys/time.h>

#include "gtest/gtest.h"
#include "ByteUtil.h"

using namespace zios::common;
using namespace log4cplus;
using namespace std;

class ByteUtilTest: public testing::Test {
protected:
};

const uint8_t NET_ORDER_BYTES[] = {0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03};


TEST_F(ByteUtilTest, shouldConvert) {
    ASSERT_EQ(1, (int)ByteUtil::getS8(&NET_ORDER_BYTES[0]));
    ASSERT_EQ(2, (int)ByteUtil::getS16(&NET_ORDER_BYTES[1]));
    ASSERT_EQ(3, (int)ByteUtil::getS32(&NET_ORDER_BYTES[3]));
}

