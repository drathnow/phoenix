/** @file ReadOnlyByteBuffer.h
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

#ifndef BYTEBUFFERR_H_
#define BYTEBUFFERR_H_
#include <stdio.h>
#include <stdint.h>
#include <string>
#include "ByteBuffer.h"

namespace zios {
namespace common {

class ReadOnlyByteBuffer : public ByteBuffer {
public:
    ReadOnlyByteBuffer(const ReadOnlyByteBuffer& otherBuffer);
    ReadOnlyByteBuffer(const ReadOnlyByteBuffer& otherBuffer, int newPosition, ByteOrder& byteOrder = ByteOrder::BigEndian);
    ReadOnlyByteBuffer(const uint8_t* wrappedBuffer, uint32_t bufferLength, ByteOrder& byteOrder = ByteOrder::BigEndian);
    virtual ~ReadOnlyByteBuffer();

    ReadOnlyByteBuffer& operator=(const ReadOnlyByteBuffer& otherBuffer);

    virtual std::string getString();

    virtual int8_t getS8();
    virtual uint8_t getU8();

    virtual int16_t getS16();
    virtual uint16_t getU16();

    virtual int32_t getS32();
    virtual uint32_t getU32();

    virtual int64_t getS64();
    virtual uint64_t getU64();

    virtual float getFloat();
    virtual double getDouble();


    virtual int getString(std::string& returnValue);

    virtual int getS8(int8_t& returnValue);
    virtual int getU8(uint8_t& returnValue);

    virtual int getS16(int16_t& returnValue);
    virtual int getU16(uint16_t& returnValue);

    virtual int getS32(int32_t& returnValue);
    virtual int getU32(uint32_t& returnValue);

    virtual int getS64(int64_t& returnValue);
    virtual int getU64(uint64_t& returnValue);

    virtual int getFloat(float& returnValue);
    virtual int getDouble(double& returnValue);

    virtual inline const unsigned char* buffer() const { return _buffer; }

    virtual int get(int8_t* buffer, int bufferLength);
    virtual int getAll(int8_t* buffer, int bufferLength);

    virtual void putString(const std::string& str);
    virtual void put(const std::string& str);

    virtual void putS8(int8_t value);
    virtual void put(int8_t value);
    virtual void putU8(uint8_t value);
    virtual void put(uint8_t value);

    virtual void putS16(int16_t value);
    virtual void put(int16_t value);
    virtual void putU16(uint16_t value);
    virtual void put(uint16_t value);

    virtual void putS32(int32_t value);
    virtual void put(int32_t value);
    virtual void putU32(uint32_t value);
    virtual void put(uint32_t value);

    virtual void putU64(uint64_t value);
    virtual void put(uint64_t value);
    virtual void putS64(int64_t value);
    virtual void put(int64_t value);

    virtual void putFloat(float value);
    virtual void put(float value);
    virtual void putDouble(double value);
    virtual void put(double value);

    virtual int put(const int8_t* buffer, int bufferLength);
    virtual int putAll(std::istream& inputStream, int byteCount = 0);
    virtual ReadOnlyByteBuffer* slice() const;

private:
    const unsigned char* _buffer;
};

}
} /* namespace zios */
#endif /* BYTEBUFFERR_H_ */
