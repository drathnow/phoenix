/** @file SystemException.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 5, 2015 by andrewj:
 * @endhistory
 */
#ifndef SYSTEMEXCEPTION_H_
#define SYSTEMEXCEPTION_H_

#include "Exception.h"

namespace zios {
namespace common {

/**
     * Exception class that will hold the value of the error code for later use, as well as
     * format the exception and display the matching OS string.
 */
class SystemException : public zios::common::Exception {
public:
    SystemException(const char* funcName, int lineNumber, const std::string& message);
    virtual ~SystemException() throw();

    inline int error() { return _error; }
private:
    int _error;
};


} } // namespace



#endif /* SYSTEMEXCEPTION_H_ */
