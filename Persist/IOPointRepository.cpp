#include <string>
#include <sqlite3.h>
#include <cstring>
#include <log4cplus/log4cplus.h>

#include "IOPointRepository.h"


namespace dios::persist
{

using namespace std;
using namespace log4cplus;

static const int NAME_IDX = 1;
static const int IO_POINT_TYPE_IDX = 2;
static const int DATA_TYPE_IDX = 3;
static const int DEVICE_ID_IDX = 4;
static const int IS_READONLY_IDX = 5;
static const int IS_SYSTEM_IDX = 6;
static const int SOURCE_ADDRESS_IDX = 7;
static const int DISPLAY_HINT_IDX = 8;
static const int OID_IDX = 9;


static const char *INSERT_STMNT = "insert into IOPoint (name, io_point_type, data_type, device_id, is_readonly, is_system, source_address, display_hint) values "
        "(:name, :io_point_type, :data_type, :device_id, :is_readonly, :is_system, :source_address, :display_hint)";

static const char *UPDATE_STMNT = "update IOPoint set name = :name"
        ", io_point_type = :io_point_type"
        ", data_type = :data_type"
        ", device_id = :device_id"
        ", is_readonly = :is_readonly"
        ", is_system = :is_system"
        ", source_address = :source_address"
        ", display_hint = :display_hint where oid = :oid";

static const char *DELETE_STMNT = "delete from IOPoint where oid = :oid";
static const char *SELECT_STMNT = "select name, io_point_type, data_type, device_id, is_readonly, is_system, source_address, display_hint, oid from IOPoint where oid = :oid";

IOPointRepository::IOPointRepository(sqlite3 *dbContext) :
        Repository(dbContext)
{
}

IOPointRepository::~IOPointRepository()
{
}

iopoint_id_t IOPointRepository::createIoPoint(const io_point_t &ioPoint)
{
    sqlite3_stmt *statement;
    iopoint_id_t result;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, ioPoint.name.c_str(), ioPoint.name.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IO_POINT_TYPE_IDX, ioPoint.io_point_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DATA_TYPE_IDX, ioPoint.data_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, DEVICE_ID_IDX, ioPoint.device_id), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_READONLY_IDX, ioPoint.readonly ? 1 : 0), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_SYSTEM_IDX, ioPoint.system ? 1 : 0), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, SOURCE_ADDRESS_IDX, ioPoint.source_address.length() == 0 ? nullptr : ioPoint.source_address.c_str(), ioPoint.source_address.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DISPLAY_HINT_IDX, ioPoint.display_hint.length() == 0 ? nullptr : ioPoint.display_hint.c_str(), ioPoint.display_hint.length(), nullptr), -1);

    result = (SQLITE_DONE == ::sqlite3_step(statement)) ? ::sqlite3_last_insert_rowid(_dbContext) : -1;

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return result;
}

int IOPointRepository::updateIoPoint(const io_point_t &ioPoint)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, ioPoint.name.c_str(), ioPoint.name.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IO_POINT_TYPE_IDX, ioPoint.io_point_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DATA_TYPE_IDX, ioPoint.data_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, DEVICE_ID_IDX, ioPoint.device_id), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_READONLY_IDX, ioPoint.readonly ? 1 : 0), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_SYSTEM_IDX, ioPoint.system ? 1 : 0), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, SOURCE_ADDRESS_IDX, ioPoint.source_address.length() == 0 ? nullptr : ioPoint.source_address.c_str(), ioPoint.source_address.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DISPLAY_HINT_IDX, ioPoint.display_hint.length() == 0 ? nullptr : ioPoint.display_hint.c_str(), ioPoint.display_hint.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, ioPoint.oid), -1);

    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;
}

int IOPointRepository::deleteIoPointWithOid(const iopoint_id_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), -1);
    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;
}

io_point_t* IOPointRepository::ioPointForOid(iopoint_id_t oid)
{
    sqlite3_stmt *statement;
    char *ptr;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, SELECT_STMNT, ::strlen(SELECT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);
    if (SQLITE_ROW != ::sqlite3_step(statement))
        return nullptr;

    io_point_t* ioPoint = new io_point_t;
    ioPoint->oid = ::sqlite3_column_int64(statement, OID_IDX-1);
    ioPoint->name = (const char*) ::sqlite3_column_text(statement, NAME_IDX-1);
    ioPoint->io_point_type = (IOPointType) ::sqlite3_column_int(statement, IO_POINT_TYPE_IDX-1);
    ioPoint->data_type = (DataType) ::sqlite3_column_int(statement, DATA_TYPE_IDX-1);
    ioPoint->device_id = ::sqlite3_column_int64(statement, DEVICE_ID_IDX-1);
    ioPoint->readonly = ::sqlite3_column_int(statement, IS_READONLY_IDX-1) == 1 ? true : false;
    ioPoint->system = ::sqlite3_column_int(statement, IS_SYSTEM_IDX-1) == 1 ? true : false;
    ptr = (char*) ::sqlite3_column_text(statement, SOURCE_ADDRESS_IDX-1);
    ioPoint->source_address = ptr == nullptr ? "" : ptr;
    ptr = (char*) ::sqlite3_column_text(statement, DISPLAY_HINT_IDX-1);
    ioPoint->display_hint = ptr == nullptr ? "" : ptr;

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return ioPoint;
}

}
