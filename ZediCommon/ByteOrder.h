/** @file ByteOrder.h
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
#ifndef BYTEORDER_H_
#define BYTEORDER_H_

#include <stdint.h>

namespace zios {
namespace common {

class ByteOrder {
public:
    virtual ~ByteOrder();

    static ByteOrder BigEndian;
    static ByteOrder LittleEndian;

    /**
     * Converts a unsigned 16 bit value to a specific byte order
     *
     * @param value - A uint16_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    uint16_t toByteOrder(uint16_t value, ByteOrder& toByteOrder) const;

    /**
     * Converts a signed 16 bit value to a specific byte order
     *
     * @param value - A uint16_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    int16_t toByteOrder(int16_t value, ByteOrder& toByteOrder) const;

    /**
     * Converts a unsigned 32 bit value to a specific byte order
     *
     * @param value - An uint32_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    uint32_t toByteOrder(uint32_t value, ByteOrder& toByteOrder) const;

    /**
     * Converts a signed 32 bit value to a specific byte order
     *
     * @param value - An int32_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    int32_t toByteOrder(int32_t value, ByteOrder& toByteOrder) const;

    /**
     * Converts a unsigned 64 bit value to a specific byte order
     *
     * @param value - A uint64_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    uint64_t toByteOrder(uint64_t value, ByteOrder& toByteOrder) const;

    /**
     * Converts a signed 64 bit value to a specific byte order
     *
     * @param value - An int64_t value
     *
     * @param toByteOrder - The taget byte order
     *
     * @return Value after swapping bytes
     */
    int64_t toByteOrder(int64_t value, ByteOrder& toByteOrder) const;

    /**
     * Returns the byte order number.  Use this value if you need to persist
     * the byte order for anything.  You can then restore the byte order by passing
     * this value to byteOrderForByteOrderNumber().
     *
     * @return A byte order number
     */
    uint8_t byteOrderNumber() const  { return _number; }

    /**
     * Equality operator.  You know what it does
     *
     * @param otherByteOrder
     * @return true if the other object is the same object.  False otherwise
     */
    bool operator==(const ByteOrder& otherByteOrder) const;

    /**
     * Returns a ByteOrder object for the given byte order number.
     *
     * @param number - Either 1 (LE) or 2 (BE)
     *
     * @return ByteOrder* - pointer to a ByteOrder object.  This is NOT a transfer of ownership.
     * Receivers of this object should never call delete on the returned object. NULL
     * if number is not 1 or 2.
     */
    static ByteOrder* byteOrderForByteOrderNumber(uint8_t number);

    /**
     * Returns the native byte order for this platform
     *
     * @return Pointer to a byte order object. It will never return NULL.
     */
    static ByteOrder& nativeOrder();

private:
    ByteOrder(uint8_t number);
    uint8_t _number;
};

} /* namespace reporting */
} /* namespace zios */

#endif /* BYTEORDER_H_ */
