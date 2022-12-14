#pragma once

#ifndef __REPOSITORY_H___
#define __REPOSITORY_H___

#include <string>
#include <sqlite3.h>
#include <memory>

#define RETURN_IF_SQLERROR(expr, value) \
    if (expr != SQLITE_OK)              \
    {                                   \
        return value;                   \
    }

namespace dios::persist
{
template<class E>
class RepositoryHelper
{
public:
    RepositoryHelper() = default;
    RepositoryHelper(RepositoryHelper<E> &&rhs) = delete;
    RepositoryHelper(const RepositoryHelper<E> &rhs) = delete;
    RepositoryHelper& operator=(const RepositoryHelper&) = delete;
    virtual ~RepositoryHelper() = default;

    virtual sqlite3_stmt* insertStatementForEntity(sqlite3 *dbContext, const E &entity) = 0;
    virtual sqlite3_stmt* updateStatementForEntity(sqlite3 *dbContext, const E &entity) = 0;
    virtual sqlite3_stmt* deleteStatementForOid(sqlite3 *dbContext, uint64_t oid) = 0;
    virtual sqlite3_stmt* selectStatementForOid(sqlite3 *dbContext, uint64_t oid) = 0;
    virtual sqlite3_stmt* multipleSelectStatementFromOid(sqlite3 *dbContext, int count, uint64_t fromOid = 0) = 0;
    virtual E* entityForSelectStatement(sqlite3_stmt *selectStatement) = 0;
};

template<typename E>
class RepositoryTpl
{
public:
    RepositoryTpl() = delete;
    virtual ~RepositoryTpl()
    {
    }

protected:
    RepositoryTpl(sqlite3 *context, RepositoryHelper<E> *repositoryHelper) :
            _repositoryHelper{repositoryHelper}, _dbContext(context)
    {
    }

public:
    uint64_t createEntity(const E &entity)
    {
        sqlite3_stmt *statement = _repositoryHelper->insertStatementForEntity(_dbContext, entity);

        uint64_t result = (SQLITE_DONE == ::sqlite3_step(statement)) ? ::sqlite3_last_insert_rowid(_dbContext) : -1;

        ::sqlite3_reset(statement);
        ::sqlite3_finalize(statement);

        return result;

    }

    int updateEntity(const E &entity)
    {
        sqlite3_stmt *statement = _repositoryHelper->updateStatementForEntity(_dbContext, entity);

        int rc = ::sqlite3_step(statement);

        ::sqlite3_reset(statement);
        ::sqlite3_finalize(statement);

        return rc == SQLITE_DONE ? 0 : -1;
    }

    int deleteEntityWithOid(uint64_t oid)
    {
        sqlite3_stmt *statement = _repositoryHelper->deleteStatementForOid(_dbContext, oid);

        int rc = ::sqlite3_step(statement);
        ::sqlite3_reset(statement);
        ::sqlite3_finalize(statement);

        return rc == SQLITE_DONE ? 0 : -1;
    }

    E* entityForOid(uint64_t oid)
    {
        sqlite3_stmt *statement = _repositoryHelper->selectStatementForOid(_dbContext, oid);
        if (statement == nullptr)
            return nullptr;

        if (SQLITE_ROW != ::sqlite3_step(statement))
            return nullptr;

        E *entity = _repositoryHelper->entityForSelectStatement(statement);

        ::sqlite3_reset(statement);
        ::sqlite3_finalize(statement);

        return entity;
    }

    int entities(std::vector<E*>& entityVector, int count, uint64_t fromOid = 0)
    {
        sqlite3_stmt *statement = _repositoryHelper->multipleSelectStatementFromOid(_dbContext, count, fromOid);

        int fetchCount = 0;
        while (SQLITE_ROW == ::sqlite3_step(statement))
        {
            E *entity = _repositoryHelper->entityForSelectStatement(statement);
            entityVector.push_back(entity);
            fetchCount++;
        }

        ::sqlite3_reset(statement);
        ::sqlite3_finalize(statement);

        return fetchCount;
    }

private:
    std::unique_ptr<RepositoryHelper<E>> _repositoryHelper;
    sqlite3 *_dbContext;
};


}
#endif /* __REPOSITORY_H___ */
