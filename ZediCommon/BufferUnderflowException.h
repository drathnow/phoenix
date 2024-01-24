/** @file BufferUnderflowException.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 6, 2013 by daver:
 * @endhistory
 */

#ifndef BUFFERUNDERFLOWEXCEPTION_H_
#define BUFFERUNDERFLOWEXCEPTION_H_


#include "Exception.h"

namespace zios {
namespace common {

class BufferUnderflowException : public Exception {
public:
    BufferUnderflowException(const char* funcName, int lineNumber, const std::string& message);
    ~BufferUnderflowException() throw();
};

}
} /* namespace zios */
#endif /* BUFFERUNDERFLOWEXCEPTION_H_ */
