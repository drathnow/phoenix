/** @file EEPromUtil.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 27, 2019 by daver:
 * @endhistory
 */
#include "EEPromUtil.h"

#include <string>
#include <string.h>    // for strlen...
#include <sstream>
#include <fcntl.h>
#include <EEProm.h>
#include <cstdio>
#include <unistd.h>

namespace zios {
namespace hal {

EEPromUtil::EEPromUtil()
{
}

EEPromUtil::~EEPromUtil()
{
}


void EEPromUtil::createMainBoardEEProm(const char* path)
{
    //  setup file

}
} /* namespace hal */
} /* namespace zios */

