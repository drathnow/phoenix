/** @file UtlTest.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 2, 2015 by andrewj:
 * IncrDev Jun 18, 2015 by eb: added base64 test and bitReverse test
 * @endhistory
 */
#include "gtest/gtest.h"
#include "Utl.h"

#include <cstring>
#include <File.h>
#include <StringUtl.h>
#include <map>
#include <fstream>

using namespace zios::common;
using namespace std;

class UtlTest: public testing::Test {
protected:
};

TEST_F(UtlTest, shouldAddTimeToTimespec) {
    struct timespec tp = { 0, 0};

    Utl::timespecAddTime(&tp, 100);
    EXPECT_EQ(0,            tp.tv_sec);
    EXPECT_EQ(100000000,    tp.tv_nsec);
}

TEST_F(UtlTest, shouldAddTimeToTimespecWithSecRollover) {
    struct timespec tp = { 0, 0};

    Utl::timespecAddTime(&tp, 5001L);
    EXPECT_EQ(5,        tp.tv_sec);
    EXPECT_EQ(1000000,  tp.tv_nsec);
}

TEST_F(UtlTest, shouldAddTimeToTimespecWithNSecRollover) {
    struct timespec tp = { 0, 900000000};

    // adds just enough to trip nsec over to 1 sec
    Utl::timespecAddTime(&tp, 100L);
    EXPECT_EQ(1, tp.tv_sec);
    EXPECT_EQ(0, tp.tv_nsec);

    // edge case, should add a full second to secs and leave nsec alone
    tp.tv_sec = 0;
    tp.tv_nsec = 1;
    Utl::timespecAddTime(&tp, 1000L);
    EXPECT_EQ(1, tp.tv_sec);
    EXPECT_EQ(1, tp.tv_nsec);

    // edge case, just over 1 second before adjusting
    tp.tv_sec = 0;
    tp.tv_nsec = 900000001;
    Utl::timespecAddTime(&tp, 100L);
    EXPECT_EQ(1, tp.tv_sec);
    EXPECT_EQ(1, tp.tv_nsec);

    // edge case, just over 1 second before adjusting
    tp.tv_sec = 0;
    tp.tv_nsec = 999999999;
    Utl::timespecAddTime(&tp, 1L);
    EXPECT_EQ(1, tp.tv_sec);
    EXPECT_EQ(999999, tp.tv_nsec);

    // edge case, add just enough to prevent rollover to a sec
    tp.tv_sec = 0;
    tp.tv_nsec = 899999999;
    Utl::timespecAddTime(&tp, 100L);
    EXPECT_EQ(0, tp.tv_sec);
    EXPECT_EQ(999999999, tp.tv_nsec);

}


TEST_F(UtlTest, shouldDecodebase64) {
    const string encodedString = "QSBnb29kIHN0cmluZyB0byBlbmNvZGUgdG8gdGVzdCBpZiB0aGUgZGVjb2RlciB3b3Jrcy4=";
    const string testString = "A good string to encode to test if the decoder works.";

   vector<uint8_t> tv = Utl::base64_decode(encodedString);

    for (size_t i = 0; i < testString.length(); i++) {
        EXPECT_EQ(testString[i], tv[i]);
    }
}

TEST_F(UtlTest, shouldReverseInt) {

    const unsigned int int1 = 0xF0F0;
    unsigned int int2 = 0xA5A5;
    unsigned int int3 = 12345;
    unsigned int int4 = 0x12345678;
    const unsigned int int1a = 0x0f0f0000;
    unsigned int int2a = 0xa5a50000;
    unsigned int int3a = 2618032128U;
    unsigned int int4a = 0x1e6a2c48;
    EXPECT_EQ(int1a,Utl::bitReverseInt(int1));
    EXPECT_EQ(int2a,Utl::bitReverseInt(int2));
    EXPECT_EQ(int3a,Utl::bitReverseInt(int3));
    EXPECT_EQ(int4a,Utl::bitReverseInt(int4));
}

class UtlMapSerializerTest : public testing::Test {
public:
    UtlMapSerializerTest() : _file("UtlMapTest.properties") {}

    void SetUp() {
        _file.turf();
    }

