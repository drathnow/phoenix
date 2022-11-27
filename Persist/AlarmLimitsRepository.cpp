#include <string>
#include <sqlite3.h>
#include <cstring>
#include <log4cplus/log4cplus.h>

#include "AlarmLimitsRepository.h"

namespace dios::persist
{

using namespace std;
using namespace log4cplus;

static const int IO_POINT_ID_IDX = 1;
static const int SET_TIME_IDX = 2;
static const int CLEAR_TIME_IDX = 3;
static const int ALARM_MASK_IDX = 4;
static const int HIGH_HIGH_SET_LIMIT_IDX = 5;
static const int HIGH_HIGH_CLEAR_LIMIT_IDX = 6;
static const int HIGH_SET_LIMIT_IDX = 7;
static const int HIGH_CLEAR_LIMIT_IDX = 8;
static const int LOW_LOW_SET_LIMIT_IDX = 9;
static const int LOW_LOW_CLEAR_LIMIT_IDX = 10;
static const int LOW_SET_LIMIT_IDX = 11;
static const int LOW_CLEAR_LIMIT_IDX = 12;
static const int OID_IDX = 13;

static const char *INSERT_STMNT =
        "insert into AlarmLimits (io_point_id, set_time, clear_time, alarm_mask, high_high_set_limit, high_high_clear_limit, high_set_limit, high_clear_limit, low_low_set_limit, low_low_clear_limit, low_set_limit, low_clear_limit) values "
                "(:io_point_id, :set_time, :clear_time, :no_data_enabled, :high_high_set_limit, :high_high_clear_limit, :high_set_limit, :high_clear_limit, :low_low_set_limit, :low_low_clear_limit, :low_set_limit, :low_clear_limit)";

static const char *UPDATE_STMNT = "update AlarmLimits "
        " set io_point_id = :io_point_id"
        ", set_time = :set_time_seconds"
        ", clear_time = :clear_time_seconds"
        ", alarm_mask = :alarm_mask"
        ", high_high_set_limit = :high_high_set_limit"
        ", high_high_clear_limit = :high_high_clear_limit"
        ", high_set_limit = :high_set_limit"
        ", high_clear_limit = :high_clear_limit"
        ", low_low_set_limit = :low_low_set_limit"
        ", low_low_clear_limit = :low_low_clear_limit"
        ", low_set_limit = :low_set_limit"
        ", low_clear_limit = :low_clear_limit"
        " where oid = :oid";

static const char *DELETE_STMNT = "delete from AlarmLimits where oid = :oid";
static const char *SELECT_STMNT =
        "select io_point_id, set_time, clear_time, alarm_mask, high_high_set_limit, high_high_clear_limit, high_set_limit, high_clear_limit, low_low_set_limit, low_low_clear_limit, low_set_limit, low_clear_limit, oid from AlarmLimits";

sqlite3_stmt* AlarmLimitsRepositoryHelper::insertStatementForEntity(sqlite3* dbContext, const alarm_limits &alarmLimits)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, INSERT_STMNT, ::strlen(INSERT_STMNT), &statement, nullptr), nullptr);

    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, alarmLimits.io_point_id), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, SET_TIME_IDX, alarmLimits.set_time_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, CLEAR_TIME_IDX, alarmLimits.clear_time_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, ALARM_MASK_IDX, alarmLimits.alarm_mask), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_HIGH_SET_LIMIT_IDX, alarmLimits.high_high_set_limit.c_str(), alarmLimits.high_high_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_HIGH_CLEAR_LIMIT_IDX, alarmLimits.high_high_clear_limit.c_str(), alarmLimits.high_high_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_SET_LIMIT_IDX, alarmLimits.high_set_limit.c_str(), alarmLimits.high_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_CLEAR_LIMIT_IDX, alarmLimits.high_clear_limit.c_str(), alarmLimits.high_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_LOW_SET_LIMIT_IDX, alarmLimits.low_low_set_limit.c_str(), alarmLimits.low_low_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_LOW_CLEAR_LIMIT_IDX, alarmLimits.low_low_clear_limit.c_str(), alarmLimits.low_low_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_SET_LIMIT_IDX, alarmLimits.low_set_limit.c_str(), alarmLimits.low_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_CLEAR_LIMIT_IDX, alarmLimits.low_clear_limit.c_str(), alarmLimits.low_clear_limit.length(), nullptr), nullptr);

    return statement;
}

