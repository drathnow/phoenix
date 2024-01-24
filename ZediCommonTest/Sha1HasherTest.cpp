/** @file Sha1HasherTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 5, 2013 by daver:
 * IncrDev Apr 03, 2014 by eb:  removed extraneous log4cplus init
 * @endhistory
 */

#include <gtest/gtest.h>
#include <log4cplus/configurator.h>
#include <Hasher.h>
#include <cstring>
#include <iostream>
#include <new>
#include <string>

namespace log4cplus { class Hierarchy; }
namespace testing { namespace internal { class Secret; } }

using namespace zios::common;
using namespace std;

class Sha1HasherTest: public testing::Test {
protected:
};

static const uint8_t EXPECTED_HASH[] = {0x31, 0xBB, 0x21, 0x0C, 0xC0, 0x02, 0xCD, 0xBD, 0x5F, 0x2C, 0xAE, 0xFE, 0x71, 0x7D, 0x61, 0x1D, 0x40, 0xC1, 0xDE, 0x05};
static const uint8_t STRING1[] = "Hello";
static const uint8_t STRING2[] = "World";

TEST_F(Sha1HasherTest, shouldCalculateHash) {
    unsigned char buffer[20];
    Sha1Hasher hasher(100);

    hasher.update(STRING1, ::strlen((const char*)STRING1));
    hasher.update(STRING2, ::strlen((const char*)STRING2));

    int size = hasher.hashedValue(buffer, sizeof(buffer));

    ASSERT_EQ(20, size);
    ASSERT_EQ(0, ::memcmp(buffer, EXPECTED_HASH, size));
}
