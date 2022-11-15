#pragma once

#ifndef __REPOSITORY_H___
#define __REPOSITORY_H___

#include <string>
#include <sqlite3.h>

namespace dios::persist
{

class Repository
{
public:
    Repository() = delete;
    virtual ~Repository() = default;

protected:
    Repository(sqlite3* context);

    uint64_t Repository::executeInsert(const std::string& insertStatement);
    bool executeCommandInContext(const std::string &sqlCommand, sqlite3 *context);

private:
    sqlite3* _dbContext;
};

}
#endif /* __REPOSITORY_H___ */
