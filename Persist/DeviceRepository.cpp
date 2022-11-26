#include <sqlite3.h>
#include <string>
#include <cstring>

#include "DeviceRepository.h"

namespace dios::persist
{

using namespace std;
using namespace dios::domain;

static const int NAME_IDX = 1;
static const int DEVICE_TYPE_IDX = 2;
static const int MAX_RETRIES_IDX = 3;
static const int REQUEST_TIMEOUT_SEC_IDX = 4;
static const int RTU_BACKOFF_TIMEOUT_SEC_IDX = 5;
static const int RTU_BACKOFF_COUNT_IDX = 6;
static const int ADDRESS_IDX = 7;
static const int PARAMETERS_IDX = 8;
static const int EXTENDED_PARAMETERS_IDX = 9;
static const int OID_IDX = 10;

static const char *INSERT_STMNT = "insert into Device (name, device_type, max_retries, request_timeout_sec, rtu_backoff_timeout_sec, rtu_backoff_count, address, parameters, extended_parameters) values "
        "(:name, :device_type, :max_retries, :request_timeout_sec, :rtu_backoff_timeout_sec, :rtu_backoff_count, :address, :parameters, :extended_parameters)";

static const char *UPDATE_STMNT = "update Device set name = :name"
        ", device_type = :device_type"
        ", max_retries = :max_retries"
        ", request_timeout_sec = :request_timeout_sec"
        ", rtu_backoff_timeout_sec = :rtu_backoff_timeout_sec"
        ", rtu_backoff_count = :rtu_backoff_count"
        ", address = :address"
        ", parameters = :parameters"
        ", extended_parameters = :extended_parameters"
        "  where oid = :oid";

static const char *DELETE_STMNT = "delete from Device where oid = :oid";
static const char *SELECT_STMNT = "select name, device_type, max_retries, request_timeout_sec, rtu_backoff_timeout_sec, rtu_backoff_count, address, parameters, extended_parameters, oid from Device";

sqlite3_stmt* DeviceRepositoryHelper::insertStatementForEntity(sqlite3 *dbContext, const device_t &device)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, device.name.c_str(), device.name.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEVICE_TYPE_IDX, device.device_type), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, MAX_RETRIES_IDX, device.max_retries), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, REQUEST_TIMEOUT_SEC_IDX, device.request_timeout_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX, device.rtu_backoff_timeout_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_COUNT_IDX, device.rtu_backoff_count), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, ADDRESS_IDX, device.address.c_str(), device.address.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, PARAMETERS_IDX, device.parameters.c_str(), device.parameters.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, EXTENDED_PARAMETERS_IDX, device.extended_parameters.c_str(), device.extended_parameters.length(), nullptr), nullptr);

    return statement;
}

sqlite3_stmt* DeviceRepositoryHelper::updateStatementForEntity(sqlite3 *dbContext, const device_t &device)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, device.name.c_str(), device.name.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEVICE_TYPE_IDX, device.device_type), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, MAX_RETRIES_IDX, device.max_retries), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, REQUEST_TIMEOUT_SEC_IDX, device.request_timeout_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX, device.rtu_backoff_timeout_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_COUNT_IDX, device.rtu_backoff_count), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, ADDRESS_IDX, device.address.c_str(), device.address.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, PARAMETERS_IDX, device.parameters.c_str(), device.parameters.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, EXTENDED_PARAMETERS_IDX, device.extended_parameters.c_str(), device.extended_parameters.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, device.oid), nullptr);

    return statement;
}

sqlite3_stmt* DeviceRepositoryHelper::deleteStatementForOid(sqlite3 *dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* DeviceRepositoryHelper::selectStatementForOid(sqlite3 *dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    string selectSql(SELECT_STMNT);
    selectSql.append(" where oid = :oid");

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, selectSql.c_str(), selectSql.length(), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}


sqlite3_stmt* DeviceRepositoryHelper::multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid)
{
    sqlite3_stmt *statement;

    string selectSql(SELECT_STMNT);

    if (fromOid > 0)
    {
        selectSql.append(" where oid > :oid");
    }

    selectSql.append(" order by oid asc");

    if (count > 0)
    {
        selectSql.append(" limit :maxLimit");
    }

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, selectSql.c_str(), selectSql.length(), &statement, nullptr), nullptr);

    int idx = 1;
    if (fromOid > 0)
    {
        RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, idx++, fromOid), nullptr);
    }
    if (count > 0)
    {
        RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, idx++, count), nullptr);
    }

    return statement;
}


device_t* DeviceRepositoryHelper::entityForSelectStatement(sqlite3_stmt *statement)
{
    device_t *device = new device_t;

    device->oid = ::sqlite3_column_int64(statement, OID_IDX - 1);
    device->name = (const char*) ::sqlite3_column_text(statement, NAME_IDX - 1);
    device->device_type = (DeviceType) ::sqlite3_column_int(statement, DEVICE_TYPE_IDX - 1);
    device->max_retries = ::sqlite3_column_int(statement, MAX_RETRIES_IDX - 1);
    device->request_timeout_seconds = ::sqlite3_column_int(statement, REQUEST_TIMEOUT_SEC_IDX - 1);
    device->rtu_backoff_timeout_seconds = ::sqlite3_column_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX - 1);
    device->rtu_backoff_count = ::sqlite3_column_int(statement, RTU_BACKOFF_COUNT_IDX - 1);
    device->address = (const char*) ::sqlite3_column_text(statement, ADDRESS_IDX - 1);
    device->parameters = (const char*) ::sqlite3_column_text(statement, PARAMETERS_IDX - 1);
    device->extended_parameters = (const char*) ::sqlite3_column_text(statement, EXTENDED_PARAMETERS_IDX - 1);

    return device;
}


device_id_t DeviceRepository::createDevice(device_t &device)
{
    device_id_t oid = createEntity(device);
    if (oid > 0)
        device.oid = oid;
    return oid;
}

int DeviceRepository::updateDevice(const device_t &device)
{
    return updateEntity(device);
}

int DeviceRepository::deleteDeviceWithOid(const device_id_t oid)
{
    return deleteEntityWithOid(oid);
}

device_t* DeviceRepository::deviceForOid(device_id_t oid)
{
    return entityForOid(oid);
}

int DeviceRepository::devices(std::vector<device_t*>& devices, int count, device_id_t fromOid)
{
    return entities(devices, count, (uint64_t)fromOid);

}


} /* namespace dios */
