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
        "select io_point_id, deadband_type, delta, oid from Deadband where oid = :oid";

DeadbandRepository::DeadbandRepository(sqlite3 *dbContext) :
        Repository(dbContext)
{
}

int64_t DeadbandRepository::createDeadband(const deadband_t &deadband)
{
    sqlite3_stmt *statement;
    int64_t result;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, deadband.io_point_id), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEADBAND_TYPE_IDX, deadband.deadband_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DELTA_IDX, deadband.delta.c_str(), deadband.delta.length(), nullptr), -1);

    result = (SQLITE_DONE == ::sqlite3_step(statement)) ? ::sqlite3_last_insert_rowid(_dbContext) : -1;

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return result;
}

int DeadbandRepository::updateDeadband(const deadband_t &deadband)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), -1);

    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, deadband.io_point_id), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, DEADBAND_TYPE_IDX, deadband.deadband_type), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, DELTA_IDX, deadband.delta.c_str(), deadband.delta.length(), nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, OID_IDX, deadband.oid), -1);

    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;

}

int DeadbandRepository::deleteDeadbandWithOid(int64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), -1);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), -1);

    int rc = ::sqlite3_step(statement);
    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return rc == SQLITE_DONE ? 0 : -1;
}

deadband_t* DeadbandRepository::deadbandForDeadbandOid(int64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(_dbContext, SELECT_STMNT, ::strlen(SELECT_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);
    if (SQLITE_ROW != ::sqlite3_step(statement))
        return nullptr;

    deadband_t* deadband = new deadband_t;
    deadband->oid = ::sqlite3_column_int64(statement, OID_IDX - 1);
    deadband->io_point_id = ::sqlite3_column_int64(statement, IO_POINT_ID_IDX - 1);
    deadband->deadband_type = (DeadbandType)::sqlite3_column_int(statement, DEADBAND_TYPE_IDX - 1);
    deadband->delta = (char*) ::sqlite3_column_text(statement, DELTA_IDX - 1);

    ::sqlite3_reset(statement);
    ::sqlite3_finalize(statement);

    return deadband;
}

} /* namespace dios */
