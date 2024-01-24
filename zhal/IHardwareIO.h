/** @file IHardwareIO.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb. 27, 2019 by daver:
 * @endhistory
 */
#ifndef IHARDWAREIO_H_
#define IHARDWAREIO_H_

#include <commonIncludes.h>
#include <stdint.h>

#define isDigitalIo(portType)       (portType == IHardwareIO::DI_T || portType == IHardwareIO::DO_T)
#define isAnalogIo(portType)        portType == IHardwareIO::AI_T

namespace zios {
namespace hal {

class IHardwareIO
{
public:
    enum DATATYPE
    {
        BOOL_T = 1,     // for like DI/ DO
        INT32_T,        // for pulse counters, or raw ADC
        FLOAT_T,        // for ADC
    };

    /**
     * Enumeration for type of HWPort exposed.
     */
    enum PORTTYPE
    {
        INVALID_T = 0,      //!< INVALID Type
        DI_T,               //!< Digital Input
        DO_T,               //!< Digital Ouput
        AI_T,               //!< Analog Input
        PI_T,               //!< Pulse Counter
        RTD_T,              //!< RTD object
        POW_T,              //!< Power object
    };

    virtual ~IHardwareIO() {}

    /**
     * Returns the type of this IHardwareIO
     */
    PORTTYPE type() const { return _type; }

    /**
     * return temporary reference for name of port
     * @return reference to name of port as a c string (null terminated).
     */
    virtual const char* name() const = 0;

    /**
     * return the label name of port that is stenciled on case
     * @return refefence to stencil name on case for port (null terminated)
     */
    virtual const char* label() const = 0;

    /**
     * This is a getter for integer type used for int8 and int32
     * If the output type is float, this will return 0.
     * @return integer32 value from the hardware pin
     */
    virtual uint32_t getInt() = 0;

    /**
     * This is a getter for floating point used for, well, floating point.
     * If the output type of hardware point is Int, then this returns 0.
     * @return floating value
     */
    virtual float getFloat() = 0;

    /**
     * gets the output type of the hardware point.
     * could be BOOL_T for a boolean type (or a unsigned 8 bit value)
     * or an INT32_t for a 32 bit integer , like a pulse counter
     * or a floating point value like for an adc
     * Note: should be thread safe....
     *
     * @retval BOOL_T  boolean(0/1)
     * @retval INT32_T 32 bit unsigned integer
     * @retval FLOAT_T floating point
     */
    virtual IHardwareIO::DATATYPE getResponseType() const = 0;

    /**
     * Returns current longtimestamp for value in hardware port
     *
     * @return current time of value
     */
    virtual long_timestamp_t timestamp() const = 0;

    /**
     * is this "port" readonly
     * @retval true if read only
     * @retval false if writable (DO only at moment)
     */
    virtual bool isReadOnly() const = 0;

    virtual bool writeBool(bool value) = 0;

protected:
    IHardwareIO(PORTTYPE type) : _type(type)
    {
    }

    PORTTYPE _type;
};

} /* namespace hal */
} /* namespace zios */

#endif /* IHARDWAREIO_H_ */
