/** @file SourceAddress.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 23, 2016 by daver:
 * @endhistory
 */
#include "SourceAddress.h"

#include <BitField.h>
#include <string>
#include <sstream>
#include <istream>
#include <ostream>
#include <locale>
#include <cstdio>
#include <StringHelper.h>
#include <ByteSwap.h>
#include <BitField.h>
#include <cstring>

namespace zios
{
namespace domain
{

using namespace std;
using namespace zios::foundation;

static const string DIGITAL_IN_PREFIX("DI.");
static const string DIGITAL_OUT_PREFIX("DO.");
static const string ANALOG_IN_PREFIX("AI.");
static const string RTD_PREFIX("RTD");
static const size_t PREFIX_LENGTH = 3;
static const char *UOM_INDICATOR = "uom=";
static const char *LOOP_INDICATOR = "loop";
static const char INVERTED_FLAG = '*';
static const char *EXTERNAL_PREFIX("EXT");
static const char *MODBUS_PREFIX("MBS");
static const char *ROC_PREFIX("ROC");
static const char *DELIMITER = ";";
static const char COLON_DELIMITER = ':';
static const char EQUAL_SIGN = '=';

static char* trimString(char *str)
{
    char *end;

    while (isspace((unsigned char) *str))
        str++;

    if (*str == 0)  // All spaces?
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;

    *(end + 1) = 0;
    return str;
}

static inline bool isInternalSensor(const char *prefix)
{
    return ::strstr(prefix, DIGITAL_IN_PREFIX.c_str()) == prefix || ::strstr(prefix, DIGITAL_OUT_PREFIX.c_str()) == prefix || ::strstr(prefix, ANALOG_IN_PREFIX.c_str()) == prefix || ::strstr(prefix, RTD_PREFIX.c_str()) == prefix;
}

InvalidSourceAddressException::InvalidSourceAddressException(const char *funcName, int lineNumber, const std::string &message) :
        Exception(funcName, lineNumber, "InvalidSourceAddressException", message)
{
}

InvalidSourceAddressException::~InvalidSourceAddressException() throw ()
{
}

const int ROCSourceAddress::EVENTS_TYPE_NUMBER = 378;
const int ROCSourceAddress::ALARMS_TYPE_NUMBER = 377;
const int ROCSourceAddress::HOURLY_HISTORY_TYPE_NUMBER = 376;
const int ROCSourceAddress::DAILY_HISTORY_TYPE_NUMBER = 375;

SourceAddress::SourceAddress(const SourceAddress &otherAddress) :
        _type(otherAddress._type)
{
}

SourceAddress::SourceAddress(Type type) :
        _type(type)
{
}

SourceAddress SourceAddress::operator=(const SourceAddress &otherAddress)
{
    _type = otherAddress._type;
    return *this;
}

SourceAddress::~SourceAddress()
{
}

bool SourceAddress::isInternal() const
{
    return InternalAddress == _type;
}

bool SourceAddress::isExternal() const
{
    return ExternalAddress == _type;
}

bool SourceAddress::isModbus() const
{
    return ModbusAddress == _type;
}

bool SourceAddress::isROC() const
{
    return ROCAddress == _type;
}

bool SourceAddress::isRtu() const
{
    switch (_type)
    {
    case ROCAddress:
    case ModbusAddress:
        return true;

    default:
        return false;
    }
}

bool InternalSourceAddress::isValidInternalSourceAddress(const std::string &addressString)
{
    return addressString.find(DIGITAL_IN_PREFIX) == 0 || addressString.find(DIGITAL_OUT_PREFIX) == 0 || addressString.find(ANALOG_IN_PREFIX) == 0 || addressString.find(RTD_PREFIX) == 0;
}

SourceAddress* SourceAddress::sourceAddressForString(const std::string &addressString)
{
    if (addressString.find(EXTERNAL_PREFIX) == 0)
        return ExternalSourceAddress::sourceAddressForString(addressString);
    else if (InternalSourceAddress::isValidInternalSourceAddress(addressString))
        return InternalSourceAddress::sourceAddressForString(addressString);
    else if (addressString.find_first_of(MODBUS_PREFIX) == 0)
        return ModbusSourceAddress::sourceAddressForString(addressString);
    else if (addressString.find_first_of(ROC_PREFIX) == 0)
        return ROCSourceAddress::sourceAddressForString(addressString);
    return NULL; // To keep the IDE happy!
}

InternalSourceAddress::InternalSourceAddress(const std::string &portName, const std::string &unitOfMeasure, bool isInverted, bool usingLoopPower) :
        SourceAddress(InternalAddress), _portName(portName), _unitOfMeasure(unitOfMeasure)
{
    _attributes.isInverted = isInverted;
    _attributes.isUsingLoopPower = usingLoopPower;
    _attributes.isDelayType = (0 == _portName.compare(0, 2, "AI")) || (0 == _portName.compare(0, 3, "RTD"));
}

InternalSourceAddress::InternalSourceAddress(const InternalSourceAddress &otherAddress) :
        SourceAddress(*this), _portName(otherAddress._portName), _unitOfMeasure(otherAddress._unitOfMeasure), _attributes(otherAddress._attributes)
{
}

InternalSourceAddress::~InternalSourceAddress()
{
}

bool InternalSourceAddress::isUsingLoopPower() const
{
    return _attributes.isUsingLoopPower == 1;
}

InternalSourceAddress InternalSourceAddress::operator=(const InternalSourceAddress &otherAddress)
{
    _portName = otherAddress._portName;
    _unitOfMeasure = otherAddress._unitOfMeasure;
    _attributes = otherAddress._attributes;
    return *this;
}

const std::string& InternalSourceAddress::portName() const
{
    return _portName;
}

const std::string& InternalSourceAddress::unitOfMeasure() const
{
    return _unitOfMeasure;
}

bool InternalSourceAddress::isInverted() const
{
    return _attributes.isInverted == 1;
}

bool InternalSourceAddress::isDelayType() const
{
    return _attributes.isDelayType == 1;
}

std::string InternalSourceAddress::toString() const
{
    string result(_portName);
    if (_attributes.isInverted)
        result.append("*");
    if (_unitOfMeasure.size() > 0)
    {
        result += COLON_DELIMITER;
        result.append(UOM_INDICATOR);
        result.append(_unitOfMeasure);
    }
    return result;
}

InternalSourceAddress* InternalSourceAddress::sourceAddressForString(const std::string &source)
{
    bool invert = false;
    bool loop = false;
    char prefix[PREFIX_LENGTH + 1];
    string portName;
    string uom;

    istringstream isstream(source);

    // test length.... sanity...
    if (source.length() < PREFIX_LENGTH)
        return NULL;

    ::memcpy(prefix, source.c_str(), PREFIX_LENGTH);
    prefix[PREFIX_LENGTH] = '\0';

    if (isInternalSensor(prefix))
    {
        char *ctx = NULL;
        char *next = NULL;
        char buf[source.length() + 1];
        char tokenBuf[source.length() + 1];

        ::strcpy(buf, source.c_str());
        if (NULL == (next = ::strtok_r(buf, DELIMITER, &ctx)))
            return NULL;

        char *pos = NULL;
        int len = ::strlen(next);
        if (NULL != (pos = ::strchr(next, INVERTED_FLAG)))
        {
            //
            // The invert symbol must be at the end of the string
            //
            if (pos != &next[len - 1])
                return NULL;

            invert = true;
            len = pos - next;
        }
        portName.assign(next, len);

        while (NULL != (next = ::strtok_r(NULL, DELIMITER, &ctx)))
        {
            trimString(next);

            //
            // This is either UOM, loop flag, or something bogus
            //
            ::strcpy(tokenBuf, next);
            if (::strstr(tokenBuf, UOM_INDICATOR) == tokenBuf)
            {
                const char *eqSign = ::strchr(tokenBuf, EQUAL_SIGN) + 1;
                if (*eqSign != '\0')
                {
                    uom.assign(eqSign);
                    StringHelper::trim(uom);
                    if (0 == uom.compare("-"))
                        uom = "";
                }
            } else if (::strstr(tokenBuf, LOOP_INDICATOR) == tokenBuf)
            {
                loop = true;
            } else
                return NULL;
        }
        return new InternalSourceAddress(portName, uom, invert, loop);
    }

    return NULL;
}

///////////////////// ExternalSourceAddress /////////////

ExternalSourceAddress::ExternalSourceAddress(const string &id, const string &handler, const string &protocol, const string &wellKnownPointName) :
        SourceAddress(ExternalAddress), _id(id), _handler(handler), _protocol(protocol), _wellKnownPointName(wellKnownPointName)
{
}

ExternalSourceAddress::ExternalSourceAddress(const char *id, const char *handler, const char *protocol, const char *wellKnownPointName) :
        SourceAddress(ExternalAddress), _id(id), _handler(handler), _protocol(protocol), _wellKnownPointName(wellKnownPointName)
{
}

ExternalSourceAddress::ExternalSourceAddress(const ExternalSourceAddress &otherAddress) :
        SourceAddress(otherAddress), _id(otherAddress._id), _handler(otherAddress._handler), _protocol(otherAddress._protocol), _wellKnownPointName(otherAddress._wellKnownPointName)
{
}

ExternalSourceAddress& ExternalSourceAddress::operator=(const ExternalSourceAddress &otherAddress)
{
    _id = otherAddress._id;
    _handler = otherAddress._handler;
    _protocol = otherAddress._protocol;
    _wellKnownPointName = otherAddress._wellKnownPointName;
    return *this;
}

ExternalSourceAddress::~ExternalSourceAddress()
{
}

const std::string& ExternalSourceAddress::id() const
{
    return _id;
}

const std::string& ExternalSourceAddress::handler() const
{
    return _handler;
}

const std::string& ExternalSourceAddress::protocol() const
{
    return _protocol;
}

const std::string& ExternalSourceAddress::wellKnownPointName() const
{
    return _wellKnownPointName;
}

void ExternalSourceAddress::makeExternalIoPointId(std::string &returnString) const
{
    returnString.clear();
    returnString.append(_id).append("/").append(_wellKnownPointName);
}

ExternalSourceAddress* ExternalSourceAddress::sourceAddressForString(const std::string &str)
{
    if (str.size() == 0)
        return NULL;

    string id;
    string protocol;
    string handler;
    string wkName;

    char *ctx = NULL;
    char *next = NULL;
    char buf[str.length() + 1];

    ::memcpy(buf, str.c_str(), str.length());
    buf[str.length()] = '\0';
    if (NULL == (next = ::strtok_r(buf, ";", &ctx)))
        return NULL;

    //
    // First valid the prefix is EXTernal
    //
    trimString(next);
    for (size_t i = 0; i < PREFIX_LENGTH; i++)
        next[i] = ::toupper(next[i]);

    if (::strncmp(next, EXTERNAL_PREFIX, PREFIX_LENGTH) != 0)
        return NULL;

    //
    // Next is the handler;
    //
    if (NULL == (next = ::strtok_r(NULL, ";", &ctx)))
        return NULL;
    trimString(next);
    for (size_t i = 0; i < PREFIX_LENGTH; i++)
        next[i] = ::toupper(next[i]);
    handler = next;

    //
    // Next the protocol
    //
    if (NULL == (next = ::strtok_r(NULL, ";", &ctx)))
        return NULL;

    trimString(next);
    for (size_t i = 0; i < PREFIX_LENGTH; i++)
        next[i] = ::toupper(next[i]);
    protocol = next;

    //
    // Next the ID
    //
    if (NULL == (next = ::strtok_r(NULL, ";", &ctx)))
        return NULL;
    trimString(next);
    id = next;

    //
    // And finally the well-known point name
    //
    if (NULL == (next = ::strtok_r(NULL, ";", &ctx)))
        return NULL;
    trimString(next);
    wkName = next;

    return new ExternalSourceAddress(id, handler, protocol, wkName);
}

ModbusSourceAddress::ModbusSourceAddress(uint16_t address, ByteSwap::BSBYTE_ORDER byteOrder, BitField *bitMask) :
        SourceAddress(ModbusAddress), _address(address), _byteOrder(byteOrder), _bitMask(bitMask)
{
}

ModbusSourceAddress::ModbusSourceAddress(const ModbusSourceAddress &otherAddress) :
        SourceAddress(otherAddress), _address(otherAddress._address), _byteOrder(otherAddress._byteOrder), _bitMask(NULL)
{
}

ModbusSourceAddress::~ModbusSourceAddress()
{
    delete _bitMask;
}

uint16_t ModbusSourceAddress::address() const
{
    return _address;
}

ByteSwap::BSBYTE_ORDER ModbusSourceAddress::byteOrder() const
{
    return _byteOrder;
}

BitField* ModbusSourceAddress::bitField() const
{
    return _bitMask;
}

uint16_t ModbusSourceAddress::adjustedValueAfterReading(uint16_t value) const
{
    return (NULL == _bitMask) ? value : (_bitMask->bitsValueFromValue(value) & 0x0000FFFF);
}

uint32_t ModbusSourceAddress::adjustedValue(uint32_t value) const
{
    if (ByteSwap::NONE == _byteOrder)
        return value;
    uint32_t myValue = value;
    ByteSwap::valueToByteSwapOrder(myValue, _byteOrder);
    return myValue;
}

ModbusSourceAddress& ModbusSourceAddress::operator=(const ModbusSourceAddress &otherAddress)
{
    _address = otherAddress._address;
    _byteOrder = otherAddress._byteOrder;
    return *this;
}

ModbusSourceAddress* ModbusSourceAddress::sourceAddressForString(const string &str)
{
    char *ctx = NULL;
    char buf[str.length() + 1];
    char *next = NULL;

    ::memcpy(buf, str.c_str(), str.length());
    buf[str.length()] = '\0';
    next = ::strtok_r(buf, ";", &ctx);
    if (NULL == next)
        return NULL;

    next = ::strtok_r(NULL, ";", &ctx);
    if (NULL == next)
        return NULL;

    //
    // Next should be the Modbus register address and, perhaps, a bit field.
    //
    unsigned int address;
    BitField *bitField = NULL;
    char bitString[32];
    int tokenCount = ::sscanf(next, "%u/%s", &address, bitString);
    if (tokenCount < 1)
        return NULL;

    if (tokenCount > 1)
    {
        if (NULL == (bitField = BitField::bitFieldForString(bitString)))
            return NULL;
    }

    //
    // Byte order is next, if provided
    //
    ByteSwap::BSBYTE_ORDER byteOrder = ByteSwap::NONE;
    if (NULL != (next = ::strtok_r(NULL, ";", &ctx)))
    {
        string oderName(next);
        if ((byteOrder = ByteSwap::byteOrderForName(oderName)) == ByteSwap::UNKNOWN)
            return NULL;
    }

    return new ModbusSourceAddress(address, byteOrder, bitField);
}

ROCSourceAddress::ROCSourceAddress(int type, int location, int parameter, zios::foundation::ByteSwap::BSBYTE_ORDER byteOrder, zios::foundation::BitField *bitMask) :
        SourceAddress(ROCAddress), _type(type), _location(location), _parameter(parameter), _addressType(REGISTER), _byteOrder(byteOrder), _bitMask(bitMask)
{

    switch (type)
    {
    case ALARMS_TYPE_NUMBER:
        _addressType = ALARMS;
        _type = 0xFF;
        break;

    case EVENTS_TYPE_NUMBER:
        _addressType = EVENTS;
        _type = 0xFF;
        break;

    case HOURLY_HISTORY_TYPE_NUMBER:
        _addressType = HOURLY_HISTORY;
        _type = 0xFF;
        break;

    case DAILY_HISTORY_TYPE_NUMBER:
        _addressType = DAILY_HISTORY;
        _type = 0xFF;
        break;

    default:
        break;
    }
}

ROCSourceAddress::~ROCSourceAddress()
{
    delete _bitMask;
}

uint8_t ROCSourceAddress::type() const
{
    return _type;
}

uint8_t ROCSourceAddress::location() const
{
    return _location;
}

uint8_t ROCSourceAddress::parameter() const
{
    return _parameter;
}

ROCSourceAddress::ROC_SRCADDR_TYPE ROCSourceAddress::addressType() const
{
    return (ROC_SRCADDR_TYPE) _addressType;
}

ByteSwap::BSBYTE_ORDER ROCSourceAddress::byteOrder() const
{
    return _byteOrder;
}

BitField* ROCSourceAddress::bitMask() const
{
    return _bitMask;
}

uint32_t ROCSourceAddress::adjustedValue(uint32_t value) const
{
    if (ByteSwap::NONE == _byteOrder)
        return value;
    uint32_t myValue = value;
    ByteSwap::valueToByteSwapOrder(myValue, _byteOrder);
    return myValue;
}

void ROCSourceAddress::toString(std::string &returnString)
{
    ostringstream oss;
    oss << "ROC;" << type() << "." << location() << "." << parameter();
}

ROCSourceAddress* ROCSourceAddress::sourceAddressForString(const std::string &str)
{
    char *ctx = NULL;
    char buf[str.length() + 1];
    char *next = NULL;

    ::memcpy(buf, str.c_str(), str.length());
    buf[str.length()] = '\0';
    next = ::strtok_r(buf, ";", &ctx);
    if (NULL == next)
        return NULL;

    //
    // Next will be the address, which has the form T.L.P[/x.x]
    //
    if (NULL == (next = ::strtok_r(NULL, ";", &ctx)))
        return NULL;

    unsigned int type;
    unsigned int location;
    unsigned int parameter;
    BitField *bitField = NULL;
    char bitString[32];
    ByteSwap::BSBYTE_ORDER byteOrder = ByteSwap::NONE;

    int addrCount = ::sscanf(next, "%u.%u.%u/%s", &type, &location, &parameter, bitString);
    if (addrCount < 3)
        return NULL;
    if (addrCount > 3)
    {
        bitField = BitField::bitFieldForString(bitString);
        if (NULL == bitField)
            return NULL;
    }

    //
    // Byte order is next, if provided
    //
    if (NULL != (next = ::strtok_r(NULL, ";", &ctx)))
    {
        string oderName(next);
        if ((byteOrder = ByteSwap::byteOrderForName(oderName)) == ByteSwap::UNKNOWN)
        {
            delete bitField;
            return NULL;
        }
    }
    return new ROCSourceAddress(type, location, parameter, byteOrder, bitField);
}

} /* namespace hardware */
} /* namespace zios */
