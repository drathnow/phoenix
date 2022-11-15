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

    iopoint_id_t insertIoPoint(const io_point_t& ioPoint);

    io_point_t ioPointForIoPointId(iopoint_id_t ioPointId);
};

}
#endif /* __IOPOINTREPOSITORY_H___ */
