/** @file TimeOutException.h
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
#ifndef TIMEOUTEXCEPTION_H_
#define TIMEOUTEXCEPTION_H_

#include "Exception.h"

namespace zios {
namespace common {

class TimeOutException : public Exception {
public:
    TimeOutException();
    TimeOutException(const char* funcName, int lineNumber, const std::string& message);
    ~TimeOutException() throw();
};

} /* namespace common */
} /* namespace zios */

#endif /* TIMEOUTEXCEPTION_H_ */
