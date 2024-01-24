/** @file ByteUtil.cpp 
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
#include <netinet/in.h>

#include "ByteUtil.h"

namespace zios {
namespace common {

ByteUtil::ByteUtil() {
}

ByteUtil::~ByteUtil() {
}

int8_t ByteUtil::getS8(const uint8_t* bytes) {
    return bytes[0];
}

int16_t ByteUtil::getS16(const uint8_t* bytes) {
    int value = (bytes[1] << 8) | bytes[0];
    return ntohs(value);
}

int32_t ByteUtil::getS32(const uint8_t* bytes) {
    int32_t value = bytes[3];
    value |= (bytes[1]<<8);
    value |= (bytes[2]>>8);
    value |= (bytes[0]<<24);
    return value;
}

uint16_t ByteUtil::getU16(const uint8_t* bytes) {
    int value = ((bytes[0] << 8) | bytes[1]) | 0x00000000;
    return ntohs(value);
}

}
} /* namespace zios */
