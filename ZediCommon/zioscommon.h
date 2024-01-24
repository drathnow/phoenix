/** @file zioscommon.h 
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
#ifndef ZIOSCOMMON_H_
#define ZIOSCOMMON_H_

#include <string>

#define ZIOS_HOME_DIR                   "/zioshome"
#define ZIOS_SERVER_FILE_PATH           "/var/tmp"
#define ZIOS_SERVER_FILENAME            "figserver"
#define ZIOS_SERVER_STATE_PATH          "/var/tmp"
#define ZIOS_SERVER_STATE_FILENAME      "figstate"
#define ZIOS_WEB_TMP                    "/webtmp"

namespace zios {
namespace common {

std::string zios_home_path();
std::string& zios_server_file_path(std::string& returnString);
std::string& zios_state_file_path(std::string& returnString);
std::string& zios_home_path(std::string& returnString);
std::string& zios_web_tmp_path(std::string& returnString);

}
}

#endif /* ZIOSCOMMON_H_ */
