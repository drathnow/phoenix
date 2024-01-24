#include <stdint.h>
#include <string>
#include <sstream>


#include "Value.h"

namespace zios::domain
{

const std::regex Value::NUMBER_REGEX("^[-+]?[0-9]+([.][0-9]+)?$");

StringValue::operator uint8_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (uint8_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator int8_t () const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (int8_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator uint16_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (uint16_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator int16_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (int16_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator uint32_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (uint32_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator int32_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (int32_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator uint64_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (uint64_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator int64_t() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (uint8_t)std::stod(_value);
    }
    return 0;
}

StringValue::operator float() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (float)std::stod(_value);
    }
    return 0;
}

StringValue::operator double() const
{
    if (std::regex_match(_value, NUMBER_REGEX))
    {
        return (double)std::stod(_value);
    }
    return 0;
}

StringValue::operator std::string() const
{
    return _value;
}

bool StringValue::assign(uint8_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(int8_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(uint16_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(int16_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(uint32_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(int32_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(uint64_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(int64_t newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(float newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(double newValue)
{
    std::ostringstream oss;
    oss << newValue;
    _value = std::move(oss.str());
    return true;
}

bool StringValue::assign(const std::string& newValue)
{
    _value = newValue;
    return true;
}

bool StringValue::assign(std::string&& newValue)
{
    _value = newValue;
    return true;
}

ByteBuffer& StringValue::serialize(ByteBuffer& byteBuffer) const
{
    byteBuffer.put(_value);
    return byteBuffer;
}


}
