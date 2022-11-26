#pragma once

#ifndef __ALARMLIMITSREPOSITORY_H___
#define __ALARMLIMITSREPOSITORY_H___

#include <sqlite3.h>
#include <cstdint>
#include <memory>
#include <AlarmConfiguration.h>

#include "repositories.h"
#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

	using namespace dios::domain;

class AlarmLimitsRepositoryHelper: public RepositoryHelper<alarm_limits_t>
{
public:
    AlarmLimitsRepositoryHelper() = default;
    ~AlarmLimitsRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const alarm_limits_t &entity);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const alarm_limits_t &entity);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0);
    alarm_limits* entityForSelectStatement(sqlite3_stmt *selectStatement);

};

class AlarmLimitsRepository: public IAlarmLimitsRepository, RepositoryTpl<alarm_limits_t>
{
public:
    AlarmLimitsRepository() = delete;
    AlarmLimitsRepository(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new AlarmLimitsRepositoryHelper())
    {
    }
    ~AlarmLimitsRepository() = default;

    alarm_limits_id_t createAlarmLimits(alarm_limits_t& alarm_limits);
    int updateAlarmLimits(const alarm_limits_t& alarm_limits);
    int deleteAlarmLimitsWithOid(alarm_limits_id_t oid);
    alarm_limits_t* alarmLimitsForOid(alarm_limits_id_t oid);
    int alarmLimitses(std::vector<alarm_limits_t*>& entityVector, int count, iopoint_id_t fromOid = 0);
};

} /* namespace dios */

#endif /* __ALARMLIMITSREPOSITORY_H___ */
