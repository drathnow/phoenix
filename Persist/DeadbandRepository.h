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

    deadband_id_t createDeadband(const deadband_t& deadband);
    int updateDeadband(const deadband_t& deadband);
    int deleteDeadbandWithOid(deadband_id_t oid);
    deadband_t* deadbandForOid(deadband_id_t oid);
};

} /* namespace dios */

#endif /* __DEADBANDREPOSITORY_H___ */
