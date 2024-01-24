/** @file ByteBufferWrapper.h
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
 * @endhistory
 */

#ifndef BYTEBUFFERWRAPPER_H_
#define BYTEBUFFERWRAPPER_H_
#include "ByteBuffer.h"
#include "ByteOrder.h"

#include <string>
#include <stdint.h>

namespace zios {
namespace common {

typedef union {
    float flt;
    uint32_t   U32;
} fltcvt_t;

class ByteBufferWrapper : public ByteBuffer {
public:
    ByteBufferWrapper(uint8_t* buffer, uint32_t bufferLength, ByteOrder& byteOrder = ByteOrder::BigEndian);
    virtual ~ByteBufferWrapper();

    virtual void putString(const std::string& str);

    virtual void putS8(int8_t value);
    virtual void putU8(uint8_t value);

    virtual void putS16(int16_t value);
    virtual void putU16(uint16_t value);

    virtual void putS32(int32_t value);
    virtual void putU32(uint32_t value);

    virtual void putS64(int64_t value);
    virtual void putU64(uint64_t value);

    virtual void putFloat(float value);
    virtual void putDouble(double value);

    virtual int putAll(std::istream& inputStream, int byteCount = 0);
    virtual int put(const int8_t* buffer, int bufferLength);

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

    virtual ByteBufferWrapper* slice() const;

    virtual int get(int8_t* buffer, int bufferLength);
    virtual int getAll(int8_t* buffer, int bufferLength);
    virtual inline const uint8_t* buffer() const { return _buffer; }

    virtual ByteBufferWrapper& operator=(const ByteBufferWrapper& otherBuffer);

private:
    uint8_t* _buffer;
};

}
} /* namespace zios */
#endif /* BYTEBUFFERWRAPPER_H_ */
