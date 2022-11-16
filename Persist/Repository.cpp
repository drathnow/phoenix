#include <string>
#include <log4cplus/log4cplus.h>

#include "Repository.h"

namespace dios::persist
{

using namespace std;
using namespace log4cplus;

bool Repository::executeCommandInContext(const string &sqlCommand, sqlite3 *context)
{
    bool foo = SQLITE_OK == ::sqlite3_exec(context, sqlCommand.c_str(), NULL, 0, NULL);
    if (!foo)
        LOG4CPLUS_ERROR(Logger::getInstance("dios::persist"), "SQL Error: " << sqlCommand << " Error : " << ::sqlite3_errmsg(context));
    return foo;
}

int Repository::executeCommandInContext(const char *command, sqlite3_callback callback, void *arg, sqlite3 *sqlContext)
{
    int rc = ::sqlite3_exec(sqlContext, command, callback, (void*) arg, NULL);
    if (SQLITE_OK != rc)
        LOG4CPLUS_ERROR(Logger::getInstance("dios::persist"), "SQL Error: " << command << " Error : " << ::sqlite3_errmsg(sqlContext));
    return rc;
}


Repository::Repository(sqlite3 *context) :
        _dbContext(context)
{
}

uint64_t Repository::executeInsert(const string& insertStatement)
{
    if (!executeCommandInContext(insertStatement, _dbContext))
        return 0;

    return ::sqlite3_last_insert_rowid(_dbContext);
}


}
