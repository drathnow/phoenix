#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <persist.h>
#include <DeadbandRepository.h>
#include <memory>

#include "OrmBaseTest.h"

namespace dios::persist
{

static const iopoint_id_t IO_POINT_ID = 1;
static const DeadbandType DEADBAND_TYPE = DeadbandType::DEADBAND_ABSOLUTE;
static const char *DELTA = "3";


static int deadbandRowCollater(void *pArg, int argc, char **values, char **columnNames)
{
    if (NULL != pArg)
    {
        deadband_t *deadband = (deadband_t*) pArg;
        for (int i = 0; i < argc; i++)
        {
            if (0 == ::strcasecmp("oid", columnNames[i]))
            {
                deadband->oid = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("io_point_id", columnNames[i]))
            {
                deadband->io_point_id = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("deadband_type", columnNames[i]))
            {
                deadband->deadband_type = (DeadbandType)atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("delta", columnNames[i]))
            {
                deadband->delta = values[i];
                continue;
            }
        }
        return 0;
    }

    return -1;
}



class DeadbandRepositoryTest: public OrmBaseTest
{
public:
    DeadbandRepositoryTest() :
            OrmBaseTest("DeadbandRepositoryTest.db", true)
    {
    }
    virtual ~DeadbandRepositoryTest() = default;

    virtual void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        deadband.io_point_id = IO_POINT_ID;
        deadband.deadband_type = DEADBAND_TYPE;
        deadband.delta = DELTA;
    }

    deadband_t deadband;

protected:
    DeadbandRepositoryTest(const char* testname) :
            OrmBaseTest(testname)
    {
    }

};

TEST_F(DeadbandRepositoryTest, shouldCreatDeadband)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    int64_t resultId = deadbandRepositoryUnderTest.createDeadband(deadband);
    ASSERT_TRUE(resultId > 0) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("Deadband"));

    deadband_t foundDeadband;
    executeCommandInContext("select * from Deadband", deadbandRowCollater, &foundDeadband);

    ASSERT_EQ(resultId, foundDeadband.oid);
    ASSERT_EQ(IO_POINT_ID, foundDeadband.io_point_id);
    ASSERT_EQ(DEADBAND_TYPE, foundDeadband.deadband_type);
    ASSERT_STREQ(DELTA, foundDeadband.delta.c_str());
}

class DeadbandRepositoryUpdateTest: public DeadbandRepositoryTest
{
public:
    DeadbandRepositoryUpdateTest() :
        DeadbandRepositoryTest("DeadbandRepositoryUpdateTest.db")
    {
    }
    ~DeadbandRepositoryUpdateTest() = default;

    void SetUp()
    {
        DeadbandRepositoryTest::SetUp();
        DeadbandRepository repo(_dbContext);
        deadband.oid = repo.createDeadband(deadband);
    }

    deadband_t deadband;
};

TEST_F(DeadbandRepositoryUpdateTest, shouldUpdateIoPointId)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    deadband.io_point_id = IO_POINT_ID+1;
    ASSERT_EQ(0, deadbandRepositoryUnderTest.updateDeadband(deadband));

    deadband_t foundDeadband;
    executeCommandInContext("select * from Deadband", deadbandRowCollater, &foundDeadband);

    ASSERT_EQ(deadband.oid, foundDeadband.oid);
    ASSERT_EQ(deadband.io_point_id, foundDeadband.io_point_id);
    ASSERT_EQ(deadband.deadband_type, foundDeadband.deadband_type);
    ASSERT_STREQ(deadband.delta.c_str(), foundDeadband.delta.c_str());
}

TEST_F(DeadbandRepositoryUpdateTest, shouldUpdateDeadbandType)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    deadband.deadband_type = DeadbandType::DEADBAND_PERCENTAGE;
    ASSERT_EQ(0, deadbandRepositoryUnderTest.updateDeadband(deadband));

    deadband_t foundDeadband;
    executeCommandInContext("select * from Deadband", deadbandRowCollater, &foundDeadband);

    ASSERT_EQ(deadband.oid, foundDeadband.oid);
    ASSERT_EQ(deadband.io_point_id, foundDeadband.io_point_id);
    ASSERT_EQ(deadband.deadband_type, foundDeadband.deadband_type);
    ASSERT_STREQ(deadband.delta.c_str(), foundDeadband.delta.c_str());
}
TEST_F(DeadbandRepositoryUpdateTest, shouldUpdateDelta)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    deadband.delta = "Hello Delta";
    ASSERT_EQ(0, deadbandRepositoryUnderTest.updateDeadband(deadband));

    deadband_t foundDeadband;
    executeCommandInContext("select * from Deadband", deadbandRowCollater, &foundDeadband);

    ASSERT_EQ(deadband.oid, foundDeadband.oid);
    ASSERT_EQ(deadband.io_point_id, foundDeadband.io_point_id);
    ASSERT_EQ(deadband.deadband_type, foundDeadband.deadband_type);
    ASSERT_STREQ(deadband.delta.c_str(), foundDeadband.delta.c_str());
}


class DeadbandRepositoryDeleteTest: public DeadbandRepositoryTest
{
public:
    DeadbandRepositoryDeleteTest() :
        DeadbandRepositoryTest("DeadbandRepositoryDeleteTest.db")
    {
    }
    ~DeadbandRepositoryDeleteTest() = default;

    void SetUp()
    {
        DeadbandRepositoryTest::SetUp();
        DeadbandRepository repo(_dbContext);
        deadband.oid = repo.createDeadband(deadband);
    }

    deadband_t deadband;
};

TEST_F(DeadbandRepositoryDeleteTest, shouldDeleteDeadband)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    ASSERT_EQ(1, rowCountInTable("Deadband"));
    ASSERT_EQ(0, deadbandRepositoryUnderTest.deleteDeadbandWithOid(deadband.oid)) << "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(0, rowCountInTable("Deadband"));
}

class DeadbandRepositoryFetchTest: public DeadbandRepositoryTest
{
public:
    DeadbandRepositoryFetchTest() :
        DeadbandRepositoryTest("DeadbandRepositoryFetchTest.db")
    {
    }
    ~DeadbandRepositoryFetchTest() = default;

    void SetUp()
    {
        DeadbandRepositoryTest::SetUp();
        DeadbandRepository repo(_dbContext);
        deadband.oid = repo.createDeadband(deadband);
    }
};

TEST_F(DeadbandRepositoryFetchTest, shouldFetchDeadband)
{
    DeadbandRepository deadbandRepositoryUnderTest(_dbContext);

    std::unique_ptr<deadband_t> foundDeadband(deadbandRepositoryUnderTest.deadbandForOid(deadband.oid));
    ASSERT_TRUE(foundDeadband.get() != nullptr);

    ASSERT_EQ(deadband.oid, foundDeadband->oid);
    ASSERT_EQ(deadband.io_point_id, foundDeadband->io_point_id);
    ASSERT_EQ(deadband.deadband_type, foundDeadband->deadband_type);
    ASSERT_STREQ(deadband.delta.c_str(), foundDeadband->delta.c_str());
}
} /* namespace dios */
