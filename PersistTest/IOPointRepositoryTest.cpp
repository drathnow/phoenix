#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <IOPointRepository.h>
#include <persist.h>
#include <cstdint>
#include <sqlite3.h>
#include <DbUpgrader.h>

#include "OrmBaseTest.h"

namespace dios::persist
{

static const char *NAME = "IOPoint1";
static DataType DATA_TYPE = DataType::INT16;
static uint64_t DEVICE_ID = 0;
static IOPointType POINT_TYPE = IOPointType::IOPOINT_TYPE_APP;
static bool IS_READONLY = false;
static bool IS_SYSTEM = true;
static const char *SOURCE_ADDRESS = "Hello World";
static const char *DISPLAY_HINT = "UOM";

class IOPointRepositoryTest: public OrmBaseTest
{
public:
    IOPointRepositoryTest() :
            OrmBaseTest("IOPointRepositoryTest.db", true)
    {
    }
    ~IOPointRepositoryTest() = default;

    void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);
    }
};

TEST_F(IOPointRepositoryTest, shouldCreateIoPoint)
{
    io_point_t ioPoint;
    ioPoint.name = NAME;
    ioPoint.data_type = DATA_TYPE;
    ioPoint.device_id = DEVICE_ID;
    ioPoint.io_point_type = POINT_TYPE;
    ioPoint.readonly = IS_READONLY;
    ioPoint.system = IS_SYSTEM;
    ioPoint.source_address = SOURCE_ADDRESS;
    ioPoint.display_hint = DISPLAY_HINT;

    IOPointRepository repositoryUnderTest(_dbContext);

    ASSERT_TRUE(repositoryUnderTest.createIoPoint(ioPoint) > 0) << "Error: " << ::sqlite3_errmsg(_dbContext);

}
}
