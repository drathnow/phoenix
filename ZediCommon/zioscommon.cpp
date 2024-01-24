/** @file zioscommon.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 8, 2019 by daver:
 * @endhistory
 */

#include "zioscommon.h"
#include <log4cplus/loggingmacros.h>

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;


static void root_path(string& returnString, const char* defaultString = "")
{
#ifdef _ONARM
    // On the ARM target, always return the default value
    returnString = defaultString;
#else
    // On x86, use the home directory so each user can have their own files
    const char *homedir;
    homedir = getenv("HOME");
    if (homedir == NULL) {
        LOG4CPLUS_ERROR(Logger::getInstance("zios.common"), "HOME variable not set.  Unable to write to state files.");
        return;
    }
    returnString.append(homedir)
            .append(defaultString);
#endif
}

std::string zios_home_path()
{
    string returnString;
    root_path(returnString);
    returnString.append(ZIOS_HOME_DIR);
    return returnString;
}

std::string& zios_server_file_path(std::string& returnString)
{
    root_path(returnString, ZIOS_SERVER_FILE_PATH);
    returnString.append("/").append(ZIOS_SERVER_FILENAME);
    return returnString;
}

std::string& zios_state_file_path(std::string& returnString)
{
    root_path(returnString, ZIOS_SERVER_STATE_PATH);
    returnString.append("/").append(ZIOS_SERVER_STATE_FILENAME);
    return returnString;
}

std::string& zios_home_path(std::string& returnString)
{
    root_path(returnString);
    returnString.append(ZIOS_HOME_DIR);
    return returnString;
}

std::string& zios_web_tmp_path(std::string& returnString)
{
    root_path(returnString, ZIOS_HOME_DIR);
    returnString.append("/").append(ZIOS_WEB_TMP);
    return returnString;
}

}
}

