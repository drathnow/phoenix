/** @file UnsupportedOperationException.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 19, 2016 by daver:
 * @endhistory
 */
#ifndef UNSUPPORTEDOPERATIONEXCEPTION_H_
#define UNSUPPORTEDOPERATIONEXCEPTION_H_
#include "Exception.h"
#include <string>

namespace zios {
namespace common {

class UnsupportedOperationException: public Exception
{
public:
    UnsupportedOperationException(const char* funcName, int lineNumber, const std::string& message);
    ~UnsupportedOperationException() throw ();
};

} /* namespace common */
} /* namespace zios */

#endif /* UNSUPPORTEDOPERATIONEXCEPTION_H_ */
