/** @file BitMask.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 20, 2017 by daver:
 * @endhistory
 */
#include "BitField.h"

#include <cstdio>
#include <string>
#include <sstream>

namespace zios {
namespace common {

using namespace std;

BitField::BitField(uint32_t startBit, uint32_t size) :
        _startBit(startBit),
        _theMask(0) {
    _theMask |= (1 << startBit);
    for (unsigned idx = startBit+1; idx < startBit+size; idx++)
        _theMask |= (1 << idx);
}

BitField::BitField(const BitField& otherBitField) {
    _startBit = otherBitField._startBit;
    _theMask = otherBitField._theMask;
}


BitField::~BitField() {
}

int BitField::bitsValueFromValue(int value) const{
    return 0xffff & ((_theMask & value) >> _startBit);
}

int BitField::valueWithBitsSet(int bitsValue, int originalValue) const {
    originalValue &= ~_theMask;
    return originalValue | (bitsValue << _startBit & _theMask);
}

BitField* BitField::bitFieldForString(const char* bitmaskString) {
    unsigned int startBit = 0;
    unsigned int size = 0;

    if (::sscanf(bitmaskString, "%u.%u", &startBit, &size) == 0)
        return NULL;

    if (startBit > 15)
        return NULL;

    if (startBit + size > 16)
        return NULL;

    return new BitField(startBit, size);
}

string BitField::toString() {
    ostringstream oss;
    oss << _startBit;
    unsigned int count = 0;
    while (_theMask) {
        count += _theMask & 1;
        _theMask >>= 1;
    }
    if (count > 1)
        oss << "." << count;

    return oss.str();
}

} /* namespace common */
} /* namespace zios */
