/** @file BufferOverflowException.cpp
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

#include "BufferOverflowException.h"

namespace zios {
namespace common {

BufferOverflowException::BufferOverflowException(const char* funcName, int lineNumber, const std::string& message) : Exception(funcName, lineNumber, message) {
}

BufferOverflowException::~BufferOverflowException() throw() {
}

}
}
