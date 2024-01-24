/** @file BitField.h
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
#ifndef BITFIELD_H_
#define BITFIELD_H_

#include <stdint.h>
#include <string>

namespace zios {
namespace common {

/**
 * A bits mask provides a convenient class for manipulating fields of bits from a 16 bit number.
 *
 * The Value returned is the masked value shift right so the LSB is in the first position.
 *
 */
class BitField {
public:
    BitField(uint32_t startBit, uint32_t size);
    BitField(const BitField& otherBitField);
    ~BitField();

    /**
     * Extracts the value of the bits from a value. The value is masked and the result
     * is shifted to the right by the start bit position to get the final result.
     *
     * @param value - The value to extract a bits from
     *
     * @return The value of the bits shifted right by the start bit position.
     */
    int bitsValueFromValue(int value) const;

    /**
     * Sets the give bit value in an original value returning the result
     *
     * @param bitsValue - The value of the bit field.
     *
     * @param originalValue - The original value that will have the bits field applied to
     *
     * @return The resulting value
     */
    int valueWithBitsSet(int bitsValue, int originalValue) const;

    /**
     * The assignment operator
     *
     * @param otherBitField
     *
     * @return Reference to this BitField.
     */
    BitField& operator=(const BitField& otherBitField);

    /**
     * Returns a string representation of this bit field
     *
     * @return string - A string
     */
    std::string toString();

    /**
     * Factory method to convert a BitField string specifier to a BitField object.
     *
     * @param bitmaskString - A bit field specifier string.  The general form of the
     * string is
     *
     *      startBit[.size]
     *
     * If only the start bit is specified, then the size field defaults to 1.
     * The maximum value for startBit is 15 but the 'size' depends on the value
     * of the 'startBit'.  The rule is that 'startBit+size' may not exceed 16.
     *
     * @return BitField object.  This is a transfer of ownership.
     */
    static BitField* bitFieldForString(const char* bitmaskString);

    friend class BitFieldTest;

private:
    int _startBit;
    int _theMask;
};

} /* namespace common */
} /* namespace zios */

#endif /* BITFIELD_H_ */
