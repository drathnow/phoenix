#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <map>
#include <AlarmConfiguration.h>
#include <AlarmConfigurationSerializer.h>

namespace zios::domain
{

using namespace std;

using testing::Mock;


class AlarmConfigurationSerializerTest: public testing::Test
{
public:
    static const uint16_t SET_TIME;
    static const uint16_t CLEAR_TIME;
    static const uint32_t LOW_LOW_SET_LIMIT;
    static const uint32_t LOW_LOW_CLEAR_LIMIT;
    static const uint32_t LOW_SET_LIMIT;
    static const uint32_t LOW_CLEAR_LIMIT;
    static const uint32_t HIGH_HIGH_SET_LIMIT;
    static const uint32_t HIGH_HIGH_CLEAR_LIMIT;
    static const uint32_t HIGH_SET_LIMIT;
    static const uint32_t HIGH_CLEAR_LIMIT;

    static const string MASK_STR;
    static const string SET_TIME_STR;
    static const string CLEAR_TIME_STR;
    static const string LOW_LOW_SET_LIMIT_STR;
    static const string LOW_LOW_CLEAR_LIMIT_STR;
    static const string LOW_SET_LIMIT_STR;
    static const string LOW_CLEAR_LIMIT_STR;
    static const string HIGH_HIGH_SET_LIMIT_STR;
    static const string HIGH_HIGH_CLEAR_LIMIT_STR;
    static const string HIGH_SET_LIMIT_STR;
    static const string HIGH_CLEAR_LIMIT_STR;

