#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <DeviceRepository.h>
#include <persist.h>

#include "OrmBaseTest.h"

namespace dios::persist
{

static const char *NAME = "Device1";
static const DeviceType DEVICE_TYPE = DeviceType::DEVICE_TYPE_MODBUS;
static const int MAX_RETRIES = 3;
static const int REQ_TIMEOUT = 10;
static const int BACKOFF_TIMEOUT = 30;
static const int BACKOFF_COUNT = 5;
static const char *ADDRESS = "Hello World";
static const char *PARAMS = "Some Params";
static const char *EXT_PARAMS = "Some Ext Params";

static int deviceRowCollater(void *pArg, int argc, char **values, char **columnNames)
{
    if (NULL != pArg)
    {
        device_t *device = (device_t*) pArg;
        for (int i = 0; i < argc; i++)
        {
            if (0 == ::strcasecmp("name", columnNames[i]))
            {
                device->name = values[i];
                continue;
            }
            if (0 == ::strcasecmp("device_type", columnNames[i]))
            {
                device->device_type = (DeviceType)atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("max_retries", columnNames[i]))
            {
                device->max_retries = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("request_timeout_sec", columnNames[i]))
            {
                device->request_timeout_seconds = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("rtu_backoff_timeout_sec", columnNames[i]))
            {
                device->rtu_backoff_timeout_seconds = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("rtu_backoff_count", columnNames[i]))
            {
                device->rtu_backoff_count = atoi(values[i]);
                continue;
            }
            if (0 == ::strcasecmp("address", columnNames[i]))
            {
                device->address = values[i];
                continue;
            }
            if (0 == ::strcasecmp("parameters", columnNames[i]))
            {
                device->parameters = values[i];
                continue;
            }
            if (0 == ::strcasecmp("extended_parameters", columnNames[i]))
            {
                device->extended_parameters = values[i];
                continue;
            }
            if (0 == ::strcasecmp("oid ", columnNames[i]))
            {
                device->oid = atol(values[i]);
                continue;
            }

        }
        return 0;
    }

    return -1;
}

class DeviceRepositoryTest: public OrmBaseTest
{
public:
    DeviceRepositoryTest() :
            OrmBaseTest("DeviceRepositoryTest")
    {
    }

    ~DeviceRepositoryTest() = default;

    virtual void SetUp()
    {
        OrmBaseTest::SetUp();
        DbUpgrader dbUpgrader;
        dbUpgrader.doUpgrade(_dbContext);

        device.name = NAME;
        device.device_type = DEVICE_TYPE;
        device.max_retries = MAX_RETRIES;
        device.request_timeout_seconds = REQ_TIMEOUT;
        device.rtu_backoff_timeout_seconds = BACKOFF_TIMEOUT;
        device.rtu_backoff_count = BACKOFF_COUNT;
        device.address = ADDRESS;
        device.parameters = PARAMS;
        device.extended_parameters = EXT_PARAMS;
    }

    device_t device;

protected:
    DeviceRepositoryTest(const char* testClassName) : OrmBaseTest(testClassName) {}
};

TEST_F(DeviceRepositoryTest, shouldCreateDevice)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    int64_t resultId = deviceRepositoryUnderTest.createDevice(device);
    ASSERT_TRUE(resultId > 0)<< "Error: " << ::sqlite3_errmsg(_dbContext);
    ASSERT_EQ(1, rowCountInTable("Device"));

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(NAME, foundDevice.name);
    ASSERT_EQ(DEVICE_TYPE, foundDevice.device_type);
    ASSERT_EQ(MAX_RETRIES, foundDevice.max_retries);
    ASSERT_EQ(REQ_TIMEOUT, foundDevice.request_timeout_seconds);
    ASSERT_EQ(BACKOFF_TIMEOUT, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(BACKOFF_COUNT, foundDevice.rtu_backoff_count);
    ASSERT_EQ(ADDRESS, foundDevice.address);
    ASSERT_EQ(PARAMS, foundDevice.parameters);
    ASSERT_EQ(EXT_PARAMS, foundDevice.extended_parameters);
}

class DeviceRepositoryUpdateTest : public DeviceRepositoryTest
{
public:
    DeviceRepositoryUpdateTest() : DeviceRepositoryTest("DeviceRepositoryUpdateTest") {}
    ~DeviceRepositoryUpdateTest() = default;

    void SetUp()
    {
        DeviceRepositoryTest::SetUp();
        DeviceRepository deviceRepositoryUnderTest(_dbContext);

        device.oid = deviceRepositoryUnderTest.createDevice(device);
    }
};

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateName)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.name = "NewDeviceName";
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateDeviceType)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.device_type = DEVICE_TYPE_ROC;
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateMaxRetries)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.max_retries = MAX_RETRIES+1;
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateRequestTimeout)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.request_timeout_seconds = REQ_TIMEOUT+100;
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateRtuBackoffTime)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.rtu_backoff_timeout_seconds = BACKOFF_TIMEOUT+200;
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateBackoffCount)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.rtu_backoff_count = BACKOFF_COUNT+200;
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateAddress)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.address = "NewAddress";
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateParameters)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.parameters = "NewParameters";
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}

TEST_F(DeviceRepositoryUpdateTest, shouldUpdateExtendedParameters)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);

    device.extended_parameters = "NewExtendedParams";
    ASSERT_EQ(0, deviceRepositoryUnderTest.updateDevice(device))<< "Error: " << ::sqlite3_errmsg(_dbContext);

    device_t foundDevice;
    executeCommandInContext("select * from Device;", deviceRowCollater, &foundDevice);

    ASSERT_EQ(device.name, foundDevice.name);
    ASSERT_EQ(device.device_type, foundDevice.device_type);
    ASSERT_EQ(device.max_retries, foundDevice.max_retries);
    ASSERT_EQ(device.request_timeout_seconds, foundDevice.request_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_timeout_seconds, foundDevice.rtu_backoff_timeout_seconds);
    ASSERT_EQ(device.rtu_backoff_count, foundDevice.rtu_backoff_count);
    ASSERT_EQ(device.address, foundDevice.address);
    ASSERT_EQ(device.parameters, foundDevice.parameters);
    ASSERT_EQ(device.extended_parameters, foundDevice.extended_parameters);
}


class DeviceRepositoryDeleteTest : public DeviceRepositoryTest
{
public:
    DeviceRepositoryDeleteTest() : DeviceRepositoryTest("DeviceRepositoryDeleteTest") {}
    ~DeviceRepositoryDeleteTest() = default;

    void SetUp()
    {
        DeviceRepositoryTest::SetUp();
        DeviceRepository deviceRepositoryUnderTest(_dbContext);

        device.oid = deviceRepositoryUnderTest.createDevice(device);
    }
};

TEST_F(DeviceRepositoryDeleteTest, shouldUpdateName)
{
    DeviceRepository deviceRepositoryUnderTest(_dbContext);
    ASSERT_EQ(1, rowCountInTable("Device"));
    deviceRepositoryUnderTest.deleteDeviceWithOid(device.oid);
    ASSERT_EQ(0, rowCountInTable("Device"));
}
} /* namespace dios */
