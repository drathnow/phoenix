/** @file IP4AddressTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 24, 2018 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <IP4Address.h>

namespace zios {
namespace common {

using namespace std;

TEST(IP4AddressTest, IP4AddressShouldHandleNumericIP) {
    {
        IP4Address a1("0.0.0.0");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("0.0.0.0", a1.getHostname());
        EXPECT_EQ("0.0.0.0", a1.getNetwork());
    }
    {
        IP4Address a1("255.255.255.255");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("255.255.255.255", a1.getHostname());
        EXPECT_EQ("255.255.255.255", a1.getNetwork());
    }
    {
        IP4Address a1("192.168.128.10");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("192.168.128.10", a1.getHostname());
        EXPECT_EQ("192.168.128.10", a1.getNetwork());
    }
    {
        IP4Address a1("10.25.1.20");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("10.25.1.20", a1.getHostname());
        EXPECT_EQ("10.25.1.20", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleNumericIPWithSuppliedDefaultNetworksize) {
    {
        IP4Address a1("192.168.128.10", 24);
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(24, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.0", a1.getNetmask());
        EXPECT_EQ("192.168.128.10", a1.getHostname());
        EXPECT_EQ("192.168.128.0", a1.getNetwork());
    }
    {
        IP4Address a1("10.25.1.20", 23);
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(23, a1.getNetworkSize());
        EXPECT_EQ("255.255.254.0", a1.getNetmask());
        EXPECT_EQ("10.25.1.20", a1.getHostname());
        EXPECT_EQ("10.25.0.0", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleNumericIPWithNetmask) {
    {
        IP4Address a1("192.168.128.10/32");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("192.168.128.10", a1.getHostname());
        EXPECT_EQ("192.168.128.10", a1.getNetwork());
    }
    {
        IP4Address a1("192.168.128.10/24");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(24, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.0", a1.getNetmask());
        EXPECT_EQ("192.168.128.10", a1.getHostname());
        EXPECT_EQ("192.168.128.0", a1.getNetwork());
    }
    {
        IP4Address a1("192.168.128.10/29");
        EXPECT_TRUE(a1.isValid());
        EXPECT_TRUE(a1.isNumeric());
        EXPECT_EQ(29, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.248", a1.getNetmask());
        EXPECT_EQ("192.168.128.10", a1.getHostname());
        EXPECT_EQ("192.168.128.8", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleEmptyString) {
    {
        IP4Address a1("");
        EXPECT_FALSE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
    {
        IP4Address a1(" ");
        EXPECT_FALSE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleNonNumericHost) {
    {
        IP4Address a1("one.two.com");
        EXPECT_TRUE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("one.two.com", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
    {
        IP4Address a1("o");
        EXPECT_TRUE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(32, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.255", a1.getNetmask());
        EXPECT_EQ("o", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleNonNumericHostWithSuppliedDefaultNetworksize) {
    {
        IP4Address a1("one.two.com", 24);
        EXPECT_TRUE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(24, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.0", a1.getNetmask());
        EXPECT_EQ("one.two.com", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
}

TEST(IP4AddressTest, IP4AddressShouldHandleNonNumericHostWithNetmask) {
    {
        IP4Address a1("one.two.com/24");
        EXPECT_TRUE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(24, a1.getNetworkSize());
        EXPECT_EQ("255.255.255.0", a1.getNetmask());
        EXPECT_EQ("one.two.com", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
    {
        IP4Address a1("o/15");
        EXPECT_TRUE(a1.isValid());
        EXPECT_FALSE(a1.isNumeric());
        EXPECT_EQ(15, a1.getNetworkSize());
        EXPECT_EQ("255.254.0.0", a1.getNetmask());
        EXPECT_EQ("o", a1.getHostname());
        EXPECT_EQ("", a1.getNetwork());
    }
}

TEST(IP4AddressTest, shouldDecodeEncodedString) {
    std::string encodedString("1.2.3.4");
    string ipaddress;
    int32_t portNumber;

    ASSERT_TRUE(IP4Address::decodeEncodedString("1.2.3.4", ipaddress, portNumber));
    ASSERT_STREQ("1.2.3.4", ipaddress.c_str());
    ASSERT_EQ(-1, portNumber);

    ASSERT_TRUE(IP4Address::decodeEncodedString("1.2.3.4/23", ipaddress, portNumber));
    ASSERT_STREQ("1.2.3.4/23", ipaddress.c_str());
    ASSERT_EQ(-1, portNumber);


    ASSERT_TRUE(IP4Address::decodeEncodedString("1.2.3.4:2100", ipaddress, portNumber));
    ASSERT_STREQ("1.2.3.4", ipaddress.c_str());
    ASSERT_EQ(2100, portNumber);

    ASSERT_TRUE(IP4Address::decodeEncodedString("1.2.3.4/23:2300", ipaddress, portNumber));
    ASSERT_STREQ("1.2.3.4/23", ipaddress.c_str());
    ASSERT_EQ(2300, portNumber);
}

}
}