    AlarmConfigurationSerializerTest() = default;
    ~AlarmConfigurationSerializerTest() = default;
};

const uint16_t AlarmConfigurationSerializerTest::SET_TIME = 100;
const uint16_t AlarmConfigurationSerializerTest::CLEAR_TIME = 50;
const uint32_t AlarmConfigurationSerializerTest::LOW_LOW_SET_LIMIT = 30;
const uint32_t AlarmConfigurationSerializerTest::LOW_LOW_CLEAR_LIMIT = 40;
const uint32_t AlarmConfigurationSerializerTest::LOW_SET_LIMIT = 50;
const uint32_t AlarmConfigurationSerializerTest::LOW_CLEAR_LIMIT = 60;
const uint32_t AlarmConfigurationSerializerTest::HIGH_HIGH_SET_LIMIT = 100;
const uint32_t AlarmConfigurationSerializerTest::HIGH_HIGH_CLEAR_LIMIT = 90;
const uint32_t AlarmConfigurationSerializerTest::HIGH_SET_LIMIT = 80;
const uint32_t AlarmConfigurationSerializerTest::HIGH_CLEAR_LIMIT = 70;

const string AlarmConfigurationSerializerTest::MASK_STR = "15";
const string AlarmConfigurationSerializerTest::SET_TIME_STR = "100";
const string AlarmConfigurationSerializerTest::CLEAR_TIME_STR = "50";
const string AlarmConfigurationSerializerTest::LOW_LOW_SET_LIMIT_STR = "30";
const string AlarmConfigurationSerializerTest::LOW_LOW_CLEAR_LIMIT_STR = "40";
const string AlarmConfigurationSerializerTest::LOW_SET_LIMIT_STR = "50";
const string AlarmConfigurationSerializerTest::LOW_CLEAR_LIMIT_STR = "60";
const string AlarmConfigurationSerializerTest::HIGH_HIGH_SET_LIMIT_STR = "100";
const string AlarmConfigurationSerializerTest::HIGH_HIGH_CLEAR_LIMIT_STR = "90";
const string AlarmConfigurationSerializerTest::HIGH_SET_LIMIT_STR = "80";
const string AlarmConfigurationSerializerTest::HIGH_CLEAR_LIMIT_STR = "70";



class WhenAlarmConfigurationSerializerIsAskedToSerializeToMap: public AlarmConfigurationSerializerTest
{
public:
    WhenAlarmConfigurationSerializerIsAskedToSerializeToMap()
    {
    }
    ~WhenAlarmConfigurationSerializerIsAskedToSerializeToMap()
    {
    }
};

TEST_F(WhenAlarmConfigurationSerializerIsAskedToSerializeToMap, shouldSerializeToMap)
{
    map<string, string> serializeMap;
    AlarmConfiguration<int32_t> alarmConfig;
    alarmConfig.setClearTime(CLEAR_TIME);
    alarmConfig.setSetTime(SET_TIME);

    alarmConfig.setLowLimits(LOW_SET_LIMIT, LOW_CLEAR_LIMIT);
    alarmConfig.setLowLowLimits(LOW_LOW_SET_LIMIT, LOW_LOW_CLEAR_LIMIT);
    alarmConfig.setHighLimits(HIGH_SET_LIMIT, HIGH_CLEAR_LIMIT);
    alarmConfig.setHighHighLimits(HIGH_HIGH_SET_LIMIT, HIGH_HIGH_CLEAR_LIMIT);

    AlarmConfigurationSerializer::serialize(alarmConfig, serializeMap);

    map<string, string>::const_iterator iter = serializeMap.find(MASK_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(MASK_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(SET_TIME_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(SET_TIME_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(SET_TIME_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(SET_TIME_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(LOW_LOW_SET_LIMIT_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(LOW_LOW_SET_LIMIT_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(LOW_LOW_CLEAR_LIMIT_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(HIGH_HIGH_SET_LIMIT_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT_STR.c_str(), iter->second.c_str());

    iter = serializeMap.find(HIGH_HIGH_CLEAR_LIMIT_KEY);
    ASSERT_TRUE(serializeMap.end() != iter);
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT_STR.c_str(), iter->second.c_str());
}

class WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap: public AlarmConfigurationSerializerTest
{
public:
    WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap()
    {
    }
    ~WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap()
    {
    }

    void SetUp()
    {
        serializeMap[MASK_KEY] = MASK_STR;
        serializeMap[SET_TIME_KEY] = SET_TIME_STR;
        serializeMap[CLEAR_TIME_KEY] = CLEAR_TIME_STR;
        serializeMap[LOW_SET_LIMIT_KEY] = LOW_SET_LIMIT_STR;
        serializeMap[LOW_CLEAR_LIMIT_KEY] = LOW_CLEAR_LIMIT_STR;
        serializeMap[LOW_LOW_SET_LIMIT_KEY] = LOW_LOW_SET_LIMIT_STR;
        serializeMap[LOW_LOW_CLEAR_LIMIT_KEY] = LOW_LOW_CLEAR_LIMIT_STR;
        serializeMap[HIGH_SET_LIMIT_KEY] = HIGH_SET_LIMIT_STR;
        serializeMap[HIGH_CLEAR_LIMIT_KEY] = HIGH_CLEAR_LIMIT_STR;
        serializeMap[HIGH_HIGH_SET_LIMIT_KEY] = HIGH_HIGH_SET_LIMIT_STR;
        serializeMap[HIGH_HIGH_CLEAR_LIMIT_KEY] = HIGH_HIGH_CLEAR_LIMIT_STR;
    }

    bool didEraseKeyFromMap(const string &key)
    {
        return serializeMap.erase(key) > 0;
    }

    AlarmConfiguration<int32_t> alarmConfiguration;
    map<string, string> serializeMap;
};

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldRestoreConfiguration)
{
    auto *alarmConfig = AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap);

    ASSERT_TRUE(NULL != alarmConfig);
    ASSERT_EQ(SET_TIME, alarmConfig->setTimeSeconds());
    ASSERT_EQ(CLEAR_TIME, alarmConfig->clearTimeSeconds());
    ASSERT_EQ(LOW_SET_LIMIT, alarmConfig->lowSetLimit());
    ASSERT_EQ(LOW_CLEAR_LIMIT, alarmConfig->lowClearLimit());
    ASSERT_EQ(LOW_LOW_SET_LIMIT, alarmConfig->lowLowSetLimit());
    ASSERT_EQ(LOW_LOW_CLEAR_LIMIT, alarmConfig->lowLowClearLimit());
    ASSERT_EQ(HIGH_SET_LIMIT, alarmConfig->highSetLimit());
    ASSERT_EQ(HIGH_CLEAR_LIMIT, alarmConfig->highClearLimit());
    ASSERT_EQ(HIGH_HIGH_SET_LIMIT, alarmConfig->highHighSetLimit());
    ASSERT_EQ(HIGH_HIGH_CLEAR_LIMIT, alarmConfig->highHighClearLimit());
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingMaskAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(MASK_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingSetTimeAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(SET_TIME_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingClearTimeAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(CLEAR_TIME_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingLowSetLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(LOW_SET_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingLowClearLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(LOW_CLEAR_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingLowLowSetLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(LOW_LOW_SET_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingLowLowClearLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(LOW_LOW_CLEAR_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingHighSetLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(HIGH_SET_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingHighCleartLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(HIGH_CLEAR_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingHighHighSetLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(HIGH_HIGH_SET_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

TEST_F(WhenAlarmConfigurationSerializerIsAskedToDeserializeFromMap, shouldDetectMissingHighHighClearLimitAndReturnInvalidAlarmSetting)
{
    ASSERT_TRUE(didEraseKeyFromMap(HIGH_HIGH_CLEAR_LIMIT_KEY));
    ASSERT_TRUE(NULL == AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfiguration, serializeMap));
}

}
