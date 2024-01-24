#pragma once

#ifndef VALUE_H_
#define VALUE_H_

#include <stdint.h>
#include <string>
#include <sstream>
#include <regex>
#include <Exception.h>
#include <ByteBuffer.h>

namespace zios::domain
{

using zios::common::Exception;
using zios::common::ByteBuffer;

class Value
{
public:
    static const std::regex NUMBER_REGEX;

    Value() = default;
    virtual ~Value() = default;

    virtual operator uint8_t() const = 0;
    virtual operator int8_t() const = 0;
    virtual operator uint16_t() const = 0;
    virtual operator int16_t() const = 0;
    virtual operator uint32_t() const = 0;
    virtual operator int32_t() const = 0;
    virtual operator uint64_t() const = 0;
    virtual operator int64_t() const = 0;
    virtual operator float() const = 0;
    virtual operator double() const = 0;
    virtual operator std::string() const = 0;

    virtual bool assign(uint8_t newValue) = 0;
    virtual bool assign(int8_t newValue) = 0;
    virtual bool assign(uint16_t newValue) = 0;
    virtual bool assign(int16_t newValue) = 0;
    virtual bool assign(uint32_t newValue) = 0;
    virtual bool assign(int32_t newValue) = 0;
    virtual bool assign(uint64_t newValue) = 0;
    virtual bool assign(int64_t newValue) = 0;
    virtual bool assign(float newValue) = 0;
    virtual bool assign(double newValue) = 0;
    virtual bool assign(const std::string& newValue) = 0;
    virtual bool assign(std::string&& newValue) = 0;

    virtual Value& operator=(const Value& otherValue) = 0;

    virtual bool operator==(const Value& otherValue) const = 0;
    virtual bool operator<(const Value& otherValue) const = 0;
    virtual bool operator>(const Value& otherValue) const = 0;
    virtual bool operator<=(const Value& otherValue) const = 0;
    virtual bool operator>=(const Value& otherValue) const = 0;

    virtual ByteBuffer& serialize(ByteBuffer& byteBuffer) const = 0;

};


template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class NumericValue : public Value
{
public:
    NumericValue() = default;
    NumericValue(T value) : _value(value) {}
    NumericValue(const std::string& stringValue)
    {
        THROW_EXCEPTION_IF(false == assign(stringValue), "String value does not convert to numeric data type");
    }

    NumericValue(const NumericValue<T>& otherValue) : _value(otherValue._value) {}
    NumericValue(NumericValue<T> &&otherValue) : _value(otherValue._value) {}
    ~NumericValue() = default;

    NumericValue<T>& operator=(const NumericValue<T>& otherValue)
    {
        _value = otherValue._value;
        return *this;
    }

    operator uint8_t() const { return (uint8_t)_value; }
    operator int8_t() const { return (int8_t)_value; }
    operator uint16_t() const { return (uint16_t)_value; }
    operator int16_t() const { return (int16_t)_value; }
    operator uint32_t() const { return (uint32_t)_value; }
    operator int32_t() const { return (int32_t)_value; }
    operator uint64_t() const { return (uint64_t)_value; }
    operator int64_t() const { return (int64_t)_value; }
    operator float() const { return (float)_value; }
    operator double() const { return (double)_value; }
    operator std::string() const
    {
        std::ostringstream oss;
        oss << _value;
        return std::move(oss.str());
    }

    bool assign(uint8_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(int8_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(uint16_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(int16_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(uint32_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(int32_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(uint64_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(int64_t value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(float value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(double value)
    {
        if (isValidRange((double)value) == false)
        {
            return false;
        }
        _value = static_cast<T>(value);
        return true;
    }

    bool assign(const std::string &newValue)
    {
        return assign((std::string&&)&newValue);
    }

    bool assign(std::string &&newValue)
    {
        if (std::regex_match(newValue, NUMBER_REGEX))
        {
            _value = static_cast<T>(std::stod(newValue));
            return true;
        }
        return false;
    }

    Value& operator=(const Value& otherValue)
    {
        _value = static_cast<T>(otherValue);
        return *this;
    }

    bool operator==(const Value& otherValue) const
    {
        return _value == static_cast<T>(otherValue);
    }

    bool operator<(const Value& otherValue) const
    {
        return _value < static_cast<T>(otherValue);
    }

    bool operator>(const Value& otherValue) const
    {
        return _value > static_cast<T>(otherValue);
    }

    bool operator<=(const Value& otherValue) const
    {
        return _value <= static_cast<T>(otherValue);
    }

    bool operator>=(const Value& otherValue) const
    {
        return _value >= static_cast<T>(otherValue);
    }

    ByteBuffer& serialize(ByteBuffer& byteBuffer) const
    {
        byteBuffer.put(_value);
        return byteBuffer;
    }

    ByteBuffer& operator<<(ByteBuffer& byteBuffer) const
    {
        byteBuffer.put(_value);
        return byteBuffer;
    }

private:
    T _value;

    bool isValidRange(double value)
    {
        return (value >= std::numeric_limits<T>::min() && value <= std::numeric_limits<T>::max());
    }
};

class StringValue : public Value
{
public:
    StringValue() = default;
    StringValue(const std::string& value) : _value(value) {}
    ~StringValue() = default;

    StringValue(const StringValue& otherValue) = default;
    StringValue(StringValue &&otherValue) = default;

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
    bool assign(const std::string& value);
    bool assign(std::string&& value);

    ByteBuffer& serialize(ByteBuffer& byteBuffer) const;

    ByteBuffer& operator<<(ByteBuffer& byteBuffer) const
    {
        byteBuffer.put(_value);
        return byteBuffer;
    }

    StringValue& operator=(const StringValue& otherValue)
    {
        _value = otherValue._value;
        return *this;
    }

    StringValue& operator=(StringValue&& otherValue)
    {
        _value = std::move(otherValue._value);
        return *this;
    }

    Value& operator=(const Value& otherValue)
    {
        _value = (std::string)otherValue;
        return *this;
    }

    bool operator==(const Value& otherValue) const
    {
        return _value == (std::string)otherValue;
    }

    bool operator<(const Value& otherValue) const
    {
        return _value < (std::string)otherValue;
    }

    bool operator>(const Value& otherValue) const
    {
        return _value > (std::string)otherValue;
    }

    bool operator<=(const Value& otherValue) const
    {
        return _value <= (std::string)otherValue;
    }

    bool operator>=(const Value& otherValue) const
    {
        return _value >= (std::string)otherValue;
    }

private:
    std::string _value;
};

} /* namespace zios::domain */

#endif /* VALUE_H_ */
