#pragma once

#ifndef __DEVICEREPOSITORY_H___
#define __DEVICEREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

class DeviceRepository: public Repository
{
public:
    DeviceRepository() = delete;
    DeviceRepository(sqlite3* dbContext);
    ~DeviceRepository();

    device_id_t createDevice(const device_t& device);
    int updateDevice(const device_t &device);
    int deleteDeviceWithOid(device_id_t oid);
    device_t* deviceForOid(device_id_t oid);
};

} /* namespace dios */

#endif /* __DEVICEREPOSITORY_H___ */
