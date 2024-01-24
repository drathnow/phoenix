/** @file NamedType.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 09, 2013 by daver:
 * @endhistory
 */

#include <iostream>
#include <sstream>
#include "NamedType.h"

namespace zios {
namespace common {

NamedType& NamedType::operator=(const NamedType& otherNamedType) {
    _name = otherNamedType._name;
    _number = otherNamedType._number;
    return *this;
}

NamedType::NamedType(const NamedType& namedType) : _name(namedType._name), _number(namedType._number) {
}

NamedType::NamedType(const char* name, int number) : _name(name), _number(number){
}

NamedType::NamedType(const std::string& name, int number) : _name(name), _number(number){
}

NamedType::~NamedType() {
}

const std::string NamedType::toString() const {
    std::ostringstream stringStream;
    stringStream << _name << '(' << _number << ')';
    return stringStream.str();
}

} /* namespace zap */
} /* namespace zios */
