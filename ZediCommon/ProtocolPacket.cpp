/** @file ProtocolPacket.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 27, 2013 by daver:
 * @endhistory
 */

#include "ProtocolPacket.h"
#include <log4cplus/loggingmacros.h>
#include <cassert>
#include <cstring>
#include <ctime>
#include "MonotonicTime.h"

#include "Utl.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <cerrno>

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;

BasePacket::BasePacket(uint32_t bufferSize, uint32_t bodyOffset, uint32_t bodyLength) :
        _buffer(new uint8_t[bufferSize]),
                _originalBodyLength(bodyLength),
                _originalBodyOffset(bodyOffset),
                _originalBufferLength(bufferSize),
                _currentbufferLength(bufferSize),
                _wrapped(false)
{
    reset(true);
}

BasePacket::BasePacket(uint8_t* wrappedBuffer, uint32_t bufferLength, uint32_t bodyOffset, uint32_t bodyLength) :
        _buffer(wrappedBuffer),
                _originalBodyLength(bodyLength),
                _originalBodyOffset(bodyOffset),
                _originalBufferLength(bufferLength),
                _currentbufferLength(bufferLength),
                _wrapped(true)
{
    reset(true);
}

BasePacket::BasePacket(const BasePacket& otherPacket) :
        _buffer(NULL),
                _originalBodyLength(0),
                _originalBodyOffset(0),
                _originalBufferLength(0),
                _currentbufferLength(0),
                _wrapped(false)
{
}

BasePacket::~BasePacket()
{
    if (_wrapped == false)
        delete[] _buffer;
}

void BasePacket::reset(bool zeroBuffer)
{
    if (_currentbufferLength != _originalBufferLength) {
        delete _buffer;
        _currentbufferLength = _originalBufferLength;
        _buffer = new uint8_t[_currentbufferLength];
    }
    _body.offset = _originalBodyOffset;
    _body.length = _originalBodyLength;
    _header.length = 0;
    _header.offset = _originalBodyOffset;
    _trailer.length = 0;
    _trailer.offset = _body.offset + _body.length;
    if (zeroBuffer)
        ::memset(_buffer, 0, _currentbufferLength);
}

uint8_t* BasePacket::header() const
{
    return &_buffer[_header.offset];
}

uint32_t BasePacket::headerLength() const
{
    return _header.length;
}

uint8_t* BasePacket::body() const
{
    return &_buffer[_body.offset];
}

uint32_t BasePacket::bodyLength() const
{
    return _body.length;
}

void BasePacket::setBodyLength(uint32_t length)
{
    _body.length = length;
    _trailer.offset = _body.offset + _body.length;
    _trailer.length = 0;
    assert(&_buffer[_body.offset + _body.length] <= &_buffer[_currentbufferLength]);
}

uint8_t* BasePacket::trailer() const
{
    return &_buffer[_trailer.offset];
}

uint32_t BasePacket::trailerLength() const
{
    return _trailer.length;
}

uint32_t BasePacket::packetLength() const
{
    return _header.length + _body.length + _trailer.length;
}

uint32_t BasePacket::bufferLength() const
{
    return _currentbufferLength;
}

bool BasePacket::resizeBody(int toSize)
{
    if (_wrapped)
        return false;

    int headerSize = _originalBodyOffset - 1;
    int trailerSize = _originalBufferLength - (headerSize + _originalBodyLength);
    _currentbufferLength = toSize + headerSize + trailerSize;
    delete _buffer;
    _buffer = new uint8_t[_currentbufferLength];

    _body.offset = _originalBodyOffset;
    _body.length = toSize;
    _header.length = 0;
    _header.offset = _originalBodyOffset;
    _trailer.length = 0;
    _trailer.offset = _body.offset + _body.length;

    return true;
}

TransmitProtocolPacket::TransmitProtocolPacket(uint32_t bufferSize, uint32_t bodyOffset, uint32_t bodyLength) :
        BasePacket(bufferSize, bodyOffset, bodyLength)
{
}

TransmitProtocolPacket::TransmitProtocolPacket(uint8_t* wrappedBuffer, uint32_t bufferLength, uint32_t bodyOffset, uint32_t bodyLength) :
        BasePacket(wrappedBuffer, bufferLength, bodyOffset, bodyLength)
{
}

TransmitProtocolPacket::~TransmitProtocolPacket()
{
}

