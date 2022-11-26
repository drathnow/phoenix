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
#include <Measurement.h>

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
        IOPoint *foundPoint = (IOPoint*) pArg;
        for (int i = 0; i < argc; i++)
        {
            if (0 == ::strcasecmp("oid", columnNames[i]))
            {
                foundPoint->setOid(atol(values[i]));
                continue;
            }
            if (0 == ::strcasecmp("name", columnNames[i]))
            {
                foundPoint->setName(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("io_point_type", columnNames[i]))
            {
                foundPoint->setPointType((IOPointType) atoi(values[i]));
                continue;
            }
            if (0 == ::strcasecmp("data_type", columnNames[i]))
            {
                foundPoint->setDataType((DataType) atoi(values[i]));
                continue;
            }
            if (0 == ::strcasecmp("device_id", columnNames[i]))
            {
                foundPoint->setDeviceId(atol(values[i]));
                continue;
            }
            if (0 == ::strcasecmp("is_readonly", columnNames[i]))
            {
                foundPoint->setReadonly(atoi(values[i]) == 0 ? false : true);
                continue;
            }
            if (0 == ::strcasecmp("is_system", columnNames[i]))
            {
                foundPoint->setSystem(atoi(values[i]) == 0 ? false : true);
                continue;
            }
            if (0 == ::strcasecmp("source_address", columnNames[i]))
            {
                foundPoint->setSourceAddress(values[i] == nullptr ? "" : values[i]);
                continue;
            }
            if (0 == ::strcasecmp("display_hint", columnNames[i]))
            {
                foundPoint->setDisplayHint(values[i] == nullptr ? "" : values[i]);
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

        testIoPoint.setName(NAME);
        testIoPoint.setDataType(DATA_TYPE);
        testIoPoint.setDeviceId(DEVICE_ID);
        testIoPoint.setPointType(POINT_TYPE);
        testIoPoint.setReadonly(IS_READONLY);
        testIoPoint.setSystem(IS_SYSTEM);
        testIoPoint.setSourceAddress(SOURCE_ADDRESS);
        testIoPoint.setDisplayHint(DISPLAY_HINT);
    }

    IOPoint testIoPoint;
};

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPoint)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIOPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    IOPoint foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid());
    ASSERT_STREQ(NAME, foundPoint.name().c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.dataType());
    ASSERT_EQ(DEVICE_ID, foundPoint.deviceId());
    ASSERT_EQ(POINT_TYPE, foundPoint.pointType());
    ASSERT_EQ(IS_READONLY, foundPoint.isReadonly());
    ASSERT_EQ(IS_SYSTEM, foundPoint.isSystem());
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.sourceAddress().c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.displayHint().c_str());
}

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPointWithEmptySourceAddress)
{
    testIoPoint.setSourceAddress("");

    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIOPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    IOPoint foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid());
    ASSERT_STREQ(NAME, foundPoint.name().c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.dataType());
    ASSERT_EQ(DEVICE_ID, foundPoint.deviceId());
    ASSERT_EQ(POINT_TYPE, foundPoint.pointType());
    ASSERT_EQ(IS_READONLY, foundPoint.isReadonly());
    ASSERT_EQ(IS_SYSTEM, foundPoint.isSystem());
    ASSERT_STREQ("", foundPoint.sourceAddress().c_str());
    ASSERT_STREQ(DISPLAY_HINT, foundPoint.displayHint().c_str());
}

TEST_F(IOPointRepositoryCreateTest, shouldCreateIoPointwithEmptyDisplayHint)
{
    testIoPoint.setDisplayHint("");

    IOPointRepository repositoryUnderTest(_dbContext);

    iopoint_id_t resultId = repositoryUnderTest.createIOPoint(testIoPoint);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("IOPoint"));

    IOPoint foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(resultId, foundPoint.oid());
    ASSERT_STREQ(NAME, foundPoint.name().c_str());
    ASSERT_EQ(DATA_TYPE, foundPoint.dataType());
    ASSERT_EQ(DEVICE_ID, foundPoint.deviceId());
    ASSERT_EQ(POINT_TYPE, foundPoint.pointType());
    ASSERT_EQ(IS_READONLY, foundPoint.isReadonly());
    ASSERT_EQ(IS_SYSTEM, foundPoint.isSystem());
    ASSERT_STREQ(SOURCE_ADDRESS, foundPoint.sourceAddress().c_str());
    ASSERT_STREQ("", foundPoint.displayHint().c_str());
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
        testIoPoint.setName(NAME);
        testIoPoint.setDataType(DATA_TYPE);
        testIoPoint.setDeviceId(DEVICE_ID);
        testIoPoint.setPointType(POINT_TYPE);
        testIoPoint.setReadonly(IS_READONLY);
        testIoPoint.setSystem(IS_SYSTEM);
        testIoPoint.setSourceAddress(SOURCE_ADDRESS);
        testIoPoint.setDisplayHint(DISPLAY_HINT);

        IOPointRepository repo(_dbContext);
        testIoPoint.setOid(repo.createIOPoint(testIoPoint));
    }

    IOPoint testIoPoint;
};

