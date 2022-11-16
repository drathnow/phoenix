#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <IOPointRepository.h>
#include <persist.h>
#include <cstdint>
#include <sqlite3.h>
#include <DbUpgrader.h>
#include <memory>
#include <StringHelper.h>
#include <vector>

#include "OrmBaseTest.h"

namespace dios::persist
{

using namespace std;
using namespace dios::foundation;

static const char *NAME = "IOPoint1";
static DataType DATA_TYPE = DataType::INT16;
static uint64_t DEVICE_ID = 0;
static IOPointType POINT_TYPE = IOPointType::IOPOINT_TYPE_APP;
static bool IS_READONLY = false;
static bool IS_SYSTEM = true;
static const char *SOURCE_ADDRESS = "Hello World";
static const char *DISPLAY_HINT = "UOM";

static int ioPointRowCollater(void *pArg, int argc, char **values, char **columnNames)
{
    if (NULL != pArg)
    {
        io_point_t *foundPoint = (io_point_t*) pArg;
        for (int i = 0; i < argc; i++)
        {
            if (0 == ::strcasecmp("oid", columnNames[i]))
            {
                foundPoint->oid = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("name", columnNames[i]))
            {
                foundPoint->name = values[i];
                continue;
            }
            if (0 == ::strcasecmp("io_point_type", columnNames[i]))
            {
                foundPoint->io_point_type = (IOPointType) atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("data_type", columnNames[i]))
            {
                foundPoint->data_type = (DataType) atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("device_id", columnNames[i]))
            {
                foundPoint->device_id = atol(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("is_readonly", columnNames[i]))
            {
                foundPoint->readonly = atoi(values[i]) == 0 ? false : true;
                continue;
            }
            if (0 == ::strcasecmp("is_system", columnNames[i]))
            {
                foundPoint->system = atoi(values[i]) == 0 ? false : true;
                continue;
            }
            if (0 == ::strcasecmp("source_address", columnNames[i]))
            {
                foundPoint->source_address = values[i] == nullptr ? "" : values[i];
                continue;
            }
            if (0 == ::strcasecmp("display_hint", columnNames[i]))
            {
                foundPoint->display_hint = values[i] == nullptr ? "" : values[i];
                continue;
            }
        }
        return 0;
    }

    return -1;
}

class IOPointRepositoryCreateTest: public OrmBaseTest
{
public:
    IOPointRepositoryCreateTest() :
            OrmBaseTest("IOPointRepositoryCreateTest.db")
    {
    }
    ~IOPointRepositoryCreateTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        testIoPoint.name = NAME;
        testIoPoint.data_type = DATA_TYPE;
        testIoPoint.device_id = DEVICE_ID;
        testIoPoint.io_point_type = POINT_TYPE;
        testIoPoint.readonly = IS_READONLY;
        testIoPoint.system = IS_SYSTEM;
        testIoPoint.source_address = SOURCE_ADDRESS;
        testIoPoint.display_hint = DISPLAY_HINT;

    }

    io_point_t testIoPoint;
};

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPoint)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIoPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid);
    ASSERT_STREQ(NAME, foundPoint.name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.display_hint.c_str());
}

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPointWithEmptySourceAddress)
{
    testIoPoint.source_address = "";

    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIoPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid);
    ASSERT_STREQ(NAME, foundPoint.name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ("", foundPoint.source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.display_hint.c_str());
}

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPointwithEmptyDisplayHint)
{
    testIoPoint.display_hint = "";

    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIoPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid);
    ASSERT_STREQ(NAME, foundPoint.name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.source_address.c_str());
    ASSERT_STREQ("", foundPoint.display_hint.c_str());
}

TEST_F(IOPointRepositoryCreateTest, shouldUpdateName)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIoPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid);
    ASSERT_STREQ(NAME, foundPoint.name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.display_hint.c_str());
}

class IOPointRepositoryUpdateTest: public OrmBaseTest
{
public:
    IOPointRepositoryUpdateTest() :
            OrmBaseTest("IOPointRepositoryUpdateTest.db")
    {
    }
    ~IOPointRepositoryUpdateTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        testIoPoint.name = NAME;
        testIoPoint.data_type = DATA_TYPE;
        testIoPoint.device_id = DEVICE_ID;
        testIoPoint.io_point_type = POINT_TYPE;
        testIoPoint.readonly = IS_READONLY;
        testIoPoint.system = IS_SYSTEM;
        testIoPoint.source_address = SOURCE_ADDRESS;
        testIoPoint.display_hint = DISPLAY_HINT;

