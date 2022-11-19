#pragma once

#ifndef __ALARMLIMITSREPOSITORY_H___
#define __ALARMLIMITSREPOSITORY_H___

#include <sqlite3.h>
#include <cstdint>
#include <memory>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

class AlarmLimitsRepositoryHelper: public RepositoryHelper<alarm_limits>
{
public:
    AlarmLimitsRepositoryHelper() = default;
    ~AlarmLimitsRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const alarm_limits &entity);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const alarm_limits &entity);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    alarm_limits* entityForSelectStatement(sqlite3_stmt *selectStatement);
};

class AlarmLimitsRepositoryTpl: public RepositoryTpl<alarm_limits_t>
{
public:
    AlarmLimitsRepositoryTpl() = delete;
    AlarmLimitsRepositoryTpl(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new AlarmLimitsRepositoryHelper())
    {
    }
    ~AlarmLimitsRepositoryTpl() = default;
};

} /* namespace dios */

#endif /* __ALARMLIMITSREPOSITORY_H___ */
