/** @file PermissionException.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 13, 2015 by andrewj:
 * @endhistory
 */
#ifndef PERMISSIONEXCEPTION_H_
#define PERMISSIONEXCEPTION_H_

#include <string>
#include "Exception.h"

namespace zios {
namespace common {

class PermissionException : public common::Exception {
public:
    PermissionException(const char* funcName, int lineNumber, const std::string& message);
    ~PermissionException() throw();
};

} // namespace zios
} // namespace common



#endif /* PERMISSIONEXCEPTION_H_ */
