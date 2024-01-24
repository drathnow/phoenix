/** @file IllegalStateException.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 04, 2013 by daver:
 * @endhistory
 */

#include "IllegalStateException.h"

namespace zios {
namespace common {

IllegalStateException::IllegalStateException(const char* funcName, int lineNumber, const std::string& message) : Exception(funcName, lineNumber, "IllegalStateException") {
}

IllegalStateException::~IllegalStateException() throw() {
}


} // namespace common
} // namespace zios
