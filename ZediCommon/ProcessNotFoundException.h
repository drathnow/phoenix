/** @file ProcessNotFoundException.h
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
#ifndef PROCESSNOTFOUNDEXCEPTION_H_
#define PROCESSNOTFOUNDEXCEPTION_H_

#include "Exception.h"

namespace zios {
namespace common {

class ProcessNotFoundException : public Exception {
public:
    ProcessNotFoundException(const char* funcName, int lineNumber, const std::string& message);
    ~ProcessNotFoundException() throw();
};


} } // namespace



#endif /* PROCESSNOTFOUNDEXCEPTION_H_ */
