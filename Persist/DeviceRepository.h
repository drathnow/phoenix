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

class DeviceRepository: public Repository
{
public:
    DeviceRepository() = delete;
    DeviceRepository(sqlite3 *dbContext);
    ~DeviceRepository();

    device_id_t createDevice(const device_t &device);
    int updateDevice(const device_t &device);
    int deleteDeviceWithOid(device_id_t oid);
    device_t* deviceForOid(device_id_t oid);
};

} /* namespace dios */

#endif /* __DEVICEREPOSITORY_H___ */
