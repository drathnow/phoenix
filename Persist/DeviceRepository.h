#pragma once

#ifndef __DEVICEREPOSITORY_H___
#define __DEVICEREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

class DeviceRepositoryHelper: public RepositoryHelper<device_t>
{
public:
    DeviceRepositoryHelper() = default;
    ~DeviceRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const device_t &entity);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const device_t &entity);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    device_t* entityForSelectStatement(sqlite3_stmt *selectStatement);
    sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0);

};

class DeviceRepositoryTpl: public RepositoryTpl<device_t>
{
public:
    DeviceRepositoryTpl() = delete;
    DeviceRepositoryTpl(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new DeviceRepositoryHelper())
    {
    }
    ~DeviceRepositoryTpl() = default;
};

} /* namespace dios */

#endif /* __DEVICEREPOSITORY_H___ */
