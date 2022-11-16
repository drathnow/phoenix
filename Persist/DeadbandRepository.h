#pragma once

#ifndef __DEADBANDREPOSITORY_H___
#define __DEADBANDREPOSITORY_H___

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

class DeadbandRepository: public Repository
{
public:
    DeadbandRepository() = delete;
    DeadbandRepository(sqlite3* dbContext);
    ~DeadbandRepository() = default;

    int64_t createDeadband(const deadband_t& deadband);
    int updateDeadband(const deadband_t& deadband);
    int deleteDeadbandWithOid(int64_t oid);
    deadband_t* deadbandForOid(int64_t oid);
};

} /* namespace dios */

#endif /* __DEADBANDREPOSITORY_H___ */
