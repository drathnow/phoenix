#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sqlite3.h>
#include <log4cplus/log4cplus.h>

#include "OrmBaseTest.h"

namespace dios::persist
{
    using namespace std;
    using namespace log4cplus;


    static int rowCounter(void* pArg, int argc, char **argv, char** columnName) {
        if (NULL != pArg) {
            int* pInt = (int*)pArg;
            (*pInt)++;
            return 0;
        }
        return -1;
    }

    static int selectCountCallback(void *returnCount, int argc, char **argv, char **columnName)
    {
        int* c = (int*) returnCount;
        *c = atoi(argv[0]);
        return 0;
    }
    void OrmBaseTest::SetUp()
    {
        Directory dir("/mnt/ramdisk/db");
        if (dir.exists())
            _dbFile.remove();
        else
            dir.mkdirs();

        int err;
        if (SQLITE_OK != (err = ::sqlite3_open(_dbFile.absolutePath(), &_dbContext)))
        {
            FAIL() << "Unable to open database file " << _dbFile.absolutePath() << ": " << ::sqlite3_errmsg(_dbContext) << " (" << "(Errorcode = " << err << ")" << endl;
        }
    }

    void OrmBaseTest::TearDown()
    {
        if (_dbContext != NULL)
            close();
        if (!_persist)
             _dbFile.remove();
    }

    void OrmBaseTest::close()
    {
        if (_dbContext != NULL)
        {
            ::sqlite3_close(_dbContext);
            ::sqlite3_shutdown();
        }
        _dbContext = NULL;
    }

    bool OrmBaseTest::tableExists(const std::string &tableName)
    {
        string sql("select name from sqlite_master where type='table' and name='");
        sql.append(tableName).append("';");

        int rowCount = 0;
        if (SQLITE_OK != ::sqlite3_exec(_dbContext, sql.c_str(), rowCounter, (void*) &rowCount, NULL))
        {
            LOG4CPLUS_ERROR(Logger::getInstance("dios.persis.test"), "Unable to check for initial table existence 'PersistenceAttributes'" << ::sqlite3_errmsg(_dbContext));
            return false;
        }
        return rowCount == 1;
    }

    bool OrmBaseTest::executeCommandInContext(const char* command)
    {
        bool foo = SQLITE_OK == ::sqlite3_exec(_dbContext, command, NULL, 0, NULL);
        if (!foo)
            LOG4CPLUS_ERROR(Logger::getInstance("dios.persist.test"), "SQL Error: " << command << " Error : " << ::sqlite3_errmsg(_dbContext));
        return foo;
    }

    int OrmBaseTest::executeCommandInContext(const char *command, sqlite3_callback callback, void *arg)
    {
        int rc = ::sqlite3_exec(_dbContext, command, callback, (void*)arg, NULL);
        if (SQLITE_OK != rc)
            LOG4CPLUS_ERROR(Logger::getInstance("dios.persist.test"), "SQL Error: " << command << " Error : " << ::sqlite3_errmsg(_dbContext));
        return rc;
    }

    int OrmBaseTest::rowCountInTable(const char *tableName, const char *whereClause)
    {
        int count = 0;
        string command("select count(*) from ");
        command.append(tableName);
        if (whereClause != NULL)
            command.append(" ")
                    .append(whereClause);

        int rc = ::sqlite3_exec(_dbContext, command.c_str(), selectCountCallback, &count, NULL);
        if (rc != SQLITE_OK) {
            LOG4CPLUS_ERROR(Logger::getInstance("dios.persist.test"), "SQL Error: " << command << " Error : " << ::sqlite3_errmsg(_dbContext));
            return -1;
        }
        return count;
    }

}
