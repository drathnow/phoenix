/** @file TimeOutException.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 16, 2016 by daver:
 * @endhistory
 */
#include "TimeOutException.h"

namespace zios {
namespace common {

TimeOutException::TimeOutException() : Exception("Timeout") {
}

TimeOutException::TimeOutException(const char* funcName, int lineNumber, const std::string& message) :
                Exception(funcName, lineNumber, "TimeOutException", message) {
}

TimeOutException::~TimeOutException() throw() {
}

} /* namespace common */
} /* namespace zios */
