#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>

#include "OrmBaseTest.h"
#include "DbUpgradeVersion0.h"

namespace dios::persist
{
    class DbUpgradeVersion0Test : public OrmBaseTest
    {
    public:
        DbUpgradeVersion0Test() : OrmBaseTest("DbUpgradeVersion0Test") {}
        ~DbUpgradeVersion0Test() = default;
    };

    TEST_F(DbUpgradeVersion0Test, testDbAccess)
    {
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        ASSERT_TRUE(tableExists(IOPOINT_TABLENAME));
        ASSERT_TRUE(tableExists(ALARM_LIMITS_TABLENAME));
        ASSERT_TRUE(tableExists(DB_VERSION_TABLENAME));
        ASSERT_TRUE(tableExists(DEVICE_TABLENAME));
        ASSERT_TRUE(tableExists(DEADBAND_TABLENAME));
    }

}
