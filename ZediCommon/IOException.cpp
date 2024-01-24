/** @file IOException.cpp
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
#include "IOException.h"

namespace zios {
namespace common {

IOException::IOException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, "IOException", message) {

}

IOException::~IOException() throw() {
}

} /* namespace modbus */
} /* namespace zios */
