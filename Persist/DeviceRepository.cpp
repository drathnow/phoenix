#include <sqlite3.h>
#include <cstring>

#include "DeviceRepository.h"

namespace dios::persist
{
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
static const char *SELECT_STMNT = "select name, device_type, max_retries, request_timeout_sec, rtu_backoff_timeout_sec, rtu_backoff_count, address, parameters, extended_parameters, oid from Device where oid = :oid";


DeviceRepository::DeviceRepository(sqlite3 *dbContext) : Repository(dbContext)
{
}

DeviceRepository::~DeviceRepository()
{
}

device_id_t DeviceRepository::createDevice(const device_t &device)
{
    sqlite3_stmt *statement;
    device_id_t result;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, device.name.c_str(), device.name.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEVICE_TYPE_IDX, device.device_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, MAX_RETRIES_IDX, device.max_retries), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, REQUEST_TIMEOUT_SEC_IDX, device.request_timeout_seconds), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX, device.rtu_backoff_timeout_seconds), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_COUNT_IDX, device.rtu_backoff_count), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, ADDRESS_IDX, device.address.c_str(), device.address.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, PARAMETERS_IDX, device.parameters.c_str(), device.parameters.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, EXTENDED_PARAMETERS_IDX, device.extended_parameters.c_str(), device.extended_parameters.length(), nullptr), -1);

    result = (SQLITE_DONE == ::sqlite3_step(statement)) ? ::sqlite3_last_insert_rowid(_dbContext) : -1;

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return result;
}

int DeviceRepository::updateDevice(const device_t &device)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, device.name.c_str(), device.name.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEVICE_TYPE_IDX, device.device_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, MAX_RETRIES_IDX, device.max_retries), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, REQUEST_TIMEOUT_SEC_IDX, device.request_timeout_seconds), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX, device.rtu_backoff_timeout_seconds), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, RTU_BACKOFF_COUNT_IDX, device.rtu_backoff_count), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, ADDRESS_IDX, device.address.c_str(), device.address.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, PARAMETERS_IDX, device.parameters.c_str(), device.parameters.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, EXTENDED_PARAMETERS_IDX, device.extended_parameters.c_str(), device.extended_parameters.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, device.oid), -1);

    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;
}

int DeviceRepository::deleteDeviceWithOid(device_id_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), -1);
    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;
}

device_t* DeviceRepository::deviceForOid(device_id_t oid, device_t &device)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, SELECT_STMNT, ::strlen(SELECT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);
    if (SQLITE_ROW != ::sqlite3_step(statement))
        return nullptr;

    device.oid = ::sqlite3_column_int64(statement, OID_IDX-1);
    device.name = (const char*) ::sqlite3_column_text(statement, NAME_IDX-1);
    device.device_type = (DeviceType)::sqlite3_column_int(statement, DEVICE_TYPE_IDX-1);
    device.max_retries = ::sqlite3_column_int(statement, MAX_RETRIES_IDX-1);
    device.request_timeout_seconds = ::sqlite3_column_int(statement, REQUEST_TIMEOUT_SEC_IDX);
    device.rtu_backoff_timeout_seconds = ::sqlite3_column_int(statement, RTU_BACKOFF_TIMEOUT_SEC_IDX);
    device.rtu_backoff_count = ::sqlite3_column_int(statement, RTU_BACKOFF_COUNT_IDX-1);
    device.address = (const char*) ::sqlite3_column_text(statement, ADDRESS_IDX-1);
    device.parameters =(const char*) ::sqlite3_column_text(statement, PARAMETERS_IDX-1);
    device.extended_parameters = (const char*) ::sqlite3_column_text(statement, EXTENDED_PARAMETERS_IDX-1);

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return &device;

}

} /* namespace dios */