void TransmitProtocolPacket::addHeader(uint32_t length)
{
    _header.offset -= length;
    _header.length += length;
    assert(&_buffer[_header.offset] >= &_buffer[0]);
}

void TransmitProtocolPacket::addTrailer(uint32_t length)
{
    _trailer.length = length;
    assert(&_buffer[_trailer.offset + _trailer.length] < &_buffer[_currentbufferLength]);
}

void TransmitProtocolPacket::merge()
{
    _body.offset = _header.offset;
    _body.length = _header.length + _body.length + _trailer.length;
    _header.length = 0;
    _trailer.offset = _body.offset + _body.length;
    _trailer.length = 0;
    assert(&_buffer[_header.offset] >= &_buffer[0]);
    assert(&_buffer[_header.offset + _header.length] == &_buffer[_body.offset]);
    assert(&_buffer[_body.offset + _body.length] == &_buffer[_trailer.offset]);
    assert(&_buffer[_trailer.offset + _trailer.length] <= &_buffer[_currentbufferLength]);
}

//
// Yes, this is "commented" out for not.  This is really the way it should be done but time
// is needed to test this.  We will leave it disabled for the time being
//
#ifdef foo
int TransmitProtocolPacket::send(int _socketDescriptor, int _writeTimeoutSeconds, log4cplus::Logger& protocolLogger) {
    MonotonicTime _systemTime;
    int totalBytesSend = 0;

    bool result = true;
    time_t startTime = _systemTime.getTimeSec();
    int whatsLeft = _currentbufferLength;

    do {
        int startPos = _currentbufferLength - whatsLeft;
        int bytesSent = ::write(_socketDescriptor, (void*)&_buffer[startPos], whatsLeft);
        if (bytesSent < 0) {
            bytesSent = 0;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                fd_set fdset;
                struct timeval tv;
                FD_ZERO(&fdset);
                FD_SET(_socketDescriptor, &fdset);
                tv.tv_sec = _writeTimeoutSeconds;
                tv.tv_usec = 0;
                if (::select(_socketDescriptor + 1, NULL, &fdset, NULL, &tv) < 1) {
                    if (tv.tv_sec == 0 && tv.tv_usec == 0)
                    LOG4CPLUS_ERROR(logger, "Write operation timed out.");
                    else
                    LOG4CPLUS_ERROR(logger, "Unable to select on socket: " << ::strerror(errno) << ". Closing socket.");
                    result = false;
                    bytesSent = whatsLeft; // To exit loop
                }
            } else {
                LOG4CPLUS_ERROR(logger, "Unable to write to socket: " << ::strerror(errno) << ".  Closing socket.");
                result = false;
                bytesSent = whatsLeft; // To exit loop
            }
        } else {
            totalBytesSend += bytesSent;
            if (protocolLogger.isEnabledFor(TRACE_LOG_LEVEL))
            LOG4CPLUS_TRACE(protocolLogger, "Trx(" << bytesSent << "): " << Utl::bytesAsHexString(&_buffer[startPos], bytesSent));
        }
        if (::difftime(startTime, _systemTime.getTimeSec()) > _writeTimeoutSeconds) {
            LOG4CPLUS_ERROR(logger, "Write operation timed out.  Closing socket.");
            result = false;
            bytesSent = whatsLeft; // To exit loop
        }
        whatsLeft -= bytesSent;
    }while (whatsLeft > 0);

    return totalBytesSend;
}
#endif

ReceiveProtocolPacket::ReceiveProtocolPacket(uint32_t bufferSize) :
        BasePacket(bufferSize, 0, bufferSize)
{

}

ReceiveProtocolPacket::ReceiveProtocolPacket(uint8_t* wrappedBuffer, uint32_t bufferLength) :
        BasePacket(wrappedBuffer, bufferLength, 0, bufferLength)
{
}

void ReceiveProtocolPacket::extractHeader(uint32_t length)
{
    _header.length += length;
    _body.offset += length;
    _body.length -= length;
}

void ReceiveProtocolPacket::extractTrailer(uint32_t length)
{
    _trailer.length += length;
    _trailer.offset -= length;
    _body.length -= length;
}

void ReceiveProtocolPacket::trim()
{
    _header.offset = _body.offset;
    _header.length = 0;
    _trailer.length = 0;
}

}
} /* namespace zios */
