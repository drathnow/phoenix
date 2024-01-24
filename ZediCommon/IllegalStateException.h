/** @file IllegalStateException.h
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

#ifndef ILLEGALSTATEEXCEPTION_H_
#define ILLEGALSTATEEXCEPTION_H_

#include <string>
#include "Exception.h"

namespace zios {
namespace common {

class IllegalStateException : public Exception {
public:
    IllegalStateException(const char* funcName, int lineNumber, const std::string& message);
	~IllegalStateException() throw();
};

} // namespace zios
} // namespace common

#endif /* ILLEGALSTATEEXCEPTION_H_ */
