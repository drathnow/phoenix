#include <string>
#include <sqlite3.h>
#include <cstring>
#include <log4cplus/log4cplus.h>

#include "DeadbandRepository.h"

namespace dios::persist
{

using namespace std;
using namespace log4cplus;

static const int IO_POINT_ID_IDX = 1;
static const int DEADBAND_TYPE_IDX = 2;
static const int DELTA_IDX = 3;
static const int OID_IDX = 4;

static const char *INSERT_STMNT =
        "insert into Deadband (io_point_id, deadband_type, delta) values "
                "(:io_point_id, :deadband_type, :delta)";

static const char *UPDATE_STMNT = "update Deadband "
        " set io_point_id = :io_point_id"
        ", deadband_type = :deadband_type"
        ", delta = :delta"
        " where oid = :oid";

static const char *DELETE_STMNT = "delete from Deadband where oid = :oid";
static const char *SELECT_STMNT =
        "select io_point_id, deadband_type, delta, oid from Deadband";


sqlite3_stmt* DeadbandRepositoryHelper::insertStatementForEntity(sqlite3* dbContext, const deadband_t &deadband)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, deadband.io_point_id), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEADBAND_TYPE_IDX, deadband.deadband_type), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DELTA_IDX, deadband.delta.c_str(), deadband.delta.length(), nullptr), nullptr);

    return statement;
}

sqlite3_stmt* DeadbandRepositoryHelper::updateStatementForEntity(sqlite3* dbContext, const deadband_t &deadband)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), nullptr);

    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, deadband.io_point_id), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEADBAND_TYPE_IDX, deadband.deadband_type), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DELTA_IDX, deadband.delta.c_str(), deadband.delta.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, deadband.oid), nullptr);

    return statement;
}

sqlite3_stmt* DeadbandRepositoryHelper::deleteStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* DeadbandRepositoryHelper::selectStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    string selectSql(SELECT_STMNT);
    selectSql.append(" where oid = :oid");

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, selectSql.c_str(), selectSql.length(), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}


sqlite3_stmt* DeadbandRepositoryHelper::multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid)
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

deadband_t* DeadbandRepositoryHelper::entityForSelectStatement(sqlite3_stmt *statement)
{
    deadband_t* deadband = new deadband_t;

    deadband->oid = ::sqlite3_column_int64(statement, OID_IDX - 1);
    deadband->io_point_id = ::sqlite3_column_int64(statement, IO_POINT_ID_IDX - 1);
    deadband->deadband_type = (DeadbandType)::sqlite3_column_int(statement, DEADBAND_TYPE_IDX - 1);
    deadband->delta = (char*) ::sqlite3_column_text(statement, DELTA_IDX - 1);

    return deadband;
}

} /* namespace dios */
