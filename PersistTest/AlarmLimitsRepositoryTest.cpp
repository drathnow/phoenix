#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <AlarmLimitsRepository.h>
#include <persist.h>

#include "OrmBaseTest.h"

#define IO_POINT_ID                 42
#define SET_TIME                    100
#define CLEAR_TIME                  200
#define NO_DATA_ENABLED             1
#define HIGH_HIGH_SET_LIMIT         "200"
#define HIGH_HIGH_CLEAR_LIMIT       "175"
#define HIGH_SET_LIMIT              "150"
#define HIGH_CLEAR_LIMIT            "125"
#define LOW_SET_LIMIT               "50"
#define LOW_CLEAR_LIMIT             "40"
#define LOW_LOW_SET_LIMIT           "30"
#define LOW_LOW_CLEAR_LIMIT         "20"

namespace dios::persist
{

static int alarmLimitsRowCollater(void *pArg, int argc, char **values, char **columnNames)
{
    if (NULL != pArg)
    {
        alarm_limits_t *alarmLimits = (alarm_limits_t*) pArg;
        for (int i = 0; i < argc; i++)
        {
            if (0 == ::strcasecmp("oid", columnNames[i]))
            {
                alarmLimits->oid = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("io_point_id", columnNames[i]))
            {
                alarmLimits->io_point_id = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("set_time", columnNames[i]))
            {
                alarmLimits->set_time_seconds = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("clear_time", columnNames[i]))
            {
                alarmLimits->clear_time_seconds = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("no_data_enabled", columnNames[i]))
            {
                alarmLimits->no_data_enabled = atoi(values[i]) == 0 ? false : true;
                continue;
            }
            if (0 == ::strcasecmp("high_high_set_limit", columnNames[i]))
            {
                alarmLimits->high_high_set_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("high_high_clear_limit", columnNames[i]))
            {
                alarmLimits->high_high_clear_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("high_set_limit", columnNames[i]))
            {
                alarmLimits->high_set_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("high_clear_limit", columnNames[i]))
            {
                alarmLimits->high_clear_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("low_low_set_limit", columnNames[i]))
            {
                alarmLimits->low_low_set_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("low_low_clear_limit", columnNames[i]))
            {
                alarmLimits->low_low_clear_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("low_set_limit", columnNames[i]))
            {
                alarmLimits->low_set_limit = values[i];
                continue;
            }
            if (0 == ::strcasecmp("low_clear_limit", columnNames[i]))
            {
                alarmLimits->low_clear_limit = values[i];
                continue;
            }
        }
        return 0;
    }

    return -1;
}

class AlarmLimitsRepositoryCreateTest: public OrmBaseTest
{
public:
    AlarmLimitsRepositoryCreateTest() :
            OrmBaseTest("AlarmLimitsRepositoryCreateTest.db")
    {
    }
    ~AlarmLimitsRepositoryCreateTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        alarmLimits.io_point_id = IO_POINT_ID;
        alarmLimits.set_time_seconds = SET_TIME;
        alarmLimits.clear_time_seconds = CLEAR_TIME;
        alarmLimits.high_high_set_limit = HIGH_HIGH_SET_LIMIT;
        alarmLimits.high_high_clear_limit = HIGH_HIGH_CLEAR_LIMIT;
        alarmLimits.high_set_limit = HIGH_SET_LIMIT;
        alarmLimits.high_clear_limit = HIGH_CLEAR_LIMIT;
        alarmLimits.low_low_set_limit = LOW_LOW_SET_LIMIT;
        alarmLimits.low_low_clear_limit = LOW_LOW_CLEAR_LIMIT;
        alarmLimits.low_set_limit = LOW_SET_LIMIT;
        alarmLimits.low_clear_limit = LOW_CLEAR_LIMIT;
    }

    alarm_limits_t alarmLimits;
};

