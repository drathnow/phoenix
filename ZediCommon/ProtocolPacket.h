/** @file ProtocolPacket.h
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

#ifndef PROTOCOLPACKET_H_
#define PROTOCOLPACKET_H_

#include <stdint.h>

namespace log4cplus {
    class Logger;
}

namespace zios {
namespace common {

struct Region {
    uint32_t offset;
    uint32_t length;
};

class BasePacket {
public:
    virtual ~BasePacket();

    virtual uint8_t* header() const;
    virtual uint32_t headerLength() const;

    virtual uint8_t* body() const;
    virtual uint32_t bodyLength() const;
    virtual void setBodyLength(uint32_t length);

    virtual uint8_t* trailer() const;
    virtual uint32_t trailerLength() const;

    /**
     * Resets this Packet. The header, body, and trailer are all set back to their
     * original offset and size.  If requested, the buffer is zeroed.
     *
     * @param zeroBuffer - Indicates if the internal buffer should be cleared. i.e. its contents
     * set to zero.  true to clear, false to not clear
     */
    virtual void reset(bool zeroBuffer = false);

    virtual uint32_t packetLength() const;
    virtual uint32_t bufferLength() const;

    /**
     * Will resize the internal buffer to the given size.  The buffer will remain at that size until the next call to clear
     *
     * @param toSize - The new size of the internal buffer.
     *
     * @return true it is was resized.  False if the buffer is wrapped.  Wrapped buffers cannot be rewrapped.
     */
    virtual bool resizeBody(int toSize);

    friend class BasePacketTest;

protected:
    BasePacket(uint32_t bufferSize, uint32_t bodyOffset, uint32_t bodyLength);
    BasePacket(uint8_t* wrappedBuffer, uint32_t bufferLength, uint32_t bodyOffset, uint32_t bodyLength);

    Region _header;
    Region _body;
    Region _trailer;

protected:
    BasePacket(const BasePacket& otherPacket);
    BasePacket& operator=(const BasePacket& otherPacket);

    uint8_t* _buffer;
    uint32_t _originalBodyLength;
    uint32_t _originalBodyOffset;
    uint32_t _originalBufferLength;
    uint32_t _currentbufferLength;
    bool _wrapped;

};

class TransmitProtocolPacket : public BasePacket {
public:
    TransmitProtocolPacket(uint32_t bufferSize, uint32_t bodyOffset, uint32_t bodyLength);
    TransmitProtocolPacket(uint8_t* wrappedBuffer, uint32_t bufferLength, uint32_t bodyOffset, uint32_t bodyLength);
    virtual ~TransmitProtocolPacket();
    virtual void addHeader(uint32_t length);
    virtual void addTrailer(uint32_t length);
    virtual void merge();

#ifdef foo
    int send(int handle, int timeoutSeconds, log4cplus::Logger& protocolLogger);
#endif

    inline bool operator==(const TransmitProtocolPacket& otherPacket) const {
        return this == &otherPacket;
    }

    friend class TransmitProtocolPacketTest;
};

class ReceiveProtocolPacket : public BasePacket {
public:
    ReceiveProtocolPacket(uint32_t bufferSize);
    ReceiveProtocolPacket(uint8_t* wrappedBuffer, uint32_t bufferLength);
    virtual void extractHeader(uint32_t length);
    virtual void extractTrailer(uint32_t length);
    virtual void trim();
};

}
} /* namespace zios */
#endif /* PROTOCOLPACKET_H_ */
