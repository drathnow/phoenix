/** @file StringUtlTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 19, 2014 by eganb:
 * IncrDev Dec 10, 2015 by eb:    add test for new hexStringToUint
 * @endhistory
 */



#include "gtest/gtest.h"
#include "StringUtl.h"

using namespace zios::common;
//using namespace log4cplus;
using namespace std;

class StringUtilTest: public testing::Test {
protected:
};

TEST_F(StringUtilTest, shouldConvertFloatString) {
    string theString("11e-1");
    float theValue;
    ASSERT_TRUE(StringUtl::stringToFloat(theString, theValue));
    ASSERT_EQ(1.1F, theValue);
}


TEST_F(StringUtilTest, shouldConvertBool) {
     bool test32;
    // unrecognized string
    EXPECT_FALSE(StringUtl::stringToBool("abbab",test32));
    // true simple conversion
    EXPECT_TRUE(StringUtl::stringToBool("true",test32));
    EXPECT_TRUE(test32);
    EXPECT_TRUE(StringUtl::stringToBool("TRUE",test32));
    EXPECT_TRUE(test32);
    // true number
    EXPECT_TRUE(StringUtl::stringToBool("5",test32));
    EXPECT_TRUE(test32);
    // true, on
    EXPECT_TRUE(StringUtl::stringToBool("on",test32));
    EXPECT_TRUE(test32);
    EXPECT_TRUE(StringUtl::stringToBool("oN",test32));
    EXPECT_TRUE(test32);
    // true
    EXPECT_TRUE(StringUtl::stringToBool("false",test32));
    EXPECT_FALSE(test32);
    EXPECT_TRUE(StringUtl::stringToBool("fAlSe",test32));
    EXPECT_FALSE(test32);
    // true number
    EXPECT_TRUE(StringUtl::stringToBool("0",test32));
    EXPECT_FALSE(test32);
    // true, on
    EXPECT_TRUE(StringUtl::stringToBool("off",test32));
    EXPECT_FALSE(test32);
    EXPECT_TRUE(StringUtl::stringToBool("oFf",test32));
    EXPECT_FALSE(test32);
}

TEST_F(StringUtilTest, shouldConvertUint) {
    uint32_t test32;
    // false + not digit and not at 1st char
    EXPECT_FALSE(StringUtl::stringToUint("1+",test32));
    // false - not digit
    EXPECT_FALSE(StringUtl::stringToUint("-1",test32));
    // false a not digit
    EXPECT_FALSE(StringUtl::stringToUint("a1",test32));
    // false . not digit
    EXPECT_FALSE(StringUtl::stringToUint("1.7",test32));
    // true simple conversion
    EXPECT_TRUE(StringUtl::stringToUint("1",test32));
    EXPECT_EQ(1U,test32);
    // true big conversion
    EXPECT_TRUE(StringUtl::stringToUint("1234567890",test32));
    EXPECT_EQ(1234567890U,test32);

    //
    // 'splanation: on a 64 bit machine, it appears that ::strtoul has a personality disorder.  It does not
    // behave consitenty between a 64 bit machine and 32 bit machine...at least it does not behave the way
    // that the docs (depending on which one your read) describe.  For that reason, we only do these next tests
    // (and other in other places ) on 32 bit architectures
    //
#if __WORDSIZE == 32
    EXPECT_FALSE(StringUtl::stringToUint("+4294967296", test32));

    EXPECT_TRUE(StringUtl::stringToUint("  +0   ", test32));
    ASSERT_EQ(0, test32);
#endif
}

