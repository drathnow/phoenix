/** @file zios.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 23, 2013 by eganb:
 * IncrDev Oct 02, 2013 by eganb: added in display, connection, reporting and other features
 * IncrDev Oct 10, 2013 by eganb: added in enhancements, change IP address to preprod public
 * IncrDev Jan 31, 2014 by eb:    change to add in new behavior complient to new model
 * IncrDev Feb 27, 2014 by eb:    changes to integrate with new calculation loading of RTE
 * IncrDev Mar 04, 2014 by eb:    add in reporting links (integration for demo 2)
 * BugFix_ Mar 05, 2014 by eb:     remove references to application  - was removed last commit
 * BugFix_ Jun 06, 2014 by eb:  add missing include file for cross compile
 * IncrDev Jun 06, 2014 by eb:  add scheduling/action management into for demo3
 * BugFix_ Jun 11, 2014 by eb:  remove reaping of objects from cppinject - causes segfault
 * IncrDev Jun 18, 2014 by eb: changes for moving ioPoint manager into rte library
 * IncrDev Jul 21, 2014 by eb: integrating in the external device/protocol handler code.
 * IncrDev Jul 25, 2014 by eb: add in code to request RTU write
 * IncrDev Jul 31, 2014 by eb: updates to last checkin.
 * IncrDev Aug 01 31, 2014 by eb: changed write rtu command to write previous read +1
 * IncrDev Aug 01 31, 2014 by eb: add in call to setup RS232 Transceiver.
 * IncrDev sep 16, 2014 by eb: modified to match changes in site referencing from id to name
 * IncrDev Sep 26, 2014 by eb: rename Tag stuff to FQTag and name stuff to Tag.
 * IncrDev Sep 30, 2014 by eb: changes to support changes in Add Port for return list of codes/ ids
 * IncrDev Oct 03, 2014 by eb: modifications for changes for ports and ext devs.
 * BugFix_ Oct 06, 2014 by eb: add in forgotten portmanager->init()
 * IncrDev Oct 30, 2014 by eb: start to add in persistence hooks
 * IncrDev Nov 17, 2014 by eb: add "ignored" ioPoint id to add iopoint structure
 * BugFix_ Nov 19, 2014 by eb: add in schemapersistenceManager.registerall bindings call that was missed.
 * IncrDev Dec 08, 2014 by eb: change scheduler to use interface instead of concrete object
 * IncrDev Jan 07, 2015 by eb: change addIOPoint to use new DTO
 * IncrDev Mar 13, 2015 by eb: change parsecmdline
 * IncrDev Mar 13, 2015 by eb: add connection->connect to --silent running
 * IncrDev Mar 25, 2015 by eb: fix up RTU test points to new site/source address
 * IncrDev Apr 07, 2015 by eb: moved most of main() into ziosApp class
 * IncrDev Apr 20, 2015 by eb:added testmode to allow override of S/N (username)
 * IncrDev May 25, 2015 by eb: add servername mode to cause override of server address
 * @endhistory
 */

#include <File.h>
#include <getopt.h>
#include <GlobalZoneType.h>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <AppManager.h>
#include <Utl.h>
#include <Zone.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <ZiosApp.h>
#include <StringUtl.h>
#include <unistd.h>
#include <err.h>
#include <csignal>
#include <cerrno>
#include <cstring>

using namespace CppInject;
using namespace std;
using namespace zios;
using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace zios::common;

// cmd to pass into app:
static const Logger startupLogger = Logger::getInstance("zios");
static const char* DEFAULT_CONFIG_FILE = "config.json";
static const char* DEFAULT_LOGCONFIG_FILE = "zios.properties";
static const char* DEFAULT_PID_FILE = "/var/run/zios.pid";
static const char* DEFAULT_DBFILE = "db/zios.db";
static string configFilePath(DEFAULT_CONFIG_FILE);
static string loggingPropertiesFilePath(DEFAULT_LOGCONFIG_FILE);
static string pidFilePath(DEFAULT_PID_FILE);
static string dbFilePath(DEFAULT_DBFILE);
static string username;
static string serverAddress;
static string key;
static bool noWatchdog;
static CppInject::Zone globalZone(&CppInject::GlobalZoneType::instance);
static Logger logger = Logger::getInstance("zios");

static void usage() {
    std::cerr << endl
              << "Usage: zios [--config=<config-file>] [--loggingproperties=<log4cplus-properties-path>] [--username=<username>] [--serveraddress=<server-address>] [--key=<base64-key>]" << endl << endl;
    std::cerr << "     --help - Shows this text" << endl << endl;
    std::cerr << "     --config=<config-file> - Sets the json config file used by ZIOS. If not provided, ZIOS will look for config.json in the current directory" << endl << endl;
    std::cerr << "     --serveraddress=<ipaddress> - Sets the server (bridge) address" << endl << endl;
    std::cerr << "     --username=<username> - Overrides the serial number in the eeprom used to log into the bridge" << endl << endl;
    std::cerr << "     --loggingproperties=<log4cplus-properties-path> - log4cplus logging properties file. If not provided, ZIOS will look for zios.properties in the current directory" << endl << endl;
    std::cerr << "     --dumpstart - Display the contents of the AppManager's startup database" << endl << endl;
    std::cerr << "     --dbfile=<dbfile-path> - Path to the ZIOS database file" << endl << endl;
    std::cerr << "     --key=<base64-key> - Base64 encode key used to authenticate device" << endl << endl;
    std::cerr << "     --kill - Kills running zios process." << endl << endl;
    std::cerr << "     --nowatchdog - Will run with a dummy watchdog." << endl << endl;
    exit(1);
}

