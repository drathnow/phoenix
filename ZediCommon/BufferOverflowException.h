/** @file BufferOverflowException.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 4, 2013 by daver:
 * @endhistory
 */

#ifndef BUFFEROVERLOWEXCEPTOIN_H_
#define BUFFEROVERLOWEXCEPTOIN_H_

#include "Exception.h"

namespace zios {
namespace common {

class BufferOverflowException : public Exception {

public:
    BufferOverflowException(const char* funcName, int lineNumber, const std::string& message);
	~BufferOverflowException() throw();
};

}
}
#endif /* BUFFEROVERLOWEXCEPTOIN_H_ */