TEST_F(IOPointRepositoryUpdateTest, shouldUpdateIoPointName)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    testIoPoint.setName("UpdatePointName1");

    ASSERT_EQ(0, repositoryUnderTest.updateIOPoint(testIoPoint))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    IOPoint foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(testIoPoint.oid(), foundPoint.oid());
    ASSERT_STREQ(testIoPoint.name().c_str(), foundPoint.name().c_str());
    ASSERT_EQ(testIoPoint.dataType(), foundPoint.dataType());
    ASSERT_EQ(testIoPoint.deviceId(), foundPoint.deviceId());
    ASSERT_EQ(testIoPoint.pointType(), foundPoint.pointType());
    ASSERT_EQ(testIoPoint.isReadonly(), foundPoint.isReadonly());
    ASSERT_EQ(testIoPoint.isSystem(), foundPoint.isSystem());
    ASSERT_STREQ(testIoPoint.sourceAddress().c_str(), foundPoint.sourceAddress().c_str());
    ASSERT_STREQ(testIoPoint.displayHint().c_str(), foundPoint.displayHint().c_str());

}

TEST_F(IOPointRepositoryUpdateTest, shouldUpdateDataType)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    testIoPoint.setDataType(DataType::BLOB);

    ASSERT_EQ(0, repositoryUnderTest.updateIOPoint(testIoPoint))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    IOPoint foundPoint;
    executeCommandInContext("select * from IOPoint;", ioPointRowCollater, &foundPoint);

    ASSERT_EQ(testIoPoint.oid(), foundPoint.oid());
    ASSERT_STREQ(testIoPoint.name().c_str(), foundPoint.name().c_str());
    ASSERT_EQ(testIoPoint.dataType(), foundPoint.dataType());
    ASSERT_EQ(testIoPoint.deviceId(), foundPoint.deviceId());
    ASSERT_EQ(testIoPoint.pointType(), foundPoint.pointType());
    ASSERT_EQ(testIoPoint.isReadonly(), foundPoint.isReadonly());
    ASSERT_EQ(testIoPoint.isSystem(), foundPoint.isSystem());
    ASSERT_STREQ(testIoPoint.sourceAddress().c_str(), foundPoint.sourceAddress().c_str());
    ASSERT_STREQ(testIoPoint.displayHint().c_str(), foundPoint.displayHint().c_str());
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

        testIoPoint.setName(NAME);
        testIoPoint.setDataType(DATA_TYPE);
        testIoPoint.setDeviceId(DEVICE_ID);
        testIoPoint.setPointType(POINT_TYPE);
        testIoPoint.setReadonly(IS_READONLY);
        testIoPoint.setSystem(IS_SYSTEM);
        testIoPoint.setSourceAddress(SOURCE_ADDRESS);
        testIoPoint.setDisplayHint(DISPLAY_HINT);

        IOPointRepository repo(_dbContext);
        repo.createIOPoint(testIoPoint);
    }

    IOPoint testIoPoint;
};

TEST_F(IOPointRepositoryDeleteTest, shouldDeleteIOPoint)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    ASSERT_EQ(1, rowCountInTable("IOPoint"));
    ASSERT_EQ(0, repositoryUnderTest.deleteIOPointWithOid(testIoPoint.oid())) << "Error: " << ::sqlite3_errmsg(_dbContext);
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

        testIoPoint.setName(NAME);
        testIoPoint.setDataType(DATA_TYPE);
        testIoPoint.setDeviceId(DEVICE_ID);
        testIoPoint.setPointType(POINT_TYPE);
        testIoPoint.setReadonly(IS_READONLY);
        testIoPoint.setSystem(IS_SYSTEM);
        testIoPoint.setSourceAddress(SOURCE_ADDRESS);
        testIoPoint.setDisplayHint(DISPLAY_HINT);

        IOPointRepository repo(_dbContext);
        repo.createIOPoint(testIoPoint);
    }

    IOPoint testIoPoint;
};

TEST_F(IOPointRepositoryFetchTest, shouldReturnIoPointWithOid)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    std::unique_ptr<IOPoint> foundPoint(repositoryUnderTest.ioPointForOid(testIoPoint.oid()));
    ASSERT_TRUE(foundPoint.get() != nullptr)<< "Error: " << ::sqlite3_errmsg(_dbContext);

    ASSERT_EQ(testIoPoint.oid(), foundPoint->oid());
    ASSERT_STREQ(testIoPoint.name().c_str(), foundPoint->name().c_str());
    ASSERT_EQ(testIoPoint.dataType(), foundPoint->dataType());
    ASSERT_EQ(testIoPoint.deviceId(), foundPoint->deviceId());
    ASSERT_EQ(testIoPoint.pointType(), foundPoint->pointType());
    ASSERT_EQ(testIoPoint.isReadonly(), foundPoint->isReadonly());
    ASSERT_EQ(testIoPoint.isSystem(), foundPoint->isSystem());
    ASSERT_STREQ(testIoPoint.sourceAddress().c_str(), foundPoint->sourceAddress().c_str());
    ASSERT_STREQ(testIoPoint.displayHint().c_str(), foundPoint->displayHint().c_str());
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

        testIoPoint.setDataType(DATA_TYPE);
        testIoPoint.setDeviceId(DEVICE_ID);
        testIoPoint.setPointType(POINT_TYPE);
        testIoPoint.setReadonly(IS_READONLY);
        testIoPoint.setSystem(IS_SYSTEM);
        testIoPoint.setSourceAddress(SOURCE_ADDRESS);
        testIoPoint.setDisplayHint(DISPLAY_HINT);

        IOPointRepository repo(_dbContext);
        for (int i = 0; i < 20; i++)
        {
            string foo;
            StringHelper::numberToString(i, foo);
            string newName(NAME);
            newName.append("-").append(foo);
            testIoPoint.setName(newName);
            repo.createIOPoint(testIoPoint);
        }
    }

    IOPoint testIoPoint;
};

