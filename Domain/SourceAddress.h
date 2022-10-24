/** @file SourceAddress.h
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
#ifndef SOURCEADDRESS_H_
#define SOURCEADDRESS_H_

#include <except.h>
#include <bitset>
#include <string>
#include <ByteSwap.h>
#include <stdint.h>
#include <BitField.h>

using namespace zios::foundation;

namespace zios::domain
{

class InvalidSourceAddressException: public zios::foundation::Exception
{
public:
    InvalidSourceAddressException(const char *funcName, int lineNumber, const std::string &message);
    ~InvalidSourceAddressException() throw ();
};

class SourceAddress
{
public:
    SourceAddress(const SourceAddress &otherAddress);
    virtual ~SourceAddress();

    enum Type
    {
        InternalAddress, ExternalAddress, ModbusAddress, ROCAddress
    };

    /**
     * Indicates if the source address is an Internal Address
     *
     * @return true if address is internal, false if not
     */
    bool isInternal() const;

    /**
     * Indicates if the source address is an External Address
     *
     * @return true if address is External, false if not
     */
    bool isExternal() const;

    /**
     * Indicates if the source address is an MODBUS Address
     *
     * @return true if address is RTU, false if not
     */
    bool isModbus() const;

    /**
     * Indicates if the address is a ROC address
     *
     * @return
     */
    bool isROC() const;

    /**
     * Indicates if the source address is an RTU source address
     *
     * @return
     */
    bool isRtu() const;

    SourceAddress operator=(const SourceAddress &otherAddress);

    /**
     *
     * @param addressString - An address string that is either an internal or external
     * address string.
     *
     * @return SourceAddress* - Pointer to a heap allocated SourceAddress string.  Use the
     * type() method to detemrine what type of object has been returned.  This is a transfer
     * of ownership. NULL if the source address could not be translated
     *
     */
    static SourceAddress* sourceAddressForString(const std::string &addressString);

protected:
    SourceAddress(Type type);

private:
    Type _type;
};

typedef struct
{
    unsigned isInverted :1;
    unsigned isUsingLoopPower :1;
    unsigned isDelayType :1;
} attributes_t;

class InternalSourceAddress: public SourceAddress
{
public:
    InternalSourceAddress(const std::string &portName, const std::string &unitOfMeasure, bool isInverted, bool usingLoopPower = true);
    InternalSourceAddress(const InternalSourceAddress &otherAddress);
    ~InternalSourceAddress();

    const std::string& portName() const;
    const std::string& unitOfMeasure() const;
    bool isInverted() const;
    bool isUsingLoopPower() const;
    bool isDelayType() const;

    InternalSourceAddress operator=(const InternalSourceAddress &otherAddress);

    std::string toString() const;

    /**
     *
     * @param str - An Internal Address string.
     *
     * @return InternalAddressString* - point to a heap allocated. This is a transfer of
     * ownership.
     *
     */
    static InternalSourceAddress* sourceAddressForString(const std::string &str);

    static bool isValidInternalSourceAddress(const std::string &addressString);

private:
    std::string _portName;
    std::string _unitOfMeasure;
    attributes_t _attributes;
};

class ExternalSourceAddress: public SourceAddress
{
public:
    ExternalSourceAddress(const ExternalSourceAddress &otherAddress);
    ~ExternalSourceAddress();

    /**
     * Returns the external Id part of the source address
     *
     * @return string reference
     */
    const std::string& id() const;

    /**
     * Returns handler portion of the address
     *
     * @return string reference
     */
    const std::string& handler() const;

    /**
     * Returns protocol portion of the address
     *
     * @return string reference
     */
    const std::string& protocol() const;

    /**
     * Returns well known point name of the address
     *
     * @return
     */
    const std::string& wellKnownPointName() const;

    /**
     * Will build an external identifying name for an IO id.
     *
     * @param returnStrring - reference to a string object to receive the
     * constructed id
     */
    void makeExternalIoPointId(std::string &returnString) const;

    ExternalSourceAddress& operator=(const ExternalSourceAddress &otherAddress);

