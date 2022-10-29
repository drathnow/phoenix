#pragma once

#ifndef __DBUPGRADER_H___
#define __DBUPGRADER_H___

#include <map>
#include <sqlite3.h>

namespace dios::persist
{

class DbUpgrade
{
public:
    DbUpgrade() = default;
    virtual ~DbUpgrade() = default;

    virtual bool doUpgrade(sqlite3 *context) = 0;

    virtual int version() const = 0;
};

class DbUpgrader
{
public:
    DbUpgrader();
    ~DbUpgrader();

    bool doUpgrade(sqlite3 *context, int toVersion = -1);

    static int currentDbVersion(sqlite3 *dbContext);

private:
    std::map<int, DbUpgrade*> _dbUpgrades;

    bool _didUpdateDbVersionInContext(int version, sqlite3 *dbContext);
};

}
#endif /* __DBUPGRADER_H___ */
