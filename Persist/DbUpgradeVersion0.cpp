#include <log4cplus/log4cplus.h>
#include <sqlite3.h>
#include <iostream>

#include "DbUpgradeVersion0.h"
#include "persist.h"

#define BEGIN_TRANSACTION \
        if (executeCommandInContext("begin transaction;", dbContext) == false) \
            return false;

#define COMMIT \
        if (executeCommandInContext("commit;", dbContext) == false) {\
            executeCommandInContext("rollback;", dbContext); \
            return false; \
        }

namespace dios::persist
{

using namespace std;
using namespace log4cplus;

static Logger logger = Logger::getInstance("zios.DbUpgrader");

using io_point_t = struct io_point;

static bool executeCommandInContext(const string& sqlCommand, sqlite3* context)
{
    bool foo = SQLITE_OK == ::sqlite3_exec(context, sqlCommand.c_str(), NULL, 0, NULL);
    if (!foo)
        LOG4CPLUS_ERROR(Logger::getInstance("test"), "SQL Error: " << sqlCommand << " Error : " << ::sqlite3_errmsg(context));
    return foo;
}


bool DbUpgradeVersion0::doUpgrade(sqlite3 *context)
{
    if (executeCommandInContext("Create Table IF NOT EXISTS DbVersion "
                "( "
                "key    Varchar2 (50) PRIMARY KEY NOT NULL, "
                "value  Varchar2 (50) NOT NULL"
                ");", context) == false)
            return false;

    if (executeCommandInContext("Create Table IF NOT EXISTS IOPoint "
                "( "
                "oid            integer primary key, "
                "name           text not null, "
                "io_point_type  integer not null, "
                "data_type      integer not null ,"
                "device_id      integer default 0 not null, "
                "is_readonly    boolean default 0 not null, "
                "is_system      boolean default 0 not null, "
                "source_address text, "
                "display_hint   text"
                ");", context) == false)
            return false;

    if (executeCommandInContext("Create Table IF NOT EXISTS AlarmLimits "
                "( "
                "oid                    integer primary key, "
                "io_point_id            integer not null, "
                "set_time               integer not null ,"
                "clear_time             integer default 0 not null, "
                "alarm_mask             integer default 0 not null, "
                "high_high_set_limit    varchar2 (50) default '0' not null, "
                "high_high_clear_limit  varchar2 (50) default '0' not null, "
                "high_set_limit         varchar2 (50) default '0' not null, "
                "high_clear_limit       varchar2 (50) default '0' not null, "
                "low_low_set_limit      varchar2 (50) default '0' not null, "
                "low_low_clear_limit    varchar2 (50) default '0' not null, "
                "low_set_limit          varchar2 (50) default '0' not null, "
                "low_clear_limit        varchar2 (50) default '0' not null"
                ")", context) == false)
            return false;

    if (executeCommandInContext("Create Table IF NOT EXISTS Device "
                "( "
                "oid                        integer primary key, "
                "device_type                integer default 0 not null, "
                "name                       text not null, "
                "max_retries                integer default 0 not null, "
                "request_timeout_sec        integer default 0 not null, "
                "rtu_backoff_timeout_sec    integer default 0 not null, "
                "rtu_backoff_count          integer default 0 not null, "
                "address                    text not null, "
                "parameters                 text, "
                "extended_parameters        text"
                ");", context) == false)
            return false;

    if (executeCommandInContext("Create Table IF NOT EXISTS Deadband "
                "( "
                "oid            integer primary key, "
                "io_point_id    integer not null, "
                "deadband_type  integer default 0 not null, "
                "delta          string not null"
                ");", context) == false)
            return false;


    return true;
}

} /* namespace dios */
