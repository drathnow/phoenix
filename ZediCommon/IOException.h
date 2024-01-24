/** @file IOException.h
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
#ifndef IOEXCEPTION_H_
#define IOEXCEPTION_H_

#include "Exception.h"
#include <string>

namespace zios {
namespace common {

class IOException : public Exception {
public:
    IOException(const char* funcName, int lineNumber, const std::string& message);
    virtual ~IOException() throw();
};

} /* namespace modbus */
} /* namespace zios */

#endif /* IOEXCEPTION_H_ */
