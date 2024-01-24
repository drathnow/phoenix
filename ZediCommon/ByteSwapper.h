/** @file ByteSwapper.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 15, 2016 by daver:
 * @endhistory
 */
#ifndef BYTESWAPPER_H_
#define BYTESWAPPER_H_

#include <stdint.h>
#include <string>

namespace zios {
namespace common {

class ByteSwapper {
public:
    static const std::string NONE_NAME;
    static const std::string BYTE2143_NAME;
    static const std::string BYTE4321_NAME;
    static const std::string BYTE1234_NAME;
    static const std::string BYTE3412_NAME;

    enum BSBYTE_ORDER {
        NONE = 0,
        BYTE1234_LE = 1,   // le
        BYTE4321_BE = 2,   // be
        BYTE2143_WS = 3,   // word swap
        BYTE3412_BS = 4,   // byte swap
        UNKNOWN = 99
    };

    virtual ~ByteSwapper();

    static void valueToByteSwapOrder(uint32_t& value, BSBYTE_ORDER byteOrder);
    static BSBYTE_ORDER byteOrderForName(const std::string& name);

private:
    ByteSwapper();
};

} /* namespace modbus */
} /* namespace zios */

#endif /* BYTESWAPPER_H_ */
