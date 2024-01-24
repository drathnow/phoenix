/** @file InvalidDeviceException.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 15, 2015 by andrewj:
 * @endhistory
 */

#include "InvalidDeviceException.h"

namespace zios {
namespace common {

InvalidDeviceException::InvalidDeviceException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, "IllegalArgumentException", message) {
}

InvalidDeviceException::~InvalidDeviceException() throw() {
}


} // namespace common
} // namespace zios

