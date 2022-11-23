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
static const char *SELECT_STMNT = "select name, io_point_type, data_type, device_id, is_readonly, is_system, source_address, display_hint, oid from IOPoint";

sqlite3_stmt* IOPointRepositoryHelper::insertStatementForEntity(sqlite3* dbContext, const IOPoint &ioPoint)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, ioPoint.name().c_str(), ioPoint.name().length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IO_POINT_TYPE_IDX, ioPoint.pointType()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DATA_TYPE_IDX, ioPoint.dataType()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, DEVICE_ID_IDX, ioPoint.deviceId()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_READONLY_IDX, ioPoint.isReadonly() ? 1 : 0), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_SYSTEM_IDX, ioPoint.isSystem() ? 1 : 0), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, SOURCE_ADDRESS_IDX, ioPoint.sourceAddress().length() == 0 ? nullptr : ioPoint.sourceAddress().c_str(), ioPoint.sourceAddress().length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DISPLAY_HINT_IDX, ioPoint.displayHint().length() == 0 ? nullptr : ioPoint.displayHint().c_str(), ioPoint.displayHint().length(), nullptr), nullptr);

    return statement;
}

sqlite3_stmt* IOPointRepositoryHelper::updateStatementForEntity(sqlite3* dbContext, const IOPoint &ioPoint)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, NAME_IDX, ioPoint.name().c_str(), ioPoint.name().length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IO_POINT_TYPE_IDX, ioPoint.pointType()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DATA_TYPE_IDX, ioPoint.dataType()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, DEVICE_ID_IDX, ioPoint.deviceId()), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_READONLY_IDX, ioPoint.isReadonly() ? 1 : 0), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, IS_SYSTEM_IDX, ioPoint.isSystem() ? 1 : 0), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, SOURCE_ADDRESS_IDX, ioPoint.sourceAddress().length() == 0 ? nullptr : ioPoint.sourceAddress().c_str(), ioPoint.sourceAddress().length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DISPLAY_HINT_IDX, ioPoint.displayHint().length() == 0 ? nullptr : ioPoint.displayHint().c_str(), ioPoint.displayHint().length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, ioPoint.oid()), nullptr);

    return statement;
}

sqlite3_stmt* IOPointRepositoryHelper::deleteStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* IOPointRepositoryHelper::selectStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;
    string selectSql(SELECT_STMNT);

    selectSql.append(" where oid = :oid");
    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, selectSql.c_str(), selectSql.length(), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* IOPointRepositoryHelper::multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid)
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

IOPoint* IOPointRepositoryHelper::entityForSelectStatement(sqlite3_stmt *statement)
{
    IOPoint* ioPoint = new IOPoint;
    char* ptr;

    ioPoint->setOid(::sqlite3_column_int64(statement, OID_IDX-1));
    ioPoint->setName((const char*) ::sqlite3_column_text(statement, NAME_IDX-1));
    ioPoint->setPointType((IOPointType) ::sqlite3_column_int(statement, IO_POINT_TYPE_IDX-1));
    ioPoint->setDataType((DataType) ::sqlite3_column_int(statement, DATA_TYPE_IDX-1));
    ioPoint->setDeviceId(::sqlite3_column_int64(statement, DEVICE_ID_IDX-1));
    ioPoint->setReadonly(::sqlite3_column_int(statement, IS_READONLY_IDX-1) == 1 ? true : false);
    ioPoint->setSystem(::sqlite3_column_int(statement, IS_SYSTEM_IDX-1) == 1 ? true : false);
    ptr = (char*) ::sqlite3_column_text(statement, SOURCE_ADDRESS_IDX-1);
    ioPoint->setSourceAddress(ptr == nullptr ? "" : ptr);
    ptr = (char*) ::sqlite3_column_text(statement, DISPLAY_HINT_IDX-1);
    ioPoint->setDisplayHint(ptr == nullptr ? "" : ptr);

    return ioPoint;
}

iopoint_id_t IOPointRepository::createIOPoint(IOPoint &ioPoint)
{
    iopoint_id_t oid = createEntity(ioPoint);
    if (oid > 0)
        ioPoint.setOid(oid);
    return oid;
}

int IOPointRepository::updateIOPoint(const IOPoint &ioPoint)
{
    return updateEntity(ioPoint);
}

int IOPointRepository::deleteIOPointWithOid(const iopoint_id_t oid)
{
    return deleteEntityWithOid(oid);
}

IOPoint* IOPointRepository::ioPointForOid(iopoint_id_t oid)
{
    return entityForOid(oid);
}

int IOPointRepository::ioPoints(std::vector<IOPoint*>& ioPoints, int count, iopoint_id_t fromOid)
{
    return entities(ioPoints, count, (uint64_t)fromOid);

}


}