TEST_F(AlarmLimitsRepositoryCreateTest, shouldCreateAlarmLimits)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    int64_t resultId = alarmLimitsRepositoryUnderTest.createAlarmLimits(alarmLimits);
    ASSERT_TRUE(resultId > 0) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(resultId, foundAlarmLimits.oid);
    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

class AlarmLimitsRepositoryUpdateTest: public OrmBaseTest
{
public:
    AlarmLimitsRepositoryUpdateTest() :
            OrmBaseTest("AlarmLimitsRepositoryUpdateTest.db")
    {
    }
    ~AlarmLimitsRepositoryUpdateTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        alarmLimits.io_point_id = IO_POINT_ID;
        alarmLimits.set_time_seconds = SET_TIME;
        alarmLimits.clear_time_seconds = CLEAR_TIME;
        alarmLimits.high_high_set_limit = HIGH_HIGH_SET_LIMIT;
        alarmLimits.high_high_clear_limit = HIGH_HIGH_CLEAR_LIMIT;
        alarmLimits.high_set_limit = HIGH_SET_LIMIT;
        alarmLimits.high_clear_limit = HIGH_CLEAR_LIMIT;
        alarmLimits.low_low_set_limit = LOW_LOW_SET_LIMIT;
        alarmLimits.low_low_clear_limit = LOW_LOW_CLEAR_LIMIT;
        alarmLimits.low_set_limit = LOW_SET_LIMIT;
        alarmLimits.low_clear_limit = LOW_CLEAR_LIMIT;

        AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

        alarmLimits.oid = alarmLimitsRepositoryUnderTest.createAlarmLimits(alarmLimits);

    }

    alarm_limits_t alarmLimits;
};

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateSetTime)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.set_time_seconds = SET_TIME + 100;
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME+100, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateClearTime)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.clear_time_seconds = CLEAR_TIME + 100;
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME+100, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}


TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateHighHighSetLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.high_high_set_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ("200.123", foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateHighHighClearLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.high_high_clear_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ("175.123", foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateHighSetLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.high_set_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ("150.123", foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateHighClearLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.high_clear_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ("125.123", foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateLowLowSetLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.low_low_set_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ("30.123", foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateLowLowClearLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.low_low_clear_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ("20.123", foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateLowSetLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.low_set_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ("50.123", foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}

TEST_F(AlarmLimitsRepositoryUpdateTest, shouldUpdateLowClearLimit)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    alarmLimits.low_clear_limit.append(".123");
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.updateAlarmLimits(alarmLimits)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));

    alarm_limits_t foundAlarmLimits;
    executeCommandInContext("select * from AlarmLimits;", alarmLimitsRowCollater, &foundAlarmLimits);

    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ("40.123", foundAlarmLimits.low_clear_limit.c_str());
}

class AlarmLimitsRepositoryDeleteTest: public OrmBaseTest
{
public:
    AlarmLimitsRepositoryDeleteTest() :
            OrmBaseTest("AlarmLimitsRepositoryDeleteTest.db")
    {
    }
    ~AlarmLimitsRepositoryDeleteTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        alarmLimits.io_point_id = IO_POINT_ID;
        alarmLimits.set_time_seconds = SET_TIME;
        alarmLimits.clear_time_seconds = CLEAR_TIME;
        alarmLimits.high_high_set_limit = HIGH_HIGH_SET_LIMIT;
        alarmLimits.high_high_clear_limit = HIGH_HIGH_CLEAR_LIMIT;
        alarmLimits.high_set_limit = HIGH_SET_LIMIT;
        alarmLimits.high_clear_limit = HIGH_CLEAR_LIMIT;
        alarmLimits.low_low_set_limit = LOW_LOW_SET_LIMIT;
        alarmLimits.low_low_clear_limit = LOW_LOW_CLEAR_LIMIT;
        alarmLimits.low_set_limit = LOW_SET_LIMIT;
        alarmLimits.low_clear_limit = LOW_CLEAR_LIMIT;

        AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

        alarmLimits.oid = alarmLimitsRepositoryUnderTest.createAlarmLimits(alarmLimits);

    }

    alarm_limits_t alarmLimits;
};

TEST_F(AlarmLimitsRepositoryDeleteTest, shouldDeleteAlarmLimits)
{
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    ASSERT_EQ(1, rowCountInTable("AlarmLimits"));
    ASSERT_EQ(0, alarmLimitsRepositoryUnderTest.deleteAlarmLimitsWithOid(alarmLimits.oid)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(0, rowCountInTable("AlarmLimits"));
}


class AlarmLimitsRepositoryFetchTest: public OrmBaseTest
{
public:
    AlarmLimitsRepositoryFetchTest() :
            OrmBaseTest("AlarmLimitsRepositoryFetchTest.db")
    {
    }
    ~AlarmLimitsRepositoryFetchTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        alarmLimits.io_point_id = IO_POINT_ID;
        alarmLimits.set_time_seconds = SET_TIME;
        alarmLimits.clear_time_seconds = CLEAR_TIME;
        alarmLimits.high_high_set_limit = HIGH_HIGH_SET_LIMIT;
        alarmLimits.high_high_clear_limit = HIGH_HIGH_CLEAR_LIMIT;
        alarmLimits.high_set_limit = HIGH_SET_LIMIT;
        alarmLimits.high_clear_limit = HIGH_CLEAR_LIMIT;
        alarmLimits.low_low_set_limit = LOW_LOW_SET_LIMIT;
        alarmLimits.low_low_clear_limit = LOW_LOW_CLEAR_LIMIT;
        alarmLimits.low_set_limit = LOW_SET_LIMIT;
        alarmLimits.low_clear_limit = LOW_CLEAR_LIMIT;

        AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

        alarmLimits.oid = alarmLimitsRepositoryUnderTest.createAlarmLimits(alarmLimits);

    }

    alarm_limits_t alarmLimits;
};

TEST_F(AlarmLimitsRepositoryFetchTest, shouldFindAlarmLimitsWithOid)
{
    alarm_limits_t foundAlarmLimits, *result;
    AlarmLimitsRepository alarmLimitsRepositoryUnderTest(_dbContext);

    result = alarmLimitsRepositoryUnderTest.alarmLimitsForAlarmLimitsId(foundAlarmLimits, alarmLimits.oid);
    ASSERT_TRUE(&foundAlarmLimits == result) << "Error: " << ::sqlite3_errmsg(_dbContext);

    ASSERT_EQ(alarmLimits.oid, foundAlarmLimits.oid);
    ASSERT_EQ(IO_POINT_ID, foundAlarmLimits.io_point_id);
    ASSERT_EQ(SET_TIME, foundAlarmLimits.set_time_seconds);
    ASSERT_EQ(CLEAR_TIME, foundAlarmLimits.clear_time_seconds);
    ASSERT_STREQ(HIGH_HIGH_SET_LIMIT, foundAlarmLimits.high_high_set_limit.c_str());
    ASSERT_STREQ(HIGH_HIGH_CLEAR_LIMIT, foundAlarmLimits.high_high_clear_limit.c_str());
    ASSERT_STREQ(HIGH_SET_LIMIT, foundAlarmLimits.high_set_limit.c_str());
    ASSERT_STREQ(HIGH_CLEAR_LIMIT, foundAlarmLimits.high_clear_limit.c_str());
    ASSERT_STREQ(LOW_LOW_SET_LIMIT, foundAlarmLimits.low_low_set_limit.c_str());
    ASSERT_STREQ(LOW_LOW_CLEAR_LIMIT, foundAlarmLimits.low_low_clear_limit.c_str());
    ASSERT_STREQ(LOW_SET_LIMIT, foundAlarmLimits.low_set_limit.c_str());
    ASSERT_STREQ(LOW_CLEAR_LIMIT, foundAlarmLimits.low_clear_limit.c_str());
}


}