TEST_F(StringUtilTest, shouldConvertHexUint) {
    uint32_t test32;
    // false + not digit
    EXPECT_FALSE(StringUtl::hexStringToUint("+1",test32));
    // false - not digit
    EXPECT_FALSE(StringUtl::hexStringToUint("-1",test32));
    // true hex conversion
    EXPECT_TRUE(StringUtl::hexStringToUint("a1",test32));
    EXPECT_EQ(0xa1,test32);
    // false . not digit
    EXPECT_FALSE(StringUtl::hexStringToUint("1.7",test32));
    // true simple conversion
    EXPECT_TRUE(StringUtl::hexStringToUint("1",test32));
    EXPECT_EQ(1U,test32);
    // true simple conversion
    EXPECT_TRUE(StringUtl::hexStringToUint("0x12",test32));
    EXPECT_EQ(0x12,test32);

    //
    // false big conversion.  We skip this test on a 64 bit processor.
    //
    if (sizeof(unsigned long int) == 4) {
        EXPECT_FALSE(StringUtl::hexStringToUint("1234567890",test32));
    }
    // true no so big conversion
    EXPECT_TRUE(StringUtl::hexStringToUint("1234abef",test32));
    EXPECT_EQ(0x1234abef,test32);
    // true no so big conversion
    EXPECT_TRUE(StringUtl::hexStringToUint("0x1234abef",test32));
    EXPECT_EQ(0x1234abef,test32);
    // false no leading 0 infront of x.
    EXPECT_FALSE(StringUtl::hexStringToUint("x1234abef",test32));

}

TEST_F(StringUtilTest, shouldConvertInt) {
    int32_t testint32_t;
    // ok +digit with + in 1st position
    EXPECT_TRUE(StringUtl::stringToInt("+1",testint32_t));
    EXPECT_EQ(1,testint32_t);
    // ok - spaces removed
    EXPECT_TRUE(StringUtl::stringToInt("     +1        ",testint32_t));
    EXPECT_EQ(1,testint32_t);
    // ok -digit with - in 1st position
    EXPECT_TRUE(StringUtl::stringToInt("-1",testint32_t));
    EXPECT_EQ(-1,testint32_t);
    // ok - spaces/tab removed
    EXPECT_TRUE(StringUtl::stringToInt("   -1       \t",testint32_t));
    EXPECT_EQ(-1,testint32_t);
    // ok - spaces/tab/crlf removed
    EXPECT_TRUE(StringUtl::stringToInt("   -1       \r\n\t",testint32_t));
    EXPECT_EQ(-1,testint32_t);
    // ok  big negative conversion
    EXPECT_TRUE(StringUtl::stringToInt("-1234567890",testint32_t));
    EXPECT_EQ(-1234567890,testint32_t);
    EXPECT_FALSE(StringUtl::stringToInt("a1",testint32_t));
    EXPECT_FALSE(StringUtl::stringToInt("++1",testint32_t));
    EXPECT_FALSE(StringUtl::stringToInt("+1-",testint32_t));
    EXPECT_FALSE(StringUtl::stringToInt("--1",testint32_t));
    EXPECT_FALSE(StringUtl::stringToInt("-1.7",testint32_t));
}

TEST_F(StringUtilTest, shouldConvertFloat) {
    float testFloat;
    // ok +digit with + in 1st position
    EXPECT_TRUE(StringUtl::stringToFloat("+1",testFloat));
    EXPECT_FLOAT_EQ(1,testFloat);
    // ok -digit with - in 1st position
    EXPECT_TRUE(StringUtl::stringToFloat("-1",testFloat));
    EXPECT_FLOAT_EQ(-1,testFloat);
    // ok  big negative conversion
    EXPECT_TRUE(StringUtl::stringToFloat("-1234567890",testFloat));
    EXPECT_FLOAT_EQ(-1234567890,testFloat);
    EXPECT_FALSE(StringUtl::stringToFloat("a1",testFloat));
    EXPECT_FALSE(StringUtl::stringToFloat("++1",testFloat));
    EXPECT_FALSE(StringUtl::stringToFloat("+1-",testFloat));
    EXPECT_FALSE(StringUtl::stringToFloat("--1",testFloat));
    EXPECT_TRUE(StringUtl::stringToFloat("-1.7",testFloat));
    EXPECT_FLOAT_EQ(-1.7,testFloat);
    EXPECT_TRUE(StringUtl::stringToFloat("+0.000000007",testFloat));
    EXPECT_FLOAT_EQ(0.000000007,testFloat);


    EXPECT_TRUE(StringUtl::stringToFloat("+177773.7258", testFloat));
    EXPECT_TRUE(StringUtl::stringToFloat("  +0   ", testFloat));
    EXPECT_FALSE(StringUtl::stringToFloat("+100g7.876%$t", testFloat));
}

