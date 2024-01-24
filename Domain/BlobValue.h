#pragma once

#ifndef BLOBVALUE_H_
#define BLOBVALUE_H_

#include "Value.h"

namespace zios::common
{
    class File;
}

namespace zios::domain
{

class BlobValue: public Value
{
public:
    static const uint32_t MAX_MEMORY_SIZE = 2048;

    BlobValue();
    BlobValue(const uint8_t* value, uint32_t size);
    BlobValue(const zios::common::File& file);
    ~BlobValue();

    BlobValue(const BlobValue &otherValue) = delete;
    BlobValue(BlobValue &&otherValue);

    BlobValue& operator=(const BlobValue& otherValue) = delete;

    operator uint8_t() const;
    operator int8_t() const;
    operator uint16_t() const;
    operator int16_t() const;
    operator uint32_t() const;
    operator int32_t() const;
    operator uint64_t() const;
    operator int64_t() const;
    operator float() const;
    operator double() const;
    operator std::string() const;

    bool assign(uint8_t newValue);
    bool assign(int8_t newValue);
    bool assign(uint16_t newValue);
    bool assign(int16_t newValue);
    bool assign(uint32_t newValue);
    bool assign(int32_t newValue);
    bool assign(uint64_t newValue);
    bool assign(int64_t newValue);
    bool assign(float newValue);
    bool assign(double newValue);
    bool assign(const std::string& newValue);
    bool assign(std::string&& newValue);

    Value& operator=(const Value& otherValue);

    bool operator==(const Value& otherValue) const;
    bool operator<(const Value& otherValue) const;
    bool operator>(const Value& otherValue) const;
    bool operator<=(const Value& otherValue) const;
    bool operator>=(const Value& otherValue) const;

    uint32_t size() const;
    uint32_t append(const uint8_t* value, uint32_t size);

    ByteBuffer& serialize(ByteBuffer& byteBuffer) const;
    ByteBuffer& deserialize(ByteBuffer& byteBuffer);

    friend class BlobValueTest;

private:
    zios::common::File* _file;
    uint8_t* _value;
    uint32_t _size;
};

} /* namespace zios */

#endif /* BLOBVALUE_H_ */
