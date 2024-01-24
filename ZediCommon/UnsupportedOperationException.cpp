/** @file UnsupportedOperationException.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 19, 2016 by daver:
 * @endhistory
 */
#include "UnsupportedOperationException.h"

namespace zios {
namespace common {

UnsupportedOperationException::UnsupportedOperationException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, "UnsupportedOperationException", message) {
}

UnsupportedOperationException::~UnsupportedOperationException() throw() {
}

} /* namespace common */
} /* namespace zios */
