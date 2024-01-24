/** @file base64Test.cpp
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 1, 2015 by eganb:
 * @endhistory
 */
#include "Utl.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ctime>
#include "commonmocks.h"

namespace zios {
namespace common_tests {

using ::testing::Return;

using namespace std;
using namespace zios::common;

class Base64Test : public ::testing::Test {
public:

};

class WhenBase64IsAskedToEncodeByteArray : public Base64Test {
public:
    static const string TEST_BYTES;
    static const string EXPECTED_STRING;

};
const string WhenBase64IsAskedToEncodeByteArray::TEST_BYTES = "Hi, how are you?  I am fine, thanks for asking. this is the end.\n\n";
const string WhenBase64IsAskedToEncodeByteArray::EXPECTED_STRING = "SGksIGhvdyBhcmUgeW91PyAgSSBhbSBmaW5lLCB0aGFua3MgZm9yIGFza2luZy4gdGhpcyBpcyB0aGUgZW5kLgoK";

TEST_F(WhenBase64IsAskedToEncodeByteArray, shouldEncodeToBase64String) {
    string encodedString;
    Utl::base64_encode((const uint8_t*)TEST_BYTES.data(), TEST_BYTES.size(), encodedString);
    EXPECT_STREQ(EXPECTED_STRING.c_str(), encodedString.c_str());

    vector<uint8_t> result = Utl::base64_decode(encodedString);
    EXPECT_EQ(TEST_BYTES.size(), result.size());
    if (TEST_BYTES.size() == result.size()){
      for (size_t i = 0; i < result.size(); i ++)
          EXPECT_EQ(TEST_BYTES[i],result[i]) << " at posistion:" << i;
    }

}


class WhenBase64IsAskedToDecodeABase64EncodedString : public Base64Test {
public:
    static const string TEST_STRING;
    static const string EXPECTED_BYTES;
};
const string WhenBase64IsAskedToDecodeABase64EncodedString::EXPECTED_BYTES = "Hi, how are you?  I am fine, thanks for asking. this is the end.\n\n";
const string WhenBase64IsAskedToDecodeABase64EncodedString::TEST_STRING = "SGksIGhvdyBhcmUgeW91PyAgSSBhbSBmaW5lLCB0aGFua3MgZm9yIGFza2luZy4gdGhpcyBpcyB0aGUgZW5kLgoK";

TEST_F(WhenBase64IsAskedToDecodeABase64EncodedString, simpleTextDecode) {
    vector<uint8_t> result = Utl::base64_decode(TEST_STRING);
    EXPECT_EQ(EXPECTED_BYTES.length(), result.size());
    if (EXPECTED_BYTES.length() == result.size()){
      for (size_t i = 0; i < result.size(); i ++)
          EXPECT_EQ(EXPECTED_BYTES[i],result[i]) << " at posistion:" << i;
    }
}


TEST_F(WhenBase64IsAskedToDecodeABase64EncodedString, simplebinaryDecode) {
    uint8_t seed[65] = { 0,
            1,2,3,4,5,6,7,8,9,10,
            11,12,13,14,15,16,17,18,19,20,
            21,22,23,24,25,26,27,28,29,30,
            31,32,33,34,35,36,37,38,39,40,
            41,42,43,44,45,46,47,48,49,50,
            51,52,53,54,55,56,57,58,59,60,
            61,62,63,64
    };
    vector<uint8_t> resultv(&seed[0],&seed[64]+1);
    string testString = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4\
OTo7PD0+P0A=";

    vector<uint8_t> result = Utl::base64_decode(testString);
    EXPECT_EQ(resultv.size(),result.size());
    if (resultv.size() == result.size()){
      for (size_t i = 0; i < result.size(); i ++)
          EXPECT_EQ(resultv[i],result[i]) << " at position:" << i;
    }


}


}}



