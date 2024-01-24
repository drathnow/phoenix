/** @file ByteBufferWrapper.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 10, 2013 by daver:
 * BugFix_ Oct 01, 2013 by eganb: problem in get/put float
 * IncrDev Oct 02, 2013 by eganb: simplified getxx routines
 * BugFix_ Oct 03, 2013 by eganb: added in missing underflow tests on getxxx routines.
 * @endhistory
 */

#include <cstring>
#include <netinet/in.h>
#include <algorithm>
#include "ByteBufferWrapper.h"
#include "BufferUnderflowException.h"
#include "BufferOverflowException.h"
#include "UnsupportedOperationException.h"

namespace zios {
namespace common {

ByteBufferWrapper::ByteBufferWrapper(unsigned char* buffer, uint32_t bufferLength, ByteOrder& byteOrder) :
                    ByteBuffer(bufferLength, byteOrder),
                    _buffer(buffer) {
}

ByteBufferWrapper::~ByteBufferWrapper() {
}

ByteBufferWrapper& ByteBufferWrapper::operator=(const ByteBufferWrapper& otherBuffer) {
    _buffer = otherBuffer._buffer;
    return *this;
}

ByteBufferWrapper* ByteBufferWrapper::slice() const {
    ByteBufferWrapper* newBuffer = new ByteBufferWrapper(*this);
    newBuffer->setPosition(0);
    return newBuffer;
}

void ByteBufferWrapper::putString(const std::string& str) {
    checkLimitToWrite(str.length(), _position, _limit);
    putU16(str.length());
    ::memcpy((void*)&_buffer[_position], str.c_str(), str.length());
    _position += str.length();
}

void ByteBufferWrapper::putS8(int8_t value)
{
    checkLimitToWrite(sizeof(int8_t), _position, _limit);
    ::memcpy((void*)&_buffer[_position], &value, sizeof(int8_t));
    _position += sizeof(int8_t);
}

void ByteBufferWrapper::putU8(uint8_t value)
{
    checkLimitToWrite(sizeof(uint8_t), _position, _limit);
    ::memcpy((void*)&_buffer[_position], &value, sizeof(uint8_t));
    _position += sizeof(int8_t);
}

void ByteBufferWrapper::putS16(int16_t value)
{
    checkLimitToWrite(sizeof(int16_t), _position, _limit);
    int16_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(int16_t));
    _position += sizeof(int16_t);
}

void ByteBufferWrapper::putU16(uint16_t value)
{
    checkLimitToWrite(sizeof(uint16_t), _position, _limit);
    uint16_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(uint16_t));
    _position += sizeof(uint16_t);
}

void ByteBufferWrapper::putS32(int32_t value)
{
    checkLimitToWrite(sizeof(int32_t), _position, _limit);
    int32_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(int32_t));
    _position += sizeof(int32_t);
}

void ByteBufferWrapper::putU32(uint32_t value)
{
    checkLimitToWrite(sizeof(uint32_t), _position, _limit);
    uint32_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(uint32_t));
    _position += sizeof(uint32_t);
}

void ByteBufferWrapper::putS64(int64_t value)
{
    checkLimitToWrite(sizeof(int64_t), _position, _limit);
    int64_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(int64_t));
    _position += sizeof(int64_t);
}

void ByteBufferWrapper::putU64(uint64_t value)
{
    checkLimitToWrite(sizeof(uint64_t), _position, _limit);
    uint64_t nValue = ByteOrder::nativeOrder().toByteOrder(value, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &nValue, sizeof(uint64_t));
    _position += sizeof(uint64_t);
}

void ByteBufferWrapper::putFloat(float value)
{
    checkLimitToWrite(sizeof(float), _position, _limit);
    fltcvt_t fvalue;
    ::memcpy(&fvalue.flt, &value, sizeof(float));
    uint32_t outValue = ByteOrder::nativeOrder().toByteOrder(fvalue.U32, _byteOrder);
    ::memcpy((void*)&_buffer[_position], &outValue, sizeof(uint32_t));
    _position += sizeof(float);
}

void ByteBufferWrapper::putDouble(double value)
{
    checkLimitToWrite(sizeof(double), _position, _limit);
    ::memcpy((void*)&_buffer[_position], &value, sizeof(double));
    _position += sizeof(double);
}

int ByteBufferWrapper::put(const int8_t* buffer, int bufferLength) {
    int bytesToWrite = std::min((_limit-_position), bufferLength);
    ::memcpy((void*)&_buffer[_position], buffer , bytesToWrite);
    _position += bufferLength;
    return bytesToWrite;
}

std::string ByteBufferWrapper::getString() {
    std::string result;
    uint32_t length = getU16();
    if (length > 0) {
        int8_t buf[length];
        get(buf, length);
        result.append((const char*)buf, length);
    }
    return result;
}

int8_t ByteBufferWrapper::getS8()
{
    checkLimitToRead(sizeof(int8_t), _position, _limit);
    int8_t value = (int8_t)_buffer[_position];
    _position++;
    return value;
}

uint8_t ByteBufferWrapper::getU8()
{
    checkLimitToRead(sizeof(int8_t), _position, _limit);
    uint8_t value = (uint8_t)_buffer[_position];
    _position++;
    return value;
}