TEST_F(StringUtilTest, shouldTrim) {
    string str;
    // right trim of space/whitespace
    string str1 = "  wer \tet  \t\r\n";
    str = StringUtl::rtrim(str1);
    EXPECT_EQ("  wer \tet",str);
    // right trim of whitespace/space
    str1 = "  wer \tet\t\r\n    ";
    str = StringUtl::rtrim(str1);
    EXPECT_EQ("  wer \tet",str);

    //ltrim of whitespace
    str1 = "\t\r\nweret\t\r";
    str = StringUtl::ltrim(str1);
    EXPECT_EQ("weret\t\r",str);

    //ltrim of space/whitespace
    str1 = "  \n\tweret\t\r";
    str = StringUtl::ltrim(str1);
    EXPECT_EQ("weret\t\r",str);

    // both end trim (leave whitespace in middle)
    str1 = "\t \r\nwer\n et\t\r";
    str = StringUtl::trim(str1);
    EXPECT_EQ("wer\n et",str);
}

TEST_F(StringUtilTest, shouldCreateStringArrayFromEmptyList) {
    std::vector<std::string> input;
    char **output = NULL;

    ASSERT_EQ(input.size(), 0);

    output = StringUtl::createStringArray(input);

    ASSERT_TRUE(output != NULL);
    ASSERT_EQ(NULL, output[0]);

    StringUtl::freeStringArray(output);
}

TEST_F(StringUtilTest, shouldCreateStringArrayFromList) {
    std::vector<std::string> input;
    char **output = NULL;

    input.push_back("one");
    input.push_back("two");
    input.push_back("three");

    ASSERT_EQ(input.size(), 3);

    output = StringUtl::createStringArray(input);

    ASSERT_TRUE(output != NULL);
    ASSERT_EQ(0, strcmp("one", output[0]));
    ASSERT_EQ(0, strcmp("two", output[1]));
    ASSERT_EQ(0, strcmp("three", output[2]));
    ASSERT_EQ(NULL, output[3]);

    StringUtl::freeStringArray(output);
}

TEST_F(StringUtilTest, shouldConvertIntToString) {
    EXPECT_EQ("0", StringUtl::intToString(0));
    EXPECT_EQ("1", StringUtl::intToString(1));
    EXPECT_EQ("2147483647", StringUtl::intToString(2147483647));
    EXPECT_EQ("-1", StringUtl::intToString(-1));
    EXPECT_EQ("-2147483647", StringUtl::intToString(-2147483647));
}

TEST_F(StringUtilTest, shouldConvertUIntToString) {
    EXPECT_EQ("0", StringUtl::uintToString(0));
    EXPECT_EQ("1", StringUtl::uintToString(1));
    EXPECT_EQ("2147483647", StringUtl::uintToString(2147483647));
    EXPECT_EQ("4294967295", StringUtl::uintToString(4294967295U));
}

TEST_F(StringUtilTest, shouldConvertEmptyStringToEmptyMap) {
    map<string,string> *result = StringUtl::stringToMap("");
    ASSERT_NE((map<string,string>*)NULL, result);
    EXPECT_EQ(0, result->size());
    delete result;
}

TEST_F(StringUtilTest, shouldConvertStringToMap) {
    map<string,string> *result = StringUtl::stringToMap("a=123\nb=456");
    ASSERT_NE((map<string,string>*)NULL, result);
    EXPECT_EQ(2, result->size());
    EXPECT_EQ("123", result->operator[]("a"));
    EXPECT_EQ("456", result->operator[]("b"));
    delete result;
}

TEST_F(StringUtilTest, shouldConvertStringAndReturnInProvidedMap) {
    map<string,string> result;
    StringUtl::stringToMap("a=123\nb=456", result);
    EXPECT_EQ(2, result.size());
    EXPECT_EQ("123", result.operator[]("a"));
    EXPECT_EQ("456", result.operator[]("b"));
}


TEST_F(StringUtilTest, shouldConvertStringWithSpacesToMap) {
    map<string,string> *result = StringUtl::stringToMap(" a = 123 \n  b =  4 5 6 \n");
    ASSERT_NE((map<string,string>*)NULL, result);
    EXPECT_EQ(2, result->size());
    EXPECT_EQ("123", result->operator[]("a"));
    EXPECT_EQ("4 5 6", result->operator[]("b"));
    delete result;
}

