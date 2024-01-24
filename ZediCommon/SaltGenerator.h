/** @file SaltGenerator.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 3, 2013 by daver:
 * @endhistory
 */

#ifndef SALTGENERATOR_H_
#define SALTGENERATOR_H_

#include <cstdlib>
#include <stdint.h>

namespace zios {
namespace common {

class SaltGenerator {
public:
    SaltGenerator();
    virtual ~SaltGenerator();

    virtual void generateSalt(uint8_t* buffer, uint32_t bufferLength);
};

}
} /* namespace zios */
#endif /* SALTGENERATOR_H_ */
