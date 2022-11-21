#pragma once

#ifndef __IOPOINTREPOSITORY_H___
#define __IOPOINTREPOSITORY_H___

#include <sqlite3.h>
#include <foundation.h>
#include <vector>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

using namespace dios::foundation;

class IOPointRepositoryHelper: public RepositoryHelper<io_point_t>
{
public:
    IOPointRepositoryHelper() = default;
    ~IOPointRepositoryHelper() = default;

    sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const io_point_t &entity);
    sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const io_point_t &entity);
    sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid);
    sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0);
    io_point_t* entityForSelectStatement(sqlite3_stmt *selectStatement);
};

class IOPointRepositoryTpl: public RepositoryTpl<io_point_t>
{
public:
    IOPointRepositoryTpl() = delete;
    IOPointRepositoryTpl(sqlite3 *dbContext) :
            RepositoryTpl(dbContext, new IOPointRepositoryHelper())
    {
    }
    ~IOPointRepositoryTpl() = default;
};

//
//class IOPointRepository : public Repository
//{
//public:
//    IOPointRepository() = delete;
//    IOPointRepository(sqlite3 *dbContext);
//    ~IOPointRepository();
//
//    iopoint_id_t createIoPoint(const io_point_t& ioPoint);
//    int updateIoPoint(const io_point_t &ioPoint);
//    int deleteIoPointWithOid(const iopoint_id_t oid);
//    io_point_t* ioPointForOid(iopoint_id_t oid);
//    int ioPoints(std::vector<io_point>& ioPoints, int count, iopoint_id_t fromOid = 0);
//};

}
#endif /* __IOPOINTREPOSITORY_H___ */