static void killZios() {
    int pid = 0;
    FILE* file = ::fopen(pidFilePath.c_str(), "r");
    if (file == NULL)
        errx(-1, "Unable to open pid file: %s", ::strerror(errno));
    else {
        ::fscanf(file, "%d", &pid);
        ::fclose(file);
        if (::kill(pid, SIGKILL) == -1)
            errx(-1, "Unable to kill zios: %s", ::strerror(errno));
        errx(0, "ZIOS is dead");
    }
}

static void parseCommandLineArgs(int argc, char* argv[]) {

    /*
     * option - name, arguments,flag,val
     *
     * name:
     * then long option name that follows the '--'
     *
     * arguments:
     * no_argument  - for no argument in argv
     * required_argument  - for must have argument in argv
     * optional_argument  - for optional argument in argv
     *
     * flag:
     *   NULL (0) return the val (or '?' if option not found)....
     *   pointer to variable to which will contain val if found, but unchanged if option not found , will return '0'
     *
     *  val:
     *   value to return or to load into variable pointed to by flag.
     *
     *   last element of array must be 0 filled.
     */
    static struct option longOptions[] = {
            { "config",                 required_argument,  0,  'c' },
            { "loggingproperties",      required_argument,  0,  'l' },
            { "serveraddress",          required_argument,  0,  's' },
            { "help",                   no_argument,        0,  'h' },
            { "username",               required_argument,  0,  'u' },
            { "key",                    required_argument,  0,  'k' },
            { "dbfile",                 required_argument,  0,  'd' },
            { "kill",                   no_argument,        0,  'i' },
            { "nowatchdog",             no_argument,        0,  'w' },
            { 0, 0, 0, 0 } };
    int c;
    int optionIndex = 0;                   // will point to position in longOptions.
    std::vector<std::string> sources;
    std::string path(configFilePath);

    /*
     *  getopt_long
     *  argc:  from command line, the count of argv
     *  argv:  the command line
     *  optString: the string of valid -'x' characters --
     *             note syntax: letter [optionally followed by ':' for required parameter or '::' for optional parameter
     *             example t: would mean require -tblah  ,  t:: would be -t or -tblah -> blah would be put into optarg or optarg would be '0'
     *  longOptions:  the option array for '--' options (see above)
     *  optionIndex:  holds the position in longoptions of found long option
     *  parameters are placed in optarg (see man getopts for description (in getopt.h))
     */
    while ((c = ::getopt_long(argc, argv, "c:l:s:h:u:k:d:iw", longOptions, &optionIndex)) != -1) {
        switch (c) {
        case '?':
            if (::isprint(optopt))
                ::fprintf(stderr, "Unknown option '-%c'.\n", optopt);
            usage();
            break;

        case 'c':
            configFilePath = optarg;
            StringUtl::trim(configFilePath);
            break;

        case 'l':
            loggingPropertiesFilePath = optarg;
            StringUtl::trim(loggingPropertiesFilePath);
            break;

        case 's':
            serverAddress = optarg;
            StringUtl::trim(serverAddress);
            break;

        case 'u':
            username= optarg;
            StringUtl::trim(username);
            break;

        case 'd':
            dbFilePath = optarg;
            StringUtl::trim(dbFilePath);
            break;

        case 'k':
            key = optarg;
            StringUtl::trim(key);
            break;

        case 'w':
            noWatchdog = true;
            break;

        case 'i':
            killZios();
            break;

        case 'h':
        default:
            usage();
        }
    }
}

static void initLogging() {
    File file(loggingPropertiesFilePath);
    if (file.exists() == false)
        cerr << "Specified logging config file does not exist: " << file.absolutePath() << endl;
    else {
        PropertyConfigurator::doConfigure(file.absolutePath());
        LOG4CPLUS_INFO(logger, "Logging configuration loaded from " << file.absolutePath());
    }
}

int main(int argc, char * argv[]) {

    try {
        parseCommandLineArgs(argc, argv);
        Utl::createPidFile(pidFilePath);
        initLogging();
        CppInject::Zone::initGlobalZone(&globalZone);
        ZiosApp theApp(configFilePath, username, serverAddress, key, dbFilePath);
        theApp.setRunWithoutWatchdog(noWatchdog);
        theApp.run();
    } catch (std::exception& e) {
        LOG4CPLUS_FATAL(startupLogger, e.what());
        LOG4CPLUS_FATAL(startupLogger, "ZIOS Abort!");
    } catch (...) {
        LOG4CPLUS_FATAL(startupLogger, "Unknown exception caused program termination");
        LOG4CPLUS_FATAL(startupLogger, "ZIOS Abort!");
    }
    return 0;
}

