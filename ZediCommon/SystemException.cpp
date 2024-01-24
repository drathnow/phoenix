/** @file SystemException.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 5, 2015 by andrewj:
 * @endhistory
 */


#include "SystemException.h"
#include <string>
#include "string.h"
#include "StringUtl.h"


namespace zios {
namespace common {

SystemException::SystemException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, message) {
}

SystemException::~SystemException() throw() {
}


}
} // namespace
