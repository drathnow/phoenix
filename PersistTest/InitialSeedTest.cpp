#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <persist.h>
#include <DbUpgrader.h>
#include <iostream>

#include "OrmBaseTest.h"

namespace dios::persist
{

static int s3lCollateProcess(void *pArg, int count, char **values, char **columnNames)
{
    for (int i = 0; i < count; i++)
        cout << "Tuple: (" << columnNames[i] << ", " << values[i] << ")" << endl;
    return 0;
}

static int executeCommandInContext(const char *command, sqlite3_callback callback, void *arg, sqlite3 *sqlContext)
{
    int rc = ::sqlite3_exec(sqlContext, command, callback, (void*) arg, NULL);
    if (SQLITE_OK != rc)
        cerr << "SQL Error: " << command << " Error : " << ::sqlite3_errmsg(sqlContext);
    return rc;
}

class InitialSeedTest: public OrmBaseTest
{
public:
    InitialSeedTest() :
            OrmBaseTest("InitialSeed.db", true)
    {
    }
    ~InitialSeedTest() = default;
};

TEST_F(InitialSeedTest, DISABLED_seedDb)
{
    DbUpgrader dbUpgrader;
    dbUpgrader.doUpgrade(_dbContext);

    struct device dev;
    dev.device_type = DeviceType::DEVICE_TYPE_MODBUS_TCP;
    dev.name = "Device1";
    dev.max_retries = 3;
    dev.request_timeout_seconds = 10;
    dev.rtu_backoff_count = 3;
    dev.rtu_backoff_timeout_seconds = 30;
    dev.address = "1.2.3.4";

    ASSERT_TRUE(insert_entity(dev, _dbContext));
    ASSERT_TRUE(dev.oid != 0);

    struct io_point iop;
    iop.name = "IOPoint1";
    iop.io_point_type = IOPOINT_TYPE_APP;
    iop.data_type = DataType::INT32;
    iop.device_id = dev.oid;
    iop.readonly = true;
    iop.system = false;
    iop.source_address = "40001";

    ASSERT_TRUE(insert_entity(iop, _dbContext));
    ASSERT_TRUE(iop.oid != 0);

    struct deadband db;
    db.deadband_type = DEADBAND_ABSOLUTE;
    db.delta = "10";
    db.io_point_id = iop.oid;

    ASSERT_TRUE(insert_entity(db, _dbContext));
    ASSERT_TRUE(iop.oid != 0);

    struct alarm_limits al;
    al.io_point_id = iop.oid;
    al.clear_time_seconds = 10;
    al.set_time_seconds = 5;
    al.low_low_set_limit = "10";
    al.low_low_clear_limit = "11";
    al.low_set_limit = "15";
    al.low_clear_limit = "16";
    al.high_high_set_limit = "30";
    al.high_high_clear_limit = "29";
    al.high_set_limit = "25";
    al.high_clear_limit = "24";
    al.no_data_enabled = true;

    ASSERT_TRUE(insert_entity(al, _dbContext));
    ASSERT_TRUE(iop.oid != 0);

    //executeCommandInContext("select * from iopoint", s3lCollateProcess, nullptr, _dbContext);
}
}
