/** @file IllegalArgumentException.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 10, 2013 by daver:
 * @endhistory
 */

#include "IllegalArgumentException.h"

namespace zios {
namespace common {

IllegalArgumentException::IllegalArgumentException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, "IllegalArgumentException", message) {
}

IllegalArgumentException::~IllegalArgumentException() throw() {
}

} /* namespace common */
} /* namespace zios */