TEST_F(IOPointRepositoryFetchManyTest, shouldFetchAllIoPoints)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<IOPoint*> ioPoints;

    ASSERT_EQ(20, repositoryUnderTest.ioPoints(ioPoints, 0));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_STREQ(name.c_str(), ioPoints[i]->name().c_str());
        ASSERT_EQ(DATA_TYPE, ioPoints[i]->dataType());
        ASSERT_EQ(DEVICE_ID, ioPoints[i]->deviceId());
        ASSERT_EQ(POINT_TYPE, ioPoints[i]->pointType());
        ASSERT_EQ(IS_READONLY, ioPoints[i]->isReadonly());
        ASSERT_EQ(IS_SYSTEM, ioPoints[i]->isSystem());
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i]->sourceAddress().c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i]->displayHint().c_str());
    }
}

TEST_F(IOPointRepositoryFetchManyTest, shouldFetch10IoPointsThenNext10)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<IOPoint*> ioPoints;

    ASSERT_EQ(10, repositoryUnderTest.ioPoints(ioPoints, 10, 0)) << "Error: " << ::sqlite3_errmsg(_dbContext);

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_EQ(DATA_TYPE, ioPoints[i]->dataType());
        ASSERT_EQ(DEVICE_ID, ioPoints[i]->deviceId());
        ASSERT_EQ(POINT_TYPE, ioPoints[i]->pointType());
        ASSERT_EQ(IS_READONLY, ioPoints[i]->isReadonly());
        ASSERT_EQ(IS_SYSTEM, ioPoints[i]->isSystem());
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i]->sourceAddress().c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i]->displayHint().c_str());;
    }

    ioPoints.clear();
    ASSERT_EQ(10, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[9]->oid()));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i+10, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_EQ(DATA_TYPE, ioPoints[i]->dataType());
        ASSERT_EQ(DEVICE_ID, ioPoints[i]->deviceId());
        ASSERT_EQ(POINT_TYPE, ioPoints[i]->pointType());
        ASSERT_EQ(IS_READONLY, ioPoints[i]->isReadonly());
        ASSERT_EQ(IS_SYSTEM, ioPoints[i]->isSystem());
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i]->sourceAddress().c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i]->displayHint().c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(0, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[9]->oid()));
}

TEST_F(IOPointRepositoryFetchManyTest, shouldFetch15IoPointsThenNext5)
{
    IOPointRepository repositoryUnderTest(_dbContext);

    vector<IOPoint*> ioPoints;

    ASSERT_EQ(15, repositoryUnderTest.ioPoints(ioPoints, 15, 0)) << "Error: " << ::sqlite3_errmsg(_dbContext);

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_EQ(DATA_TYPE, ioPoints[i]->dataType());
        ASSERT_EQ(DEVICE_ID, ioPoints[i]->deviceId());
        ASSERT_EQ(POINT_TYPE, ioPoints[i]->pointType());
        ASSERT_EQ(IS_READONLY, ioPoints[i]->isReadonly());
        ASSERT_EQ(IS_SYSTEM, ioPoints[i]->isSystem());
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i]->sourceAddress().c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i]->displayHint().c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(5, repositoryUnderTest.ioPoints(ioPoints, 15, ioPoints[14]->oid()));

    for (int i = 0; i < (signed)ioPoints.size(); i++)
    {
        string foo, name;
        StringHelper::numberToString(i+15, foo);
        name.append(NAME).append("-").append(foo);
        ASSERT_EQ(DATA_TYPE, ioPoints[i]->dataType());
        ASSERT_EQ(DEVICE_ID, ioPoints[i]->deviceId());
        ASSERT_EQ(POINT_TYPE, ioPoints[i]->pointType());
        ASSERT_EQ(IS_READONLY, ioPoints[i]->isReadonly());
        ASSERT_EQ(IS_SYSTEM, ioPoints[i]->isSystem());
        ASSERT_STREQ(SOURCE_ADDRESS, ioPoints[i]->sourceAddress().c_str());
        ASSERT_STREQ(DISPLAY_HINT, ioPoints[i]->displayHint().c_str());
    }

    ioPoints.clear();
    ASSERT_EQ(0, repositoryUnderTest.ioPoints(ioPoints, 10, ioPoints[4]->oid()));
}

}
