#pragma once

#ifndef __REPOSITORY_H___
#define __REPOSITORY_H___

#include <string>
#include <sqlite3.h>

#define RETURN_IF_SQLERROR(expr, value) \
    if (expr != SQLITE_OK)              \
    {                                   \
        return value;                   \
    }

namespace dios::persist
{

class Repository
{
public:
    Repository() = delete;
    virtual ~Repository() = default;

protected:
    Repository(sqlite3* context);

    uint64_t executeInsert(const std::string& insertStatement);
    bool executeCommandInContext(const std::string &sqlCommand, sqlite3 *context);
    int executeCommandInContext(const char *command, sqlite3_callback callback, void *arg, sqlite3 *sqlContext);

protected:
    sqlite3* _dbContext;
};

}
#endif /* __REPOSITORY_H___ */
