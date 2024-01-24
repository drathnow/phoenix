/** @file ByteOrder.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 24, 2016 by daver:
 * @endhistory
 */
#include "ByteOrder.h"
#include "Exception.h"
#include "IllegalArgumentException.h"
#include "HexDumper.h"

#include <cassert>
#include <byteswap.h>
#include <arpa/inet.h>
#include <iostream>

namespace zios {
namespace common {

static const uint8_t LE = 1;
static const uint8_t BE = 2;

ByteOrder ByteOrder::BigEndian(BE);
ByteOrder ByteOrder::LittleEndian(LE);
static ByteOrder* platformOrder = NULL;

ByteOrder::~ByteOrder() {
}

ByteOrder::ByteOrder(uint8_t number) :
        _number(number) {
}

uint16_t ByteOrder::toByteOrder(uint16_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_16(value);
}

int16_t ByteOrder::toByteOrder(int16_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_16(value);
}

uint32_t ByteOrder::toByteOrder(uint32_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_32(value);
}

int32_t ByteOrder::toByteOrder(int32_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_32(value);
}

uint64_t ByteOrder::toByteOrder(uint64_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_64(value);
}

int64_t ByteOrder::toByteOrder(int64_t value, ByteOrder& toByteOrder) const {
    return (*this == toByteOrder) ? value : bswap_64(value);
}

bool ByteOrder::operator==(const ByteOrder& otherByteOrder) const {
    return _number == otherByteOrder._number;
}

ByteOrder& ByteOrder::nativeOrder() {
    if (platformOrder == NULL) {
        int num = 1;
        platformOrder = (*(char*)&num == 1) ? &LittleEndian : &BigEndian;
    }
    return *platformOrder;
}

ByteOrder* ByteOrder::byteOrderForByteOrderNumber(uint8_t number) {
    switch (number) {
        case BE:
            return &BigEndian;
        case LE:
            return &LittleEndian;
    }
    return NULL; // Will never get here but keeps Eclipse happy.
}

} /* namespace reporting */
} /* namespace zios */
