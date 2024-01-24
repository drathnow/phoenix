/** @file Exception.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 29, 2013 by daver:
 * @endhistory
 */

#include "Exception.h"

#include <sstream>

namespace zios {
namespace common {

using namespace std;

Exception::Exception(const string& message) : _message(message) {
}

Exception::Exception(const char* funcName, int lineNumber, const string& message) {
    ostringstream oss;
    oss << "[" << funcName << "." << lineNumber << "] " << message;
    _message = oss.str();
}

Exception::Exception(const char* funcName, int lineNumber, const char* name, const std::string& message) {
    ostringstream oss;
    oss << "[" << funcName << "." << lineNumber << "] " << name << " - " << message;
    _message = oss.str();
}


Exception::Exception(const Exception& exception) : _message(exception._message) {
}

const char* Exception::what() const throw() {
    return _message.c_str();
}

Exception& Exception::operator=(const Exception& otherException) {
    _message = otherException._message;
    return *this;
}

Exception::~Exception() throw() {
}

} /* namespace common */
} /* namespace zios */
