/** @file BufferUnderflowException.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 6, 2013 by daver:
 * @endhistory
 */

#include "BufferUnderflowException.h"

namespace zios {
namespace common {

BufferUnderflowException::BufferUnderflowException(const char* funcName, int lineNumber, const std::string& message) : Exception(funcName, lineNumber, message) {
}

BufferUnderflowException::~BufferUnderflowException() throw() {
}

}
} /* namespace zios */