sqlite3_stmt* AlarmLimitsRepositoryHelper::updateStatementForEntity(sqlite3* dbContext, const alarm_limits &alarmLimits)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, UPDATE_STMNT, ::strlen(UPDATE_STMNT), &statement, nullptr), nullptr);

    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, IO_POINT_ID_IDX, alarmLimits.io_point_id), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, SET_TIME_IDX, alarmLimits.set_time_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, CLEAR_TIME_IDX, alarmLimits.clear_time_seconds), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int(statement, ALARM_MASK_IDX, alarmLimits.alarm_mask), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_HIGH_SET_LIMIT_IDX, alarmLimits.high_high_set_limit.c_str(), alarmLimits.high_high_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_HIGH_CLEAR_LIMIT_IDX, alarmLimits.high_high_clear_limit.c_str(), alarmLimits.high_high_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_SET_LIMIT_IDX, alarmLimits.high_set_limit.c_str(), alarmLimits.high_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, HIGH_CLEAR_LIMIT_IDX, alarmLimits.high_clear_limit.c_str(), alarmLimits.high_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_LOW_SET_LIMIT_IDX, alarmLimits.low_low_set_limit.c_str(), alarmLimits.low_low_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_LOW_CLEAR_LIMIT_IDX, alarmLimits.low_low_clear_limit.c_str(), alarmLimits.low_low_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_SET_LIMIT_IDX, alarmLimits.low_set_limit.c_str(), alarmLimits.low_set_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_text(statement, LOW_CLEAR_LIMIT_IDX, alarmLimits.low_clear_limit.c_str(), alarmLimits.low_clear_limit.length(), nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 13, alarmLimits.oid), nullptr);

    return statement;
}

sqlite3_stmt* AlarmLimitsRepositoryHelper::deleteStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, DELETE_STMNT, ::strlen(DELETE_STMNT), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* AlarmLimitsRepositoryHelper::selectStatementForOid(sqlite3* dbContext, uint64_t oid)
{
    sqlite3_stmt *statement;

    string selectSql(SELECT_STMNT);
    selectSql.append(" where oid = :oid");

    RETURN_IF_SQLERROR(::sqlite3_prepare_v2(dbContext, selectSql.c_str(), selectSql.length(), &statement, nullptr), nullptr);
    RETURN_IF_SQLERROR(::sqlite3_bind_int64(statement, 1, oid), nullptr);

    return statement;
}

sqlite3_stmt* AlarmLimitsRepositoryHelper::multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid)
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


alarm_limits* AlarmLimitsRepositoryHelper::entityForSelectStatement(sqlite3_stmt *statement)
{
    alarm_limits_t *alarmLimits = new alarm_limits_t;
    alarmLimits->oid = ::sqlite3_column_int64(statement, OID_IDX - 1);
    alarmLimits->io_point_id = ::sqlite3_column_int64(statement, IO_POINT_ID_IDX - 1);
    alarmLimits->set_time_seconds = ::sqlite3_column_int(statement, SET_TIME_IDX - 1);
    alarmLimits->clear_time_seconds = ::sqlite3_column_int(statement, CLEAR_TIME_IDX - 1);
    alarmLimits->high_high_set_limit = (char*) ::sqlite3_column_text(statement, HIGH_HIGH_SET_LIMIT_IDX - 1);
    alarmLimits->high_high_clear_limit = (char*) ::sqlite3_column_text(statement, HIGH_HIGH_CLEAR_LIMIT_IDX - 1);
    alarmLimits->high_set_limit = (char*) ::sqlite3_column_text(statement, HIGH_SET_LIMIT_IDX - 1);
    alarmLimits->high_clear_limit = (char*) ::sqlite3_column_text(statement, HIGH_CLEAR_LIMIT_IDX - 1);
    alarmLimits->low_low_set_limit = (char*) ::sqlite3_column_text(statement, LOW_LOW_SET_LIMIT_IDX - 1);
    alarmLimits->low_low_clear_limit = (char*) ::sqlite3_column_text(statement, LOW_LOW_CLEAR_LIMIT_IDX - 1);
    alarmLimits->low_set_limit = (char*) ::sqlite3_column_text(statement, LOW_SET_LIMIT_IDX - 1);
    alarmLimits->low_clear_limit = (char*) ::sqlite3_column_text(statement, LOW_CLEAR_LIMIT_IDX - 1);

    return alarmLimits;
}


alarm_limits_id_t AlarmLimitsRepository::createAlarmLimits(alarm_limits_t &alarm_limits)
{
    alarm_limits_id_t oid = createEntity(alarm_limits);
    if (oid > 0)
        alarm_limits.oid = oid;
    return oid;
}

int AlarmLimitsRepository::updateAlarmLimits(const alarm_limits_t &alarm_limits)
{
    return updateEntity(alarm_limits);
}

int AlarmLimitsRepository::deleteAlarmLimitsWithOid(const alarm_limits_id_t oid)
{
    return deleteEntityWithOid(oid);
}

alarm_limits_t* AlarmLimitsRepository::alarmLimitsForOid(alarm_limits_id_t oid)
{
    return entityForOid(oid);
}

int AlarmLimitsRepository::alarmLimitses(std::vector<alarm_limits_t*>& alarmLimits, int count, alarm_limits_id_t fromOid)
{
    return entities(alarmLimits, count, (uint64_t)fromOid);

}


} /* namespace dios */
