/** @file zios.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 14, 2017 by daver:
 * @endhistory
 */
#ifndef ZIOS_H_
#define ZIOS_H_

#include <stdint.h>
#include <string>

#define ZIOS_HOME_DIR                   "/zioshome"
#define ZIOS_SERVER_FILE_PATH           "/var/tmp"
#define ZIOS_SERVER_FILENAME            "figserver"
#define ZIOS_SERVER_STATE_PATH          "/var/tmp"
#define ZIOS_SERVER_STATE_FILENAME      "figstate"
#define ZIOS_WEB_TMP                    "/webtmp"

namespace zios::common
{



class Zios {
public:
    ~Zios();

    static std::string homePath();
    static std::string& serverFilePath(std::string& returnString);
    static std::string& stateFilePath(std::string& returnString);
    static std::string& homePath(std::string& returnString);
    static std::string& webTmpPath(std::string& returnString);

private:
    Zios();
};

}

#endif /* ZIOS_H_ */
