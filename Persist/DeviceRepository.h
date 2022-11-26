#pragma once

#ifndef __DEVICEREPOSITORY_H___
#define __DEVICEREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>
#include <repositories.h>
#include <Device.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{
using namespace dios::domain;

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

class DeviceRepository: public IDeviceRepository, RepositoryTpl<device_t>
{
public:
    DeviceRepository() = delete;
    DeviceRepository(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new DeviceRepositoryHelper())
    {
    }
    ~DeviceRepository() = default;

    device_id_t createDevice(device_t &device);
    int updateDevice(const device_t &device);
    int deleteDeviceWithOid(device_id_t oid);
    device_t* deviceForOid(device_id_t oid);
    int devices(std::vector<device_t*>& entityVector, int count, device_id_t fromOid = 0);
};

} /* namespace dios */

#endif /* __DEVICEREPOSITORY_H___ */