int16_t ByteBufferWrapper::getS16()
{
    checkLimitToRead(sizeof(int16_t), _position, _limit);
    int16_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(int16_t));	// to make sure of alignment
    _position += sizeof(int16_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint16_t ByteBufferWrapper::getU16()
{
    checkLimitToRead(sizeof(uint16_t), _position, _limit);
    uint16_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(uint16_t));
    _position += sizeof(uint16_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

int32_t ByteBufferWrapper::getS32()
{
    checkLimitToRead(sizeof(int32_t), _position, _limit);
    int32_t value;
    ::memcpy(&value, &_buffer[_position], sizeof(int32_t));
    _position += sizeof(int32_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint32_t ByteBufferWrapper::getU32() {
    checkLimitToRead(sizeof(uint32_t), _position, _limit);
    uint32_t value;
    ::memcpy(&value,&_buffer[_position],sizeof(uint32_t));
    _position += sizeof(uint32_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

int64_t ByteBufferWrapper::getS64() {
    checkLimitToRead(sizeof(uint64_t), _position, _limit);
    int64_t value;
    ::memcpy(&value, &_buffer[_position], sizeof(int64_t));
    _position += sizeof(int64_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

uint64_t ByteBufferWrapper::getU64() {
    checkLimitToRead(sizeof(uint64_t), _position, _limit);
    uint64_t value;
    ::memcpy(&value, &_buffer[_position], sizeof(uint64_t));
    _position += sizeof(uint64_t);
    return _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
}

float ByteBufferWrapper::getFloat() {
    checkLimitToRead(sizeof(float), _position, _limit);
    fltcvt_t fvalue;
    ::memcpy(&fvalue.U32, &_buffer[_position], sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(float);
    fvalue.U32 = _byteOrder.toByteOrder(fvalue.U32, ByteOrder::nativeOrder());
    return fvalue.flt;
}

double ByteBufferWrapper::getDouble() {
    return 0;
}

//-------------------------------------------

int ByteBufferWrapper::getString(std::string& retValue) {
    uint16_t length = 0;
    int status;
    if ((status = getU16(length)) < 0)
        return status;
    if (length > 0) {
        int8_t buf[length];
        get(buf, length);
        retValue.clear();
        retValue.append((const char*)buf, length);
    }
    return length;
}

int ByteBufferWrapper::getS8(int8_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int8_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    returnValue = (int8_t)_buffer[_position];
    _position++;
    return sizeof(int8_t);
}

int ByteBufferWrapper::getU8(uint8_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int8_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    returnValue = (uint8_t)_buffer[_position];
    _position++;
    return sizeof(uint8_t);
}

int ByteBufferWrapper::getS16(int16_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int16_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int16_t));   // to make sure of alignment
    _position += sizeof(int16_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int16_t);
}

int ByteBufferWrapper::getU16(uint16_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint16_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int16_t));   // to make sure of alignment
    _position += sizeof(uint16_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint16_t);
}

int ByteBufferWrapper::getS32(int32_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int32_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue, &_buffer[_position], sizeof(int32_t));   // to make sure of alignment
    _position += sizeof(int32_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int32_t);
}

int ByteBufferWrapper::getU32(uint32_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint32_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(uint32_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint32_t);
}

int ByteBufferWrapper::getS64(int64_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(int64_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(int64_t));
    _position += sizeof(int64_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(int64_t);
}

int ByteBufferWrapper::getU64(uint64_t& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(uint64_t), _position, _limit))
        return BUFFER_UNDERFLOW;
    ::memcpy(&returnValue,&_buffer[_position],sizeof(uint64_t));
    _position += sizeof(uint64_t);
    returnValue = _byteOrder.toByteOrder(returnValue, ByteOrder::nativeOrder());
    return sizeof(uint64_t);
}

int ByteBufferWrapper::getFloat(float& returnValue) {
    if (isLimitOutsideReadBounds(sizeof(float), _position, _limit))
        return BUFFER_UNDERFLOW;
    uint32_t value;
    fltcvt_t fvalue;
    ::memcpy(&value,&_buffer[_position],sizeof(uint32_t));   // to make sure of alignment
    _position += sizeof(float);
    fvalue.U32 = _byteOrder.toByteOrder(value, ByteOrder::nativeOrder());
    returnValue = fvalue.flt;
    return sizeof(float);
}

int ByteBufferWrapper::getDouble(double& returnValue) {
    THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Method not supported");
    return 0;
}

int ByteBufferWrapper::get(int8_t* buffer, int bufferLength) {
    if (_position == _limit)
        THROW_NAMED_EXCEPTION(BufferUnderflowException, "");
    int whatsLeft = _limit - _position;
    int bytesToRead = std::min(whatsLeft, bufferLength);
    ::memcpy((void*)buffer, &_buffer[_position], bytesToRead);
    _position += bytesToRead;
    return bytesToRead;
}

int ByteBufferWrapper::getAll(int8_t* buffer, int bufferLength) {
    if (_position == _limit)
        THROW_NAMED_EXCEPTION(BufferUnderflowException, "");
    int whatsLeft = _limit - _position;
    int bytesToRead = std::min(whatsLeft, bufferLength);
    ::memcpy((void*)buffer, &_buffer[_position], bytesToRead);
    _position += bytesToRead;
    return bytesToRead;
}

int ByteBufferWrapper::putAll(std::istream& inputStream, int byteCount) {
    if (byteCount == 0) {
        inputStream.seekg(0, inputStream.end);
        byteCount = inputStream.tellg();
    }

    if (_limit - _position < byteCount)
        THROW_NAMED_EXCEPTION(BufferOverflowException, "putAll(istream) failed");

    inputStream.seekg(0, inputStream.beg);
    inputStream.read((char*)&_buffer[_position], byteCount);
    _position += byteCount;
    return byteCount;
}

}
} /* namespace zios */
