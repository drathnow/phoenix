/** @file ByteBuffer.h
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

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#include "ByteOrder.h"

#include <cstddef>
#include <string>
#include <stdint.h>

namespace zios {
namespace common {


class ByteBuffer {
public:
    enum ErrorStatus {
        BUFFER_UNDERFLOW = -1,
        BUFFER_OVERFLOW = -2
    };

    ByteBuffer(const ByteBuffer& otherBuffer);
    virtual ~ByteBuffer();

    /** putString
     *
     * Writes a string to the underlying buffer.  The string is written in the standard zios format,
     * where the first 16 bytes (unsigned short) is the count of bytes that make up the string, not including
     * the count bytes, and the remainder n bytes make up the string. NOTE: The string is not NULL
     * terminated.
     *
     * @param[in] str - reference to a string object
     */
    virtual void putString(const std::string& str) = 0;
    void put(const std::string& str) { putString(str); }

    /**
     * Writes an signed byte to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putS8(int8_t value) = 0;
    void put(int8_t value) { putS8(value); }

    /**
     * Writes an unsigned signed byte to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putU8(uint8_t value) = 0;
    void put(uint8_t value) { putU8(value); }

    /**
     * Writes an signed short to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putS16(int16_t value) = 0;
    void put(int16_t value) { putS16(value); }

    /**
     * Writes an unsigned signed short to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putU16(uint16_t value) = 0;
    void put(uint16_t value) { putU16(value); }

    /**
     * Writes an signed 32 bit integer to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putS32(int32_t value) = 0;
    void put(int32_t value) { putS32(value); }

    /**
     * Writes an unsigned 32 bit integer to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putU32(uint32_t value) = 0;
    void put(uint32_t value) { putU32(value); }

    /**
     * Writes an signed 64 bit integer to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putS64(int64_t value) = 0;
    void put(int64_t value) { putS64(value); }

    /**
     * Writes an unsigned 64 bit integer to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putU64(uint64_t value) = 0;
    void put(uint64_t value) { putU64(value); }

    /**
     * Writes an 32 bit float to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putFloat(float value) = 0;
    void put(float value) { putFloat(value); }

    /**
     * Writes an 64 bit double to the underlying byte stream
     *
     * @param value - The value to write
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual void putDouble(double value) = 0;
    void put(double value) { put(value); }

    /**
     * Writes the contents of a buffer to the underylying byte stream.
     *
     * @param buffer - An array of signed 8 bit bytes
     *
     * @param bufferLength - The length of the buffer.
     *
     * @return the number of bytes written to the underlying byte stream. If there
     * is not enough room in the stream, this value could be less than bufferLength.
     */
    virtual int put(const int8_t* buffer, int bufferLength) = 0;

    /**
     * Writes all data from the input stream into this byte buffer
     *
     * @param inputStream
     *
     * @param byteCount Number of bytes to retrieve from the stream. If zero, then the
     * stream will be queried to determine the number of bytes it contains.  If greater
     * than zero, only that number of bytes will be read.
     *
     * @return Number of bytes read from the stream
     *
     * @throws BufferOverflowException - thrown if writing this value will result in writing
     * outside the bounds of the underlying byte stream.
     */
    virtual int putAll(std::istream& inputStream, int byteCount = 0) = 0;

    /** getString
     *
     * Reads and returns a string from the underlying byte buffer.  The string is assumed to be
     * encoded in the stanards zios string formate where the first 16 bits (unsigned short) specifies
     * the count, or number of bytes that make up the string, not including the count bytes.  The following
     * bytes make up the string.
     *
     * @return string - a string object.
     *
     * @throws BufferUnderflowException - thrown if the there are no more, or not enough,
     * bytes left in the underlying byte stream.
     */
    virtual std::string getString() = 0;

    /**
     * Reads and returns a signed 8 bit value from the underlying byte stream.
     *
     * @return int8_t - A signed 8 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual int8_t getS8() = 0;


    /**
     * Reads and returns an unsigned 8 bit value from the underlying byte stream.
     *
     * @return int8_t - An unigned 8 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual uint8_t getU8() = 0;

    /**
     * Reads and returns a signed 16 bit value from the underlying byte stream.
     *
     * @return int8_t - A signed 16 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual int16_t getS16() = 0;

    /**
     * Reads and returns an unsigned 16 bit value from the underlying byte stream.
     *
     * @return int8_t - An unsigned 16 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual uint16_t getU16() = 0;

    /**
     * Reads and returns a signed 32 bit value from the underlying byte stream.
     *
     * @return int8_t - A signed 32 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual int32_t getS32() = 0;

    /**
     * Reads and returns an unsigned 32 bit value from the underlying byte stream.
     *
     * @return int8_t - An unsigned 32 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual uint32_t getU32() = 0;

    /**
     * Reads and returns a signed 64 bit value from the underlying byte stream.
     *
     * @return int8_t - A signed 64 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual int64_t getS64() = 0;

    /**
     * Reads and returns an unsigned 64 bit value from the underlying byte stream.
     *
     * @return int8_t - An unsigned 64 bit value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual uint64_t getU64() = 0;

    /**
     * Reads and returns an 32 bit float value from the underlying byte stream.
     *
     * @param - Optional parameter to indicate the byte format of the FLOAT in the stream.
     * Default is BigEndian.
     *
     * @return float - An unsigned 32 bit float value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual float getFloat() = 0;

    /**
     * Reads and returns an 64 bit double value from the underlying byte stream.
     *
     * @return float - An unsigned 64 bit double value.
     *
     * @throws BufferUnderflowException - thrown if there are not enough bytes left in
     * the buffer to satisfy the requested read.
     */
    virtual double getDouble() = 0;

    /**
     * Reads a string from the underlying byte stream
     *
     * @param returnValue  - Reference to a string that will receive the value.  The
     * string will be cleared prior to reading the value.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getString(std::string& returnValue) = 0;

    /**
     * Reads a signed 8 bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to a signed 8bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getS8(int8_t& returnValue) = 0;

    /**
     * Reads an unsigned 8 bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to an unsigned 8bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getU8(uint8_t& returnValue) = 0;

    /**
     * Reads a signed 16bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to a signed 16bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getS16(int16_t& returnValue) = 0;

    /**
     * Reads an unsigned 16bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to an unsigned 16bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getU16(uint16_t& returnValue) = 0;

    /**
     * Reads a signed 32bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to a signed 32bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getS32(int32_t& returnValue) = 0;

    /**
     * Reads an unsigned 32bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to an unsigned 32bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getU32(uint32_t& returnValue) = 0;

    /**
     * Reads a signed 64bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to a signed 64bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getS64(int64_t& returnValue) = 0;

    /**
     * Reads an unsigned 64bit byte from the underlying byte stream
     *
     * @param returnValue  - Reference to an unsigned 64bit byte value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getU64(uint64_t& returnValue) = 0;

    /**
     * Reads an 32bit float byte from the underlying byte stream
     *
     * @param returnValue  - Reference to 32bit float value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getFloat(float& returnValue) = 0;

    /**
     * Reads an 64bit double byte from the underlying byte stream
     *
     * @param returnValue  - Reference to 46bit double value that will receive the
     * value read from the stream.
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int getDouble(double& returnValue) = 0;

    /**
     * Returns a pointer to the underlying buffer.
     *
     * @return Pointer to buffer
     */
    virtual const unsigned char* buffer() const = 0;

    /**
     * Reads the contents of the underlying byte stream into the buffer.
     *
     * @param buffer - pointer to an buffer to receive the bytes read from
     * the stream.
     *
     * @param bufferLength
     *
     * @return The number of bytes read.  BUFFER_UNDERFLOW if there are not
     * enough bytes in the underlying stream to satisfy the request.
     */
    virtual int get(int8_t* buffer, int bufferLength) = 0;

    /**
     * Write the entire contents of the underlying byte stream into a buffer.  If there
     * are fewer bytes available than the size of the buffer, only the available
     * bytes will be returned.
     *
     * @param buffer - pointer to an buffer to receive the bytes read from
     * the stream.
     *
     * @param bufferLength
     *
     * @return The number of bytes read.  This method will never return BUFFER_UNDERFLOW
     */
    virtual int getAll(int8_t* buffer, int bufferLength) = 0;

    /**
     * Flip prepares the buffer for input or output, depending on the last operation performed.
     * If this ByteBuffer was written to, then flip will prepare it for reading.  If it was last read
     * from, flip will reset the byte buffer for writing. The actual operation perform is the limit
     * is set to the current position and then the position is set to zero. If the mark is defined
     * then it is discarded.
     *
     * @return Reference to this byte buffer.
     */
    virtual ByteBuffer& flip();

    /**
     * Clears this ByteBuffer. The position is set to zero, the limit is set to the capacity, and the mark is discarded.
     */
    virtual void clear();

    /**
     *  Sets this buffer's mark at the current position. Use this method if you wish change the buffer's
     *  internal position and then set it back again (using reset());
     *
     * @return Reference to this ByteBuffer.
     */
    virtual ByteBuffer& mark();

    /**
     * Resets this buffer's position to the previously-marked position.
     *
     * @return Reference to this ByteBuffer.
     */
    virtual ByteBuffer& reset();

    /**
     * Skips over the current byte.  That is, move the position one byte forward.  If the
     * position is at the limit, the position is not changed.
     *
     * @return Reference to this ByteBuffer.
     */
    virtual ByteBuffer& skip();

    /**
     * Skips over count number of bytes.  If moving the position count bytes moves the
     * position beyond the limit, the position will not advance futher than then limit.
     *
     * @param count - the number of bytes to skip over.
     *
     * @return Reference to this ByteBuffer.
     */
    virtual ByteBuffer& skip(uint32_t count);

    /** setPosition
     *
     * Sets the current position.
     *
     * @param[in] newPosition - The new position value; must be non-negative and no larger
     * than the current limit
     *
     * @throws IllegalArgumentException if the new position is less than zero or greater than
     * the limmit of this buffer.
     *
     */
    virtual void setPosition(int newPosition);

    /**
     * Sets the limit of this ByteBuffer.
     *
     * @param newLimit - New limit. The value cannot be less than the current position nor greater
     * than the current capacity of the buffer;
     *
     * @throws IllegalArgumentException if the new limit is less than the current position or greater
     * than the capacity.
     */
    virtual void setLimit(int newLimit);

    /**
     * Returns the capacity of this byte buffer. The capacity is the total size of the
     * underlying buffer.   - Reference to an int that will receive the paramter value
     *
     * @return int - The capacity.
     */
    virtual int capacity() const;

    /**
     * Returns the position of this byte buffer.  The position is the index of the next
     * byte to be read or written. The position can never be negative or greater than the
     * limit.
     *
     * @return
     */
    virtual int position() const;

    /**
     * Returns this buffers limit. The limit is the index of the first element that
     * should not be read or written. The limit can never be negative nor greater than
     * the capacity.
     *
     * @return
     */
    virtual int limit() const;

    /**
     * Returns the remaning number of bytes that can be read from or written to, this
     * byte buffer.
     *
     * @return
     */
    int remaining() const;

    bool isReadOnly() const;

    inline bool operator==(const ByteBuffer& otherBuffer) const {
        return this == &otherBuffer;
    }

protected:
    ByteBuffer(int capacity, ByteOrder& byteOrder = ByteOrder::BigEndian);

    int _position;
    int _limit;
    int _capacity;
    int _mark;
    bool _readonly;
    ByteOrder& _byteOrder;

    void checkLimitToWrite(int extraBytes, int currentSize, int limit);
    void checkLimitToRead(int bytesWanted, int currentSize, int limit);

    bool isLimitOutsideReadBounds(int bytesWanted, int currentSize, int limit);

private:
    ByteBuffer& operator=(const ByteBuffer& otherBuffer);

};

}
} /* namespace zios */
#endif /* BYTEBUFFER_H_ */
