#include "ByteSwap.h"

#include <endian.h>
#include <cctype>
#include <algorithm>

#include "ByteOrder.h"

namespace zios::foundation
{

using namespace std;

const std::string ByteSwap::NONE_NAME("NONE");
const std::string ByteSwap::BYTE2143_NAME("2143");
const std::string ByteSwap::BYTE4321_NAME("4321");
const std::string ByteSwap::BYTE1234_NAME("1234");
const std::string ByteSwap::BYTE3412_NAME("3412");

ByteSwap::ByteSwap()
{
}

ByteSwap::~ByteSwap()
{
}

/**
 *  Big Endian
 *      byte#      : | 3 | 2 | 1 | 0 |
 *      mapped to# : | 3 | 2 | 1 | 0 |
 *
 *  Word Swap:
 *      byte#      : | 3 | 2 | 1 | 0 |
 *      mapped to# : | 1 | 0 | 3 | 2 |
 *
 *  Byte Swap:
 *      byte#      : | 3 | 2 | 1 | 0 |
 *      mapped to# : | 2 | 3 | 0 | 1 |
 *
 *  Little Endian
 *      byte#      : | 3 | 2 | 1 | 0 |
 *      mapped to# : | 0 | 1 | 2 | 3 |
 *
 *
 */
//void ByteSwapper::valueToByteSwapOrder(uint32_t& value, BSBYTE_ORDER byteOrder) {
//    uint32_t result = value;
//    switch(byteOrder) {
//        case BYTE2143: // We are already in LE
//            result = (result >> 16) | (result << 16);
//            break;
//
//        case BYTE3412:
//            result = htobe32(value);
//            result = (result >> 16) | (result << 16);
//            break;
//
//        case BYTE1234:
//            result = htole32(value); // Just reverse the string
//            break;
//
//        case BYTE4321:
//            result = htobe32(value);
//            break;
//
//        case NONE :
//        default:
//            break;
//    }
//}
/******************************************************************************
 * htombpl : Convert host-endian value into specified modbus-endian value
 */
uint32_t htombpl(uint32_t dword, int order)
{
    switch (order) {
    case 4321:
    case 0:
    default:
        dword = htobe32(dword);
        break;
    case 3412:
        dword = htole32(dword);
        dword = (dword >> 16) | (dword << 16);
        break;
    case 2143:
    case 1:
        dword = htobe32(dword);
        dword = (dword >> 16) | (dword << 16);
        break;
    case 1234:
        dword = htole32(dword);
        break;
    }
    return dword;
}

/******************************************************************************
 * mpbtohl : Convert modbus-endian value into specified host-endian value
 */
uint32_t mbptohl(uint32_t dword, int order)
{
    switch (order) {
    case 4321:
    case 0:
    default:
        dword = be32toh(dword);
        break;
    case 3412:
        dword = le32toh(dword);
        dword = (dword >> 16) | (dword << 16);
        break;
    case 2143:
    case 1:
        dword = be32toh(dword);
        dword = (dword >> 16) | (dword << 16);
        break;
    case 1234:
        dword = le32toh(dword);
        break;
    }
    return dword;
}

void ByteSwap::valueToByteSwapOrder(uint32_t &uint32_tValue,
        BSBYTE_ORDER byteOrder)
{
    switch (byteOrder) {
    case BYTE4321_BE:
        break;

    case BYTE2143_WS:
        uint32_tValue = uint32_tValue >> 16
                | (uint32_tValue << 16 & 0xffff0000);
        break;

    case BYTE3412_BS: {
        uint16_t hi = uint32_tValue >> 16;
        uint16_t low = 0x0000ffff & uint32_tValue;
        uint32_tValue = htobe16(hi) << 16 | htobe16(low);
        break;
    }

    case BYTE1234_LE:
        uint32_tValue = htobe32(uint32_tValue);
        break;

    default:
    case NONE:
        break;

    } // switch
}

ByteSwap::BSBYTE_ORDER ByteSwap::byteOrderForName(const string &name)
{
    string myName = name;
    transform(myName.begin(), myName.end(), myName.begin(), ::toupper);

    if (NONE_NAME == myName)
        return NONE;

    if (BYTE2143_NAME == myName)
        return BYTE2143_WS;

    if (BYTE3412_NAME == myName)
        return BYTE3412_BS;

    if (BYTE1234_NAME == myName)
        return BYTE1234_LE;

    if (BYTE4321_NAME == myName)
        return BYTE4321_BE;

    return UNKNOWN;
}

} /* namespace zios */
