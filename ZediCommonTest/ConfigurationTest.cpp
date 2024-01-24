/** @file ConfigurationTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 29, 2013 by daver:
 * BugFix_ Jan 08, 2015 by eb:  add cJSON_Delete(_root) in Configuration test
 * @endhistory
 */

#include <Configuration.h>
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <log4cplus/configurator.h>
#include <cstring>
#include <cjson/cJSON.h>
#include <iostream>

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;

static const char* A_CONFIG =
        "{"
        "    \"StringValue\" : \"Test\","                   \
        "    \"IntValue\" : 234234,"                        \
        "    \"ReportDeliveryService\": {"                  \
        "        \"MaxBufferSize\": 2048,"                  \
        "        \"RetryStrategy\": {"                      \
        "            \"Name\": \"FixedInterval\","          \
        "            \"MaxRetries\": 3,"                    \
        "            \"SecondsBetweenRetries\": 30"         \
        "        },"                                        \
        "        \"Persistence\": {"                        \
        "            \"Directory\" : \"~/rptest\","         \
        "            \"MaxFileSize\" : 64000"               \
        "        }"                                         \
        "    },"                                            \
        "    \"Connector\" : {"                             \
        "        \"ServerAddress\": \"192.168.169.114\","   \
        "        \"RemotePort\": 3100,"                     \
        "        \"LocalPort\" : 3200,"                     \
        "        \"RcvBufferSize\": 4096,"                  \
        "        \"TrxBufferSize\": 4096,"                  \
        "        \"MaxPacketSize\": 2048,"                  \
        "        \"RetryStrategy\": {"                      \
        "            \"Name\": \"ProgressiveWait\","        \
        "            \"ShortIntervalRetries\": 3,"          \
        "            \"DefWaitTimeSeconds\": 30,"           \
        "            \"ProgressiveDelaySeconds\": 10,"      \
        "            \"MaxDelaySeconds\": 60,"              \
        "            \"MaxSanityThreshold\": 0"             \
        "        },"                                        \
        "        \"Authentication\" : {"                    \
        "           \"Username\" : \"ZGW-000001\","         \
        "           \"HashType\": {"                        \
        "               \"Name\" : \"SHA1\","               \
        "               \"IterationCount\":  1024"          \
        "           }"                                      \
        "        }"                                         \
        "    }"                                             \
        "}";

class ConfigurationTest: public testing::Test {
};

class StringConfigurationEx : public StringConfiguration {
public:
    StringConfigurationEx(const ConfigString& jsonString) : StringConfiguration(jsonString) {}
    ~StringConfigurationEx() {}

    bool isDirty() { return _dirty; }
};

TEST_F(ConfigurationTest, shouldTellMeIfKeyIsPresent) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.isKeyPresent("ReportDeliveryService:RetryStrategy"));
    ASSERT_FALSE(config.isKeyPresent("FooMcGoo"));
}

TEST_F(ConfigurationTest, shouldReturnJSONStruct) {
    StringConfigurationEx config(A_CONFIG);
    const cJSON* json = config.cjsonItemForKey("ReportDeliveryService:RetryStrategy");
    ASSERT_TRUE(json != NULL);
}
TEST_F(ConfigurationTest, shouldBeValidConfigString) {
    cJSON* root = cJSON_Parse(A_CONFIG);
    ASSERT_TRUE(root != NULL);
    cJSON_Delete(root);
}

TEST_F(ConfigurationTest, shouldSetStringValue) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.setStringValueForKey("FOO", "ReportDeliveryService:RetryStrategy:Name"));
    ASSERT_TRUE(config.isDirty());
    ASSERT_STREQ("FOO", config.valueAsString("ReportDeliveryService:RetryStrategy:Name", NULL));
}

TEST_F(ConfigurationTest, shouldCreateStringValueWhenAskToCreateOnSet) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.createStringKeyWithValue("ReportDeliveryService:RetryStrategy:Foo", "FOO"));
    ASSERT_TRUE(config.isDirty());
    ASSERT_TRUE(config.isKeyPresent("ReportDeliveryService:RetryStrategy:Foo"));
    ASSERT_STREQ("FOO", config.valueAsString("ReportDeliveryService:RetryStrategy:Foo", NULL));
}

TEST_F(ConfigurationTest, shouldSetRootStringValue) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.setStringValueForKey("FOO", "StringValue"));
    ASSERT_TRUE(config.isDirty());
    ASSERT_STREQ("FOO", config.valueAsString("StringValue", NULL));
}

TEST_F(ConfigurationTest, shouldSetRootIntValue) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.setIntValueForKey(100, "IntValue"));
    ASSERT_TRUE(config.isDirty());
    ASSERT_EQ(100, config.valueAsInt("IntValue", 0));
}

TEST_F(ConfigurationTest, shouldSetIntValue) {
    StringConfigurationEx config(A_CONFIG);
    ASSERT_TRUE(config.setIntValueForKey(100, "ReportDeliveryService:MaxBufferSize"));
    ASSERT_TRUE(config.isDirty());
    ASSERT_EQ(100, config.valueAsInt("ReportDeliveryService:MaxBufferSize", 0));
}

TEST_F(ConfigurationTest, shouldReturnDefaultStringValue) {
    StringConfiguration config(A_CONFIG);
    ASSERT_STREQ("Spooge", config.valueAsString("ReportDeliveryService:RetryStrategy:Foo", "Spooge"));
}

TEST_F(ConfigurationTest, shouldReturnDefaultIntValue) {
    StringConfiguration config(A_CONFIG);
    ASSERT_EQ(100, config.valueAsInt("ReportDeliveryService:Bar", 100));
}

TEST_F(ConfigurationTest, shouldReturnStringValue) {
    StringConfiguration config(A_CONFIG);
    ASSERT_STREQ("FixedInterval", config.valueAsString("ReportDeliveryService:RetryStrategy:Name", NULL));
}

TEST_F(ConfigurationTest, shouldReturnIntValue) {
    StringConfiguration config(A_CONFIG);
    ASSERT_EQ(2048, config.valueAsInt("ReportDeliveryService:MaxBufferSize", 0));
}

}
}
