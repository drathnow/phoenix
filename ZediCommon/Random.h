/** @file Random.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 31, 2014 by daver:
 * @endhistory
 */
#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdint.h>

namespace zios {
namespace common {

class Random {
public:
    Random();
    virtual ~Random();

    virtual uint32_t nextU32();
    virtual uint16_t nextU16();
    virtual uint8_t nextU8();
};

} /* namespace common */
} /* namespace zios */

#endif /* RANDOM_H_ */
