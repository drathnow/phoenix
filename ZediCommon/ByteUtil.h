/** @file ByteUtil.h 
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

#ifndef BYTEUTIL_H_
#define BYTEUTIL_H_
#include <stdint.h>

namespace zios {
namespace common {

class ByteUtil {
public:
    static int8_t getS8(const uint8_t* bytes);
    static int16_t getS16(const uint8_t* bytes);
    static uint16_t getU16(const uint8_t* bytes);
    static int32_t getS32(const uint8_t* bytes);

private:
    ByteUtil();
    ~ByteUtil();
};

}
} /* namespace zios */
#endif /* BYTEUTIL_H_ */
