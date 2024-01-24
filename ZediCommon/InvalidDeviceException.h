/** @file InvalidDeviceException.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 15, 2015 by andrewj:
 * @endhistory
 */
#ifndef INVALIDDEVICEEXCEPTION_H_
#define INVALIDDEVICEEXCEPTION_H_

#include <string>
#include "Exception.h"

namespace zios {
namespace common {

class InvalidDeviceException : public common::Exception {
public:
    InvalidDeviceException(const char* funcName, int lineNumber, const std::string& message);
    ~InvalidDeviceException() throw();
};

} // namespace zios
} // namespace common

#endif /* INVALIDDEVICEEXCEPTION_H_ */
