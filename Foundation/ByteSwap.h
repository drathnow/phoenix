#ifndef BYTESWAP_H_
#define BYTESWAP_H_

#include <stdint.h>
#include <string>

namespace zios::foundation
{

struct ByteSwap
{
    static const std::string NONE_NAME;
    static const std::string BYTE2143_NAME;
    static const std::string BYTE4321_NAME;
    static const std::string BYTE1234_NAME;
    static const std::string BYTE3412_NAME;

    enum BSBYTE_ORDER
    {
        NONE = 0, BYTE1234_LE = 1,   // le
        BYTE4321_BE = 2,   // be
        BYTE2143_WS = 3,   // word swap
        BYTE3412_BS = 4,   // byte swap
        UNKNOWN = 99
    };

    static void valueToByteSwapOrder(uint32_t &value, BSBYTE_ORDER byteOrder);
    static BSBYTE_ORDER byteOrderForName(const std::string &name);
};

} /* namespace zios */

#endif /* BYTESWAP_H_ */