        IOPointRepository repo(_dbContext);
        testIoPoint.oid = repo.createIoPoint(testIoPoint);
    }

    io_point_t testIoPoint;
};

TEST_F(IOPointRepositoryUpdateTest, shouldUpdateIoPointName)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    testIoPoint.name = "UpdatePointName1";

    ASSERT_EQ(0, repositoryUnderTest.updateIoPoint(testIoPoint))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(testIoPoint.oid, foundPoint.oid);
    ASSERT_STREQ("UpdatePointName1", foundPoint.name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.display_hint.c_str());

}

TEST_F(IOPointRepositoryUpdateTest, shouldUpdateDataType)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    testIoPoint.data_type = DataType::BLOB;

    ASSERT_EQ(0, repositoryUnderTest.updateIoPoint(testIoPoint))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    io_point_t foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(testIoPoint.oid, foundPoint.oid);
    ASSERT_STREQ(NAME, foundPoint.name.c_str());
    ASSERT_EQ(DataType::BLOB, foundPoint.data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint.device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint.io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint.readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint.system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.display_hint.c_str());

}

class IOPointRepositoryDeleteTest: public OrmBaseTest
{
public:
    IOPointRepositoryDeleteTest() :
            OrmBaseTest("IOPointRepositoryDeleteTest.db")
    {
    }
    ~IOPointRepositoryDeleteTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        testIoPoint.name = NAME;
        testIoPoint.data_type = DATA_TYPE;
        testIoPoint.device_id = DEVICE_ID;
        testIoPoint.io_point_type = POINT_TYPE;
        testIoPoint.readonly = IS_READONLY;
        testIoPoint.system = IS_SYSTEM;
        testIoPoint.source_address = SOURCE_ADDRESS;
        testIoPoint.display_hint = DISPLAY_HINT;

        IOPointRepository repo(_dbContext);
        testIoPoint.oid = repo.createIoPoint(testIoPoint);
    }

    io_point_t testIoPoint;
};

TEST_F(IOPointRepositoryDeleteTest, shouldDeleteIOPoint)
{
    IOPointRepository repositoryUnderTest(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));
    ASSERT_EQ(0, repositoryUnderTest.deleteIoPointWithOid(testIoPoint.oid))<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(0, rowCountInTable("IOPoint"));
}

class IOPointRepositoryFetchTest: public OrmBaseTest
{
public:
    IOPointRepositoryFetchTest() :
            OrmBaseTest("IOPointRepositoryFetchTest.db")
    {
    }
    ~IOPointRepositoryFetchTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        testIoPoint.name = NAME;
        testIoPoint.data_type = DATA_TYPE;
        testIoPoint.device_id = DEVICE_ID;
        testIoPoint.io_point_type = POINT_TYPE;
        testIoPoint.readonly = IS_READONLY;
        testIoPoint.system = IS_SYSTEM;
        testIoPoint.source_address = SOURCE_ADDRESS;
        testIoPoint.display_hint = DISPLAY_HINT;

        IOPointRepository repo(_dbContext);
        testIoPoint.oid = repo.createIoPoint(testIoPoint);
    }

    io_point_t testIoPoint;
};

TEST_F(IOPointRepositoryFetchTest, shouldReturnIoPointWithOid)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    std::unique_ptr<io_point_t> foundPoint(repositoryUnderTest.ioPointForOid(testIoPoint.oid));
    ASSERT_TRUE(foundPoint.get() != nullptr)<< "Error: " << ::sqlite3_errmsg(_dbContext);

    ASSERT_EQ(testIoPoint.oid, foundPoint->oid);
    ASSERT_STREQ(NAME, foundPoint->name.c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint->data_type);
    ASSERT_EQ(DEVICE_ID, foundPoint->device_id);
    ASSERT_EQ(POINT_TYPE, foundPoint->io_point_type);
    ASSERT_EQ(IS_READONLY, foundPoint->readonly);
    ASSERT_EQ(IS_SYSTEM, foundPoint->system);
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint->source_address.c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint->display_hint.c_str());
}

