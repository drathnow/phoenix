#pragma once

#ifndef __IOPOINTREPOSITORY_H___
#define __IOPOINTREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>
#include <vector>
#include <repositories.h>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

using namespace dios::domain;
using namespace dios::foundation;

class IOPointRepositoryHelper: public RepositoryHelper<IOPoint>
{
public:
    IOPointRepositoryHelper() = default;
    ~IOPointRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const IOPoint &ioPoint);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const IOPoint &ioPoint);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0);
    IOPoint* entityForSelectStatement(sqlite3_stmt *selectStatement);
};

class IOPointRepository: public IIOPointRepository, RepositoryTpl<IOPoint>
{
public:
    IOPointRepository() = delete;
    IOPointRepository(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new IOPointRepositoryHelper())
    {
    }
    ~IOPointRepository() = default;

    iopoint_id_t createIOPoint(IOPoint &ioPoint);
    int updateIOPoint(const IOPoint &ioPoint);
    int deleteIOPointWithOid(iopoint_id_t oid);
    IOPoint* ioPointForOid(iopoint_id_t oid);
    int ioPoints(std::vector<IOPoint*>& entityVector, int count, iopoint_id_t fromOid = 0);
};

}
#endif /* __IOPOINTREPOSITORY_H___ */