    void TearDown() {
        _file.turf();
    }

    File _file;
};

class WhenUtlIsAskedToSerializeMapToFile : public UtlMapSerializerTest {
public:
    ~WhenUtlIsAskedToSerializeMapToFile() {}
};

TEST_F(WhenUtlIsAskedToSerializeMapToFile, shouldWritePropertiesToFile) {
    map<string, string> properties;
    properties.insert(pair<string, string>("foo", "bar"));
    properties.insert(pair<string, string>("bob", "spooge"));

    Utl::serializeMapToFile(properties, _file);

    ASSERT_TRUE(_file.exists());

    char buffer[_file.size()];
    ifstream istream(_file.absolutePath(), fstream::in);
    ASSERT_TRUE(istream);
    istream.read(buffer, sizeof(buffer));
    ASSERT_EQ(_file.size(), istream.gcount());
    istream.close();
    properties.clear();
    StringUtl::stringToMap(string(buffer), properties);

    ASSERT_TRUE(properties.end() != properties.find("foo"));
    ASSERT_STREQ("bar", properties.find("foo")->second.c_str());
    ASSERT_TRUE(properties.end() != properties.find("bob"));
    ASSERT_STREQ("spooge", properties.find("bob")->second.c_str());
}

class WhenUtlIsAskedToSerializeMapFromFile : public UtlMapSerializerTest {
public:
    ~WhenUtlIsAskedToSerializeMapFromFile() {}

    void SetUp() {
        _file.turf();
        ofstream ostream(_file.absolutePath(), fstream::out);
        if (false == ostream.is_open())
            FAIL();
        ostream <<"# This is a comment" << endl;
        ostream << "foo=bar" << endl;
        ostream << "freddy=zipplemier" << endl;
        ostream.close();
    }
};

TEST_F(WhenUtlIsAskedToSerializeMapFromFile, shouldLoadAllValuesFromFile) {
    map<string, string> properties;
    ASSERT_EQ(0, Utl::deserializeMapFromFile(properties, _file));
    ASSERT_TRUE(properties.end() != properties.find("foo"));
    ASSERT_STREQ("bar", properties.find("foo")->second.c_str());

    ASSERT_TRUE(properties.end() != properties.find("freddy"));
    ASSERT_STREQ("zipplemier", properties.find("freddy")->second.c_str());
}

static const string HEX("0102030A0BCC");
static const uint8_t BYTES[] = {0x01, 0x02, 0x03, 0x0A, 0x0B, 0xCC};

TEST(WhenUtlIsAskedForBytesForHexStringWhenAllDigitsAreValid, shouldConvertToBytes) {
    size_t len = HEX.size()/2;
    string buffer;

    EXPECT_EQ(len, Utl::hexStringAsBytes(string("0102030A0BCC"), buffer));
    EXPECT_EQ(0, ::memcmp(BYTES, buffer.c_str(), len));
    EXPECT_EQ(len, Utl::hexStringAsBytes(string("0102030a0bcc"), buffer));
    EXPECT_EQ(0, ::memcmp(BYTES, buffer.c_str(), len));
}

TEST(WhenUtlIsAskedForBytesForHexStringWhenTheStringIsEmpty, shouldReturnZero) {
    string buffer;
    EXPECT_EQ(0, Utl::hexStringAsBytes(string(""), buffer));
}

TEST(WhenUtlIsAskedForBytesForHexStringWithInvalidDigits, shouldNotConvertToBytes) {
    string buffer;
    EXPECT_EQ(-1, Utl::hexStringAsBytes("01K2030A0BCC", buffer));
    EXPECT_EQ(-1, Utl::hexStringAsBytes("01 2030A0BCC", buffer));
}

TEST(WhenUtlIsAskedForBytesForHexStringHasWrongDigitcount, shouldNotConvertToBytes) {
    string buffer;
    EXPECT_EQ(-1, Utl::hexStringAsBytes("0102030A0BC", buffer));
}

TEST(WhenUtlIsAskedForMacAddressForInterface, shouldReturnMacAddress) {
    string macAddr;
    cout << "MAC Address: " << Utl::macAddressForInterface(macAddr, "eth0") << endl;
}
