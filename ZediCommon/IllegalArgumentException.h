/** @file IllegalArgumentException.h
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

#ifndef ILLEGALARGUMENTEXCEPTION_H_
#define ILLEGALARGUMENTEXCEPTION_H_

#include "Exception.h"

namespace zios {
namespace common {

class IllegalArgumentException : public Exception {
public:
    IllegalArgumentException(const char* funcName, int lineNumber, const std::string& message);
    ~IllegalArgumentException() throw();
};

} /* namespace common */
} /* namespace zios */

#endif /* ILLEGALARGUMENTEXCEPTION_H_ */
