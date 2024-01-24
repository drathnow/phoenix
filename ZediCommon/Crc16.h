/** @file Crc16.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 12, 2014 by kevinl:
 * IncrDev Jul 25, 2014 by eganb:   change to unsigned char from char
 * @endhistory
 */

#ifndef MBCRC16_H_
#define MBCRC16_H_

/* Platform header */
#include <stdint.h>
#include <cstdlib>

namespace zios {
namespace common {

class Crc16 {
public:
    ~Crc16() {}

    static uint16_t calculate(const uint8_t* byteArr, const size_t byteCnt);

private:
    Crc16() {}
};


#define CRC_16       0x01     // Polynomial = x^16 + x^15 + x^2 + 1
#define CRC_CCITT    0x02     // Polynomial = x^16 + x^12 + x^5 + 1
#define CRC_REFLECT  0x80


/**
 * Calculates a CRC for a byte array.
 *
 * @param seed - Initial seed value for the CRC.
 *
 * @param data - Pointer to the byte array
 *
 * @param dataSize - Lenght of the byte array
 *
 * @param mode - Bit mask that defines which polynomial is used in the calculation of the
 * CRC and which CRC table.
 *
 * @return The CRC for the given array.
 */
uint16_t CRC_calculate(uint16_t seed, const uint8_t* data, uint32_t dataSize, uint32_t mode);

uint16_t CRC_calculate(const uint8_t* data, uint32_t dataSize);

}
} // namespace

#endif /* MBCRC_H_ */