class IOPointRepositoryFetchManyTest: public OrmBaseTest
{
public:
    IOPointRepositoryFetchManyTest() :
            OrmBaseTest("IOPointRepositoryFetchTest.db", true)
    {
    }
    ~IOPointRepositoryFetchManyTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        testIoPoint.data_type = DATA_TYPE;
        testIoPoint.device_id = DEVICE_ID;
        testIoPoint.io_point_type = POINT_TYPE;
        testIoPoint.readonly = IS_READONLY;
        testIoPoint.system = IS_SYSTEM;
        testIoPoint.source_address = SOURCE_ADDRESS;
        testIoPoint.display_hint = DISPLAY_HINT;

        IOPointRepository repo(_dbContext);
        for (int i = 0; i < 20; i++)
        {
            string foo;
            StringHelper::numberToString(i, foo);
            testIoPoint.name = NAME;
            testIoPoint.name.append("-").append(foo);
            testIoPoint.oid = repo.createIoPoint(testIoPoint);
        }
    }

    io_point_t testIoPoint;
};

TEST_F(IOPointRepositoryFetchManyTest, shouldFetchAllIoPoints)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<io_point_t> ioPoints;

    ASSERT_EQ(20, repositoryUnderTest.ioPoints(ioPoints, 0));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i].name.c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i].data_type);
        ASSERT_EQ(DEVICE_ID, ioPoints[i].device_id);
        ASSERT_EQ(POINT_TYPE, ioPoints[i].io_point_type);
        ASSERT_EQ(IS_READONLY, ioPoints[i].readonly);
        ASSERT_EQ(IS_SYSTEM, ioPoints[i].system);
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i].source_address.c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i].display_hint.c_str());
    }
}

TEST_F(IOPointRepositoryFetchManyTest, shouldFetch10IoPointsThenNext10)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<io_point_t> ioPoints;

    ASSERT_EQ(10, repositoryUnderTest.ioPoints(ioPoints, 10, 0)) << "Error: " << ::sqlite3_errmsg(_dbContext);

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i].name.c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i].data_type);
        ASSERT_EQ(DEVICE_ID, ioPoints[i].device_id);
        ASSERT_EQ(POINT_TYPE, ioPoints[i].io_point_type);
        ASSERT_EQ(IS_READONLY, ioPoints[i].readonly);
        ASSERT_EQ(IS_SYSTEM, ioPoints[i].system);
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i].source_address.c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i].display_hint.c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(10, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[9].oid));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i+10, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i].name.c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i].data_type);
        ASSERT_EQ(DEVICE_ID, ioPoints[i].device_id);
        ASSERT_EQ(POINT_TYPE, ioPoints[i].io_point_type);
        ASSERT_EQ(IS_READONLY, ioPoints[i].readonly);
        ASSERT_EQ(IS_SYSTEM, ioPoints[i].system);
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i].source_address.c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i].display_hint.c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(0, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[9].oid));
}


TEST_F(IOPointRepositoryFetchManyTest, shouldFetch15IoPointsThenNext5)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<io_point_t> ioPoints;

    ASSERT_EQ(15, repositoryUnderTest.ioPoints(ioPoints, 15, 0)) << "Error: " << ::sqlite3_errmsg(_dbContext);

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i].name.c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i].data_type);
        ASSERT_EQ(DEVICE_ID, ioPoints[i].device_id);
        ASSERT_EQ(POINT_TYPE, ioPoints[i].io_point_type);
        ASSERT_EQ(IS_READONLY, ioPoints[i].readonly);
        ASSERT_EQ(IS_SYSTEM, ioPoints[i].system);
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i].source_address.c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i].display_hint.c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(5, repositoryUnderTest.ioPoints(ioPoints, 15, ioPoints[14].oid));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i+15, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i].name.c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i].data_type);
        ASSERT_EQ(DEVICE_ID, ioPoints[i].device_id);
        ASSERT_EQ(POINT_TYPE, ioPoints[i].io_point_type);
        ASSERT_EQ(IS_READONLY, ioPoints[i].readonly);
        ASSERT_EQ(IS_SYSTEM, ioPoints[i].system);
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i].source_address.c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i].display_hint.c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(0, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[4].oid));
}
}
