/** @file dbupgrader.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug. 29, 2019 by daver:
 * @endhistory
 */

#include <getopt.h>
#include <string>
#include <iostream>
#include <StringUtl.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <File.h>
#include <sqlite3.h>
#include <DbUpgrader.h>

using namespace std;
using namespace log4cplus;
using namespace zios::orm;
using namespace zios::common;

#define DEFAULT_DB_FILE_PATH    "/zioshome/zios/db/zios.db"

static string dbFilePath(DEFAULT_DB_FILE_PATH);
static bool printVersion = false;

static void usage()
{
    cout << "Usage: dbupgrader [options]" << endl;
    cout << "Options:" << endl;
    cout << "     -f [FILE], --dbfile=[FILE]" << endl;
    cout << "                      Path to database FILE.  If FILE does not exist, it will be created." << endl;
    cout << "                      If no file is specified. /zioshome/zios/db/zios.db will be assumed." << endl;
    cout << "     -p, --print      Print DB version and exit" << endl;
    cout << "     -h, --help       Shows this text" << endl << endl;
    exit(1);
}

static void parseCommandLineArgs(int argc, char *argv[])
{

    static struct option longOptions[] = {
            { "dbfile",     required_argument, 0, 'f' },
            { "print",      no_argument, 0, 'p' },
            { "help",       no_argument, 0, 'h' },
            { 0, 0, 0, 0 } };
    int c;
    int optionIndex = 0;                   // will point to position in longOptions.

    while ((c = ::getopt_long(argc, argv, "f:hp", longOptions, &optionIndex)) != -1) {
        switch (c) {
            case '?':
                if (::isprint(optopt))
                    ::fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                usage();
                break;

            case 'f':
                dbFilePath = optarg;
                StringUtl::trim(dbFilePath);
                break;

            case 'p':
                printVersion = true;
                break;

            case 'h':
                default:
                usage();
        }
    }
}

int main(int argc, char **argv)
{
    int err = 0;
    sqlite3* sqlContext = NULL;
    bool dbExists = false;

    //
    // We use basic logging for this.  Nothing special
    //
    log4cplus::Initializer initializer;
    log4cplus::BasicConfigurator config;
    config.configure();

    Logger logger = Logger::getInstance("zios.dbupgrader");

    //
    // Go parse out the command line.
    //
    parseCommandLineArgs(argc, argv);

    File dbFile(dbFilePath);

    if ((dbExists = dbFile.exists()) == false)
        LOG4CPLUS_WARN(logger, "db file " << dbFile.absolutePath() << " does not exist.  It will be created");
    else if (printVersion == false)
        LOG4CPLUS_INFO(logger, "Upgrading file " << dbFile.absolutePath());

    dbFile.mkdirs();

    if (SQLITE_OK != (err = ::sqlite3_open(dbFile.absolutePath(), &sqlContext))) {
        LOG4CPLUS_FATAL(logger, "Unable to create db file '" << dbFile.absolutePath() << "': " << ::sqlite3_errmsg(sqlContext));
        exit(-1);
    }

    if (printVersion && dbExists)
        LOG4CPLUS_INFO(logger, "db file " << dbFile.absolutePath() << " is currently at version " << DbUpgrader::currentDbVersion(sqlContext));
    else {
        DbUpgrader dbUpgrader;
        if (dbUpgrader.doUpgrade(sqlContext) == false)
            LOG4CPLUS_ERROR(logger, "Errors were detected during upgrade. Upgrade failed!");
        else
            LOG4CPLUS_INFO(logger, "Upgrade complete. Current version is " << DbUpgrader::currentDbVersion(sqlContext));
    }

    ::sqlite3_close(sqlContext);

    return 1;
}
