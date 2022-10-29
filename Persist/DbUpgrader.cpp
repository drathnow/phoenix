#include <log4cplus/log4cplus.h>
#include <string>
#include <StringHelper.h>
#include <cstring>
#include <mutex>
#include <iomanip>
#include <algorithm>
#include <cstdio>

#include "DbUpgrader.h"

#include "DbUpgradeVersion0.h"

namespace dios::persist
{

using namespace std;
using namespace log4cplus;
using namespace dios::foundation;

static mutex upgradeLock;

static const char *TABLE_SQL = "select name from sqlite_master where type='table' and name='DbVersion';";
static const char *VERSION_SQL = "select value from DbVersion where key = 'VERSION'";
static const char *VER_PREFIX = "ZDB";

static Logger logger = Logger::getInstance("dios::persist.DbUpgrader");

static int tableProcess(void *pArg, int argc, char **argv, char **columnName)
{
    if (NULL != pArg)
    {
        int *pInt = (int*) pArg;
        (*pInt)++;
        return 0;
    }
    return -1;
}

static int grabVersion(void *pArg, int argc, char **argv, char **columnName)
{
    if (NULL != pArg && argc == 1)
    {
        string version(&argv[0][::strlen(VER_PREFIX)]);
        uint32_t *pInt = (uint32_t*) pArg;
        StringHelper::stringToNumber(version, *pInt);
        return 0;
    }
    return -1;
}

DbUpgrader::DbUpgrader()
{
    _dbUpgrades.insert(pair<int, DbUpgrade*>(0, new DbUpgradeVersion0()));
}

DbUpgrader::~DbUpgrader()
{
    for_each(_dbUpgrades.begin(), _dbUpgrades.end(), [](pair<int, DbUpgrade*> p){delete p.second;});
    _dbUpgrades.clear();
}

int DbUpgrader::currentDbVersion(sqlite3 *dbContext)
{
    int rowCount = 0;
    if (SQLITE_OK != ::sqlite3_exec(dbContext, TABLE_SQL, tableProcess, (void*) &rowCount, NULL))
    {
        LOG4CPLUS_ERROR(logger, "Unable to check for initial table existence 'PersistenceAttributes'" << ::sqlite3_errmsg(dbContext));
        return -1;
    }

    int currentVersion = 0;
    if (rowCount != 0)
    {
        if (SQLITE_OK != ::sqlite3_exec(dbContext, VERSION_SQL, grabVersion, (void*) &currentVersion, NULL))
        {
            LOG4CPLUS_ERROR(logger, "Unable to get DB version number" << ::sqlite3_errmsg(dbContext));
            return -1;
        }
    }
    return currentVersion;
}

bool DbUpgrader::doUpgrade(sqlite3 *dbContext, int toVersion)
{
    unique_lock<mutex> lock;

    int rowCount = 0;
    if (SQLITE_OK != ::sqlite3_exec(dbContext, TABLE_SQL, tableProcess, (void*) &rowCount, NULL))
    {
        LOG4CPLUS_ERROR(logger, "Unable to check for initial table existence 'PersistenceAttributes'" << ::sqlite3_errmsg(dbContext));
        upgradeLock.unlock();
        return false;
    }

    int currentVersion = 0;
    if (rowCount != 0)
    {
        if (SQLITE_OK != ::sqlite3_exec(dbContext, VERSION_SQL, grabVersion, (void*) &currentVersion, NULL))
        {
            LOG4CPLUS_ERROR(logger, "Unable to get DB version number" << ::sqlite3_errmsg(dbContext));
            upgradeLock.unlock();
            return false;
        }
        currentVersion++;
    }

    int lastUpgrade = (toVersion == -1) ? _dbUpgrades.size() : std::min(toVersion, (int)_dbUpgrades.size());

    for (int i = currentVersion; i < lastUpgrade; i++)
    {
        map<int, DbUpgrade*>::const_iterator iter = _dbUpgrades.find(i);
        if (iter == _dbUpgrades.end())
        {
            LOG4CPLUS_ERROR(logger, "DB Upgrader failed: Missing upgrader numbered " << i);
            upgradeLock.unlock();
            return false;
        }

        LOG4CPLUS_INFO(logger, "Executing DbUpgradeVersion " << iter->second->version());
        if (iter->second->doUpgrade(dbContext) == false)
        {
            LOG4CPLUS_ERROR(logger, "DB Upgrader " << i << " failed");
            upgradeLock.unlock();
            return false;
        }

        bool foo = _didUpdateDbVersionInContext(iter->second->version(), dbContext);
        if (foo == false)
        {
            LOG4CPLUS_ERROR(logger, "DB Upgrader failed: Unable to upgrade DB version to " << i);
            upgradeLock.unlock();
            return false;
        }
    }

    return true;
}

bool DbUpgrader::_didUpdateDbVersionInContext(int version, sqlite3 *dbContext)
{
    ostringstream command;
    command << "update DbVersion set value = '" << VER_PREFIX << setfill('0') << setw(5) << version << "' where key = 'VERSION';";
    if (SQLITE_OK != ::sqlite3_exec(dbContext, command.str().c_str(), NULL, NULL, NULL))
    {
        LOG4CPLUS_ERROR(logger, "Unable update DB version number'" << ::sqlite3_errmsg(dbContext));
        return false;
    }
    return true;
}

}
