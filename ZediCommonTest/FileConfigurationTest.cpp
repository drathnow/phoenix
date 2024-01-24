/** @file FileConfiguration.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 19, 2014 by daver:
 * BugFix_ Jan 08, 2015 by eb:  add cJSON_Delete(_root) in Configuration test
 * @endhistory
 */

#include <Configuration.h>
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <log4cplus/configurator.h>
#include <cjson/cJSON.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include "commonmocks.h"

#include "MockNotificationCenter.h"

namespace zios {
namespace common_tests {

using namespace std;
using namespace zios::common;

using testing::_;
using testing::ByRef;
using testing::Eq;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::MakeMatcher;
using ::testing::Matcher;

static const char* A_CONFIG =
        "{"
                "    \"ReportDeliveryService\": {"
                "        \"MaxBufferSize\": 2048,"
                "        \"RetryStrategy\": {"
                "            \"Name\": \"FixedInterval\","
                "            \"MaxRetries\": 3,"
                "            \"SecondsBetweenRetries\": 30"
                "        },"
                "        \"Persistence\": {"
                "            \"Direcotry\" : \"~/rptest\","
                "            \"MaxFileSize\" : 64000"
                "        }"
                "    },"
                "    \"Connector\" : {"
                "        \"ServerAddress\": \"192.168.169.114\","
                "        \"RemotePort\": 3100,"
                "        \"LocalPort\" : 3200,"
                "        \"RcvBufferSize\": 4096,"
                "        \"TrxBufferSize\": 4096,"
                "        \"MaxPacketSize\": 2048,"
                "        \"ConnectionRetryStrategy\": {"
                "            \"Name\": \"ProgressiveWait\","
                "            \"ShortIntervalRetries\": 3,"
                "            \"DefWaitTimeSeconds\": 30,"
                "            \"ProgressiveDelaySeconds\": 10,"
                "            \"MaxDelaySeconds\": 60,"
                "            \"MaxSanityThreshold\": 0"
                "        },"
                "        \"Authentication\" : {"
                "           \"Username\" : \"ZGW-000001\","
                "           \"HashType\": {"
                "               \"Name\" : \"SHA1\","
                "               \"IterationCount\":  1024"
                "           }"
                "        }"
                "    }"
                "}";

class FileConfigurationTest: public testing::Test
{
public:
    void SetUp()
    {
        system("mkdir ./FileConfigurationTest");
        ofstream outfile("./FileConfigurationTest/config.json", ios_base::trunc);
        outfile.write(A_CONFIG, ::strlen(A_CONFIG));
        outfile.close();
    }

    void TearDown()
    {
        system("rm -rf ./FileConfigurationTest");
    }
};

class FileConfigurationEx: public FileConfiguration
{
public:
    FileConfigurationEx(NotificationCenter& notificationCenter, ConfigurationFileManager& fileManager) :
            FileConfiguration(notificationCenter, fileManager)
    {
    }
    virtual ~FileConfigurationEx()
    {
    }

    bool isDirty() const
    {
        return _dirty;
    }
};

TEST_F(FileConfigurationTest, shouldSaveFileAndPostNotificationIfConfigChanged)
{
    MockNotificationCenter nc;
    File file("./FileConfigurationTest/config.json");
    ConfigurationFileManager fileManager(file, 3);
    FileConfigurationEx config(nc, fileManager);

    config.setIntValueForKey(100, "ReportDeliveryService:MaxBufferSize");
    ASSERT_TRUE(config.isDirty());
    ASSERT_TRUE(config.save());
    ASSERT_FALSE(config.isDirty());

    FileConfiguration newConfig(nc, fileManager);
    ASSERT_EQ(100, newConfig.valueAsInt("ReportDeliveryService:MaxBufferSize", 0));
}

TEST_F(FileConfigurationTest, shouldNotSaveFileAndNotPostNotificationIfConfigHasntChanged)
{
    MockNotificationCenter nc;
    File file("./FileConfigurationTest/config.json");
    ConfigurationFileManager fileManager(file, 3);
    FileConfigurationEx config(nc, fileManager);

    EXPECT_CALL(nc, postNotification(Configuration::CONFIG_CHANGE_NOTIFICATION_NAME, NULL)).Times(0);
    ASSERT_FALSE(config.isDirty());
    ASSERT_TRUE(config.save());
    ASSERT_FALSE(config.isDirty());
}

TEST_F(FileConfigurationTest, shouldBeValidConfigString)
{
    cJSON* root = cJSON_Parse(A_CONFIG);
    ASSERT_TRUE(root != NULL);
    cJSON_Delete(root);

}

TEST_F(FileConfigurationTest, shouldReturnValue)
{
    MockNotificationCenter nc;
    File file("./FileConfigurationTest/config.json");
    ConfigurationFileManager fileManager(file, 3);
    FileConfiguration config(nc, fileManager);
    ASSERT_STREQ("FixedInterval", config.valueAsString("ReportDeliveryService:RetryStrategy:Name", NULL));
}

}
}
