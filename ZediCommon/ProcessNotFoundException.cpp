/** @file ProcessNotFoundException.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 13, 2015 by andrewj:
 * @endhistory
 */

#include "ProcessNotFoundException.h"

namespace zios {
namespace common {


ProcessNotFoundException::ProcessNotFoundException(const char* funcName, int lineNumber, const std::string& message) : Exception(funcName, lineNumber, message) {
}

ProcessNotFoundException::~ProcessNotFoundException() throw() {
}


}
} // namespace

