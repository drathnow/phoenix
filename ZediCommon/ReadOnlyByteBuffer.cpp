/** @file ReadOnlyByteBuffer.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 9, 2013 by daver:
 * @endhistory
 */

#include <cassert>
#include <cstring>
#include <netinet/in.h>
#include "ReadOnlyByteBuffer.h"
#include "BufferUnderflowException.h"

#define min(m,n) ((m) < (n) ? (m) : (n))

typedef union {
    float flt;
    uint32_t   U32;
    char chars[4];
} fltcvt_t;

namespace zios {
namespace common {

ReadOnlyByteBuffer::ReadOnlyByteBuffer(const uint8_t* wrappedBuffer, uint32_t bufferLength, ByteOrder& byteOrder) :
    ByteBuffer(bufferLength, byteOrder),
    _buffer(wrappedBuffer) {
}

ReadOnlyByteBuffer::~ReadOnlyByteBuffer() {
}

ReadOnlyByteBuffer::ReadOnlyByteBuffer(const ReadOnlyByteBuffer& otherBuffer) :
        ByteBuffer(*this) {
    _buffer = otherBuffer._buffer;
}

ReadOnlyByteBuffer::ReadOnlyByteBuffer(const ReadOnlyByteBuffer& otherBuffer, int newPosition, ByteOrder& byteOrder) :
        ByteBuffer(otherBuffer._capacity, byteOrder) {
    _buffer = otherBuffer._buffer;
    _position = 0;
}

ReadOnlyByteBuffer& ReadOnlyByteBuffer::operator=(const ReadOnlyByteBuffer& otherBuffer) {
    _buffer = otherBuffer._buffer;
    _byteOrder = otherBuffer._byteOrder;
    return *this;
}

std::string ReadOnlyByteBuffer::getString() {
    std::string result;
    uint32_t length = getU16();
    if (length > 0) {
        int8_t buf[length];
        get(buf, length);
        result.append((const char*)buf, length);
    }
    return result;
}

int8_t ReadOnlyByteBuffer::getS8() {
    checkLimitToRead(sizeof(int8_t), _position, _limit);
    int8_t value = (int8_t)_buffer[_position];
    _position++;
    return value;
}

uint8_t ReadOnlyByteBuffer::getU8() {
    checkLimitToRead(sizeof(int8_t), _position, _limit);
    uint8_t value = (uint8_t)_buffer[_position];
    _position++;
    return value;
}

int16_t ReadOnlyByteBuffer::getS16() {
    checkLimitToRead(sizeof(int16_t), _position, _limit);
    int16_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(int16_t));   // to make sure of alignment
    _position += sizeof(int16_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint16_t ReadOnlyByteBuffer::getU16() {
    checkLimitToRead(sizeof(uint16_t), _position, _limit);
    uint16_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(uint16_t));
    _position += sizeof(uint16_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

int32_t ReadOnlyByteBuffer::getS32() {
    checkLimitToRead(sizeof(int32_t), _position, _limit);
    int32_t value;
    ::memcpy(&value, &_buffer[_position], sizeof(int32_t));
    _position += sizeof(int32_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint32_t ReadOnlyByteBuffer::getU32() {
    checkLimitToRead(sizeof(uint32_t), _position, _limit);
    uint32_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(uint32_t));
    _position += sizeof(uint32_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

int64_t ReadOnlyByteBuffer::getS64() {
    checkLimitToRead(sizeof(int64_t), _position, _limit);
    int64_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(int64_t));
    _position += sizeof(int64_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint64_t ReadOnlyByteBuffer::getU64() {
    checkLimitToRead(sizeof(uint64_t), _position, _limit);
    uint64_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(uint64_t));
    _position += sizeof(uint64_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

float ReadOnlyByteBuffer::getFloat() {
    checkLimitToRead(sizeof(float), _position, _limit);
    fltcvt_t fvalue;
    ::memcpy(&fvalue.U32, &_buffer[_position],sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(float);
    fvalue.U32 = _byteOrder.toByteOrder(fvalue.U32, ByteOrder::nativeOrder());
    return fvalue.flt;
}

double ReadOnlyByteBuffer::getDouble() {
    return 0;
}

int ReadOnlyByteBuffer::get(int8_t* buffer, int bufferLength) {
    if (_position == _limit)
        return BUFFER_UNDERFLOW;
    int whatsLeft = _limit - _position;
    int bytesToRead = min(whatsLeft, bufferLength);
    ::memcpy((void*)buffer, &_buffer[_position], bytesToRead);
    _position += bytesToRead;
    return bytesToRead;
}

int ReadOnlyByteBuffer::getAll(int8_t* buffer, int bufferLength) {
    if (_position == _limit)
        THROW_NAMED_EXCEPTION(BufferUnderflowException, "");
    int whatsLeft = _limit - _position;
    int bytesToRead = min(whatsLeft, bufferLength);
    ::memcpy((void*)buffer, &_buffer[_position], bytesToRead);
    _position += bytesToRead;
    return bytesToRead;
}

int ReadOnlyByteBuffer::getString(std::string& retValue) {
    uint16_t length = 0;
    int status;
    if ((status = getU16(length)) < 0)
        return status;
    if (length > 0) {
        int8_t buf[length];
        get(buf, length);
        retValue.append((const char*)buf, length);
    }
    return length;
}

int ReadOnlyByteBuffer::getS8(int8_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int8_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    returnValue = (int8_t)_buffer[_position];
    _position++;
    return sizeof(int8_t);
}

int ReadOnlyByteBuffer::getU8(uint8_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int8_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    returnValue = (uint8_t)_buffer[_position];
    _position++;
    return sizeof(uint8_t);
}

int ReadOnlyByteBuffer::getS16(int16_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int16_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int16_t));   // to make sure of alignment
    _position += sizeof(int16_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int16_t);
}

int ReadOnlyByteBuffer::getU16(uint16_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint16_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int16_t));   // to make sure of alignment
    _position += sizeof(uint16_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint16_t);
}

int ReadOnlyByteBuffer::getS32(int32_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int32_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int32_t));   // to make sure of alignment
    _position += sizeof(int32_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int32_t);
}

int ReadOnlyByteBuffer::getU32(uint32_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint32_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(uint32_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint32_t);
}

int ReadOnlyByteBuffer::getS64(int64_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int64_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int64_t));
    _position += sizeof(int64_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int64_t);
}

int ReadOnlyByteBuffer::getU64(uint64_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint64_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(uint64_t));
    _position += sizeof(uint64_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint64_t);
}

int ReadOnlyByteBuffer::getFloat(float& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(float), _position, _limit))
        return BUFFER_UNDERFLOW;
    fltcvt_t fvalue;
    ::memcpy(&fvalue.U32,&_buffer[_position],sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(float);
    fvalue.U32 = _byteOrder.toByteOrder(fvalue.U32, ByteOrder::nativeOrder());
    returnValue = fvalue.flt;
    return sizeof(float);
}

int ReadOnlyByteBuffer::getDouble(double& returnValue) {
    return 0;
}

void ReadOnlyByteBuffer::putS8(int8_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(int8_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putU8(uint8_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(uint8_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putS16(int16_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(int16_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putU16(uint16_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(uint16_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putS32(int32_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(int32_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putU32(uint32_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(uint32_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putU64(uint64_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(uint64_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putS64(int64_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(int64_t value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putFloat(float value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(float value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::putDouble(double value) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(double value) {
    assert(isReadOnly() == false);
}


int ReadOnlyByteBuffer::put(const int8_t* buffer, int bufferLength) {
    assert(isReadOnly() == false);
    return 0;
}

int ReadOnlyByteBuffer::putAll(std::istream& inputStream, int byteCount) {
    assert(isReadOnly() == false);
    return 0;
}

void ReadOnlyByteBuffer::putString(const std::string& str) {
    assert(isReadOnly() == false);
}

void ReadOnlyByteBuffer::put(const std::string& str) {
    assert(isReadOnly() == false);
}

ReadOnlyByteBuffer* ReadOnlyByteBuffer::slice() const
{
    return new ReadOnlyByteBuffer(*this, 0);
}

}
} /* namespace zios */