    /**
     * Builds an ExternalSourceAddress from a string.  The format is exptected to be:
     *
     *      EXT;<hdlr>.<prot>;id/<wkName>
     *
     *      where hdlr   - is the handler that defines a handler to retrieve and send commands
     *            prot   - is the protocol used to decode/encode messages
     *            id     - is the identifier for the device
     *            wkName - is the well known point name
     *
     * @param str - the formatted string
     *
     * @return ExternalSourceAddress* - point to a heap allocated ExternalSourceAddress.  This is a
     * a transfer of ownership.
     *
     * @throws IllegalArgumentException if the string does not have the correct format
     */
    static ExternalSourceAddress* sourceAddressForString(const std::string &str);

private:
    ExternalSourceAddress(const std::string &id, const std::string &handler, const std::string &protocol, const std::string &wellKnownPointName);
    ExternalSourceAddress(const char *id, const char *handler, const char *protocol, const char *wellKnownPointName);

    std::string _id;
    std::string _handler;
    std::string _protocol;
    std::string _wellKnownPointName;
};

/**
 * The ModbusSourceAddress contains information required to poll
 */
class ModbusSourceAddress: public SourceAddress
{
public:
    /**
     *
     * @param address
     * @param byteOrder
     *
     * @param bitMask - Optional BitField specifier. If a BitField is specified, byteOrder is ignored.
     * If you use BitField, set byteOrder to ByteSwap::NONE. This is a transfer of ownership.
     *
     */
    ModbusSourceAddress(uint16_t address, zios::foundation::ByteSwap::BSBYTE_ORDER byteOrder, zios::foundation::BitField *bitMask = NULL);
    ModbusSourceAddress(const ModbusSourceAddress &otherAddress);
    ~ModbusSourceAddress();

    ModbusSourceAddress& operator=(const ModbusSourceAddress &otherAddress);

    uint16_t address() const;

    zios::foundation::ByteSwap::BSBYTE_ORDER byteOrder() const;

    zios::foundation::BitField* bitField() const;

    uint16_t adjustedValueAfterReading(uint16_t value) const;

    uint32_t adjustedValue(uint32_t value) const;

    /**
     * Builds an ExternalSourceAddress from a string.  The format is exptected to be:
     *
     *      MODBUS;<address>;<byte-swap>
     *
     *      where address   - is the register address of the value to be read
     *            byte-swap - is an optoinal value for the byte swapping.  Valid values are
     *                        NONE - No byte swapping
     *                        1234 - Little Endian
     *                        4321 - BigEndian
     *                        2143 - Word swapp
     *                        3412 - Byte swap
     *
     *
     * @param str - the formatted string
     *
     * @return RtuSourceAddress* - point to a heap allocated ExternalSourceAddress.  This is a
     * a transfer of ownership.
     *
     * @throws IllegalArgumentException if the string does not have the correct format
     */
    static ModbusSourceAddress* sourceAddressForString(const std::string &str);

private:
    uint16_t _address;
    zios::foundation::ByteSwap::BSBYTE_ORDER _byteOrder;
    zios::foundation::BitField *_bitMask;
};

class ROCSourceAddress: public SourceAddress
{
public:
    static const int ALARMS_TYPE_NUMBER;
    static const int EVENTS_TYPE_NUMBER;
    static const int HOURLY_HISTORY_TYPE_NUMBER;
    static const int DAILY_HISTORY_TYPE_NUMBER;

    enum ROC_SRCADDR_TYPE
    {
        REGISTER, HOURLY_HISTORY, DAILY_HISTORY, ALARMS, EVENTS, NONE
    };

    ROCSourceAddress(int type, int location, int parameter, zios::foundation::ByteSwap::BSBYTE_ORDER byteOrder = zios::foundation::ByteSwap::NONE, zios::foundation::BitField *bitMask = NULL);
    ~ROCSourceAddress();

    uint8_t type() const;
    uint8_t location() const;
    uint8_t parameter() const;

    ROC_SRCADDR_TYPE addressType() const;

    zios::foundation::ByteSwap::BSBYTE_ORDER byteOrder() const;
    zios::foundation::BitField* bitMask() const;
    uint32_t adjustedValue(uint32_t value) const;
    void toString(std::string &returnString);

    static ROCSourceAddress* sourceAddressForString(const std::string &str);

private:
    uint8_t _type;
    uint8_t _location;
    uint8_t _parameter;
    ROC_SRCADDR_TYPE _addressType;
    zios::foundation::ByteSwap::BSBYTE_ORDER _byteOrder;
    zios::foundation::BitField *_bitMask;

};

} /* namespace zios */

#endif /* SOURCEADDRESS_H_ */
