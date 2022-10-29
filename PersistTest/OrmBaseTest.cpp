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

    void OrmBaseTest::SetUp()
    {
        Directory dir("./db");
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
            LOG4CPLUS_ERROR(Logger::getInstance("test"), "Unable to check for initial table existence 'PersistenceAttributes'" << ::sqlite3_errmsg(_dbContext));
            return false;
        }
        return rowCount == 1;
    }

}
