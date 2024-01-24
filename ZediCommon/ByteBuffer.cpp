/** @file ByteBuffer.cpp
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

#include "ByteBuffer.h"
#include "ByteOrder.h"

#include <netinet/in.h>
#include <stdint.h>
#include <cstddef>
#include <cstring>
#include <algorithm>

#include "BufferOverflowException.h"
#include "BufferUnderflowException.h"
#include "IllegalArgumentException.h"

#define min(m,n) ((m) < (n) ? (m) : (n))

namespace zios {
namespace common {

ByteBuffer::ByteBuffer(int capacity, ByteOrder& byteOrder) :
        _position(0),
        _limit(capacity),
        _capacity(capacity),
        _mark(-1),
        _readonly(false),
        _byteOrder(byteOrder) {
}

ByteBuffer::~ByteBuffer() {
}

ByteBuffer::ByteBuffer(const ByteBuffer& otherBuffer)  :
                _position(otherBuffer._position),
                _limit(otherBuffer._limit),
                _capacity(otherBuffer._capacity),
                _mark(otherBuffer._mark),
                _readonly(otherBuffer._readonly),
                _byteOrder(otherBuffer._byteOrder) {
}

ByteBuffer& ByteBuffer::flip() {
    _limit = position();
    _position = 0;
    return *this;
}

ByteBuffer& ByteBuffer::mark() {
    _mark = _position;
    return *this;
}

ByteBuffer& ByteBuffer::reset() {
    if (_mark >= 0)
        _position = _mark;
    return *this;
}

ByteBuffer& ByteBuffer::skip() {
    return skip(1);
}

void ByteBuffer::setPosition(int position) {
    if (position< 0 || position > _limit)
        THROW_NAMED_EXCEPTION(IllegalArgumentException, "The new position must be non-negative and no larger than the current limit");
    _position = position;
}

void ByteBuffer::setLimit(int newLimit) {
    if (newLimit < _position || newLimit > _capacity)
        THROW_NAMED_EXCEPTION(IllegalArgumentException, "The new position must be non-negative and no larger than the current limit");
    _limit = newLimit;
}

int ByteBuffer::capacity() const {
    return _capacity;
}

int ByteBuffer::position() const {
    return _position;
}

int ByteBuffer::limit() const {
    return _limit;
}

int ByteBuffer::remaining() const {
    return std::max(0, _limit - _position);
}

bool ByteBuffer::isReadOnly() const {
    return _readonly;
}

ByteBuffer& ByteBuffer::skip(uint32_t count) {
    _position += count;
    return *this;
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& otherBuffer) {
    _position = otherBuffer._position;
    _limit = otherBuffer._limit;
    _capacity = otherBuffer._capacity;
    _mark = otherBuffer._mark;
    _readonly = otherBuffer._readonly;
    return *this;
}

void ByteBuffer::clear() {
    _position = 0;
    _limit = _capacity;
}

void ByteBuffer::checkLimitToWrite(int extraBytes, int currentSize, int limit) {
    if (currentSize + extraBytes > limit)
        THROW_NAMED_EXCEPTION(BufferOverflowException, "");
}

void ByteBuffer::checkLimitToRead(int bytesWanted, int currentSize, int limit) {
    if (currentSize + bytesWanted > limit)
        THROW_NAMED_EXCEPTION(BufferUnderflowException, "");
}

bool ByteBuffer::isLimitOutsideReadBounds(int bytesWanted, int currentSize, int limit) {
    return (currentSize + bytesWanted > limit);
}

}
} /* namespace zios */