TEST_F(StringUtilTest, shouldConvertStringToMapIgnoringInvalidLines) {
    map<string,string> *result = StringUtl::stringToMap("a=123\nbad\n\n\nb=456\n");
    ASSERT_NE((map<string,string>*)NULL, result);
    EXPECT_EQ(2, result->size());
    EXPECT_EQ("123", result->operator[]("a"));
    EXPECT_EQ("456", result->operator[]("b"));
    delete result;
}

TEST_F(StringUtilTest, shouldConvertStringToMapIgnoringCommentLines) {
    map<string,string> *result = StringUtl::stringToMap("a=123\n#c=789\n#commentline\n\n\nb=456\n");
    ASSERT_NE((map<string,string>*)NULL, result);
    EXPECT_EQ(2, result->size());
    EXPECT_EQ("123", result->operator[]("a"));
    EXPECT_EQ("456", result->operator[]("b"));
    delete result;
}

TEST_F(StringUtilTest, shouldSplitStringToVector) {
    vector<string> *result = StringUtl::stringToVector("  one, two ,a b c, ", ',', true);
    ASSERT_NE((vector<string>*)NULL, result);
    EXPECT_EQ(4, result->size());
    EXPECT_EQ("one", result->operator[](0));
    EXPECT_EQ("two", result->operator[](1));
    EXPECT_EQ("a b c", result->operator[](2));
    EXPECT_EQ("", result->operator[](3));
    delete result;
}

TEST_F(StringUtilTest, shouldSplitStringToVectorContainingEmptyStrings) {
    vector<string> *result = StringUtl::stringToVector(",,,", ',', true);
    ASSERT_NE((vector<string>*)NULL, result);
    EXPECT_EQ(4, result->size());
    EXPECT_EQ("", result->operator[](0));
    EXPECT_EQ("", result->operator[](1));
    EXPECT_EQ("", result->operator[](2));
    EXPECT_EQ("", result->operator[](3));
    delete result;
}

TEST_F(StringUtilTest, shouldSplitStringToVectorIgnoringComments) {
    vector<string> *result = StringUtl::stringToVector("# this is a comment", ',', true);
    ASSERT_NE((vector<string>*)NULL, result);
    EXPECT_EQ(0, result->size());
    delete result;
}

TEST_F(StringUtilTest, shouldSplitStringToVectorNotIgnoringCommentsWhenNotEnabled) {
    vector<string> *result = StringUtl::stringToVector("# this is a comment", ',', false);
    ASSERT_NE((vector<string>*)NULL, result);
    EXPECT_EQ(1, result->size());
    delete result;
}

TEST_F(StringUtilTest, shouldSplitStringToVectorIgnoringEmptyString) {
    vector<string> *result = StringUtl::stringToVector("  ", ',', true);
    ASSERT_NE((vector<string>*)NULL, result);
    EXPECT_EQ(0, result->size());
    delete result;
}

TEST(WhenStringUtlIsAskedToConvertNumericStringTo64BitLong, shouldConvertString) {
    uint64_t longNumber;

    ASSERT_TRUE(StringUtl::stringToU64("12345", longNumber));
    ASSERT_EQ(12345L, longNumber);
    ASSERT_TRUE(StringUtl::stringToU64("4294967296", longNumber));
    ASSERT_EQ(4294967296LL, longNumber);
}

TEST(WhenStringUtlIsAskedConvertAStreamToMap, shouldConvertStringsInStream)
{
    ostringstream oss;
    oss << "key1=value1" << endl;
    oss << "key2=value2" << endl;
    oss << "key3=value3" << endl;

    istringstream iss(oss.str());
    map<string, string> kvMap;

    ASSERT_EQ(3, StringUtl::streamToMap(iss, kvMap));
    ASSERT_TRUE(kvMap.find("key1") != kvMap.end());
    ASSERT_STREQ("value1", kvMap.find("key1")->second.c_str());

    ASSERT_TRUE(kvMap.find("key2") != kvMap.end());
    ASSERT_STREQ("value2", kvMap.find("key2")->second.c_str());

    ASSERT_TRUE(kvMap.find("key3") != kvMap.end());
    ASSERT_STREQ("value3", kvMap.find("key3")->second.c_str());

}
