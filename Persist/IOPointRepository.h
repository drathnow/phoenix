#pragma once

#ifndef __IOPOINTREPOSITORY_H___
#define __IOPOINTREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

using namespace dios::foundation;

class IOPointRepository : public Repository
{
public:
    IOPointRepository() = delete;
    IOPointRepository(sqlite3 *dbContext);
    ~IOPointRepository();

    iopoint_id_t createIoPoint(const io_point_t& ioPoint);
    int updateIoPoint(const io_point_t &ioPoint);
    int deleteIoPointWithOid(const iopoint_id_t oid);
    io_point_t* ioPointForOid(iopoint_id_t oid);
};

}
#endif /* __IOPOINTREPOSITORY_H___ */
