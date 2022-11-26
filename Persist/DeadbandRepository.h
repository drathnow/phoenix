#pragma once

#ifndef __DEADBANDREPOSITORY_H___
#define __DEADBANDREPOSITORY_H___

#include <Deadband.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{
using namespace dios::domain;

class DeadbandRepositoryHelper: public RepositoryHelper<deadband_t>
{
public:
    DeadbandRepositoryHelper() = default;
    ~DeadbandRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const deadband_t &entity);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const deadband_t &entity);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    deadband_t* entityForSelectStatement(sqlite3_stmt *selectStatement);
    sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0);
};

class DeadbandRepository: public RepositoryTpl<deadband_t>
{
public:
    DeadbandRepository() = delete;
    DeadbandRepository(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new DeadbandRepositoryHelper())
    {
    }
    ~DeadbandRepository() = default;
};

} /* namespace dios */

#endif /* __DEADBANDREPOSITORY_H___ */
