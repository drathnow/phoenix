/** @file IAnalogIO.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb. 28, 2019 by daver:
 * @endhistory
 */
#ifndef IANALOGIO_H_
#define IANALOGIO_H_

#include "IHardwareIO.h"
#include <commonIncludes.h>

namespace zios {
namespace hal {

class IDigitalIO;

class IAnalogIO : public IHardwareIO
{
public:
    virtual ~IAnalogIO() {}

    /**
     * update internal scaling factor m in mx+b
     * @param[in] slope scaling factor
     */
    virtual void setSlope(float slope) = 0;

    /**
     * update internal offset factor b in mx+b
     * @param offset offset factor
     */
    virtual void setOffset(float offset) = 0;

    /**
     *
     * @return Pointer to an IDigitalIO. This is not a transfer of ownership. Callers should not hold
     * the refence beyond the scope of the point where the pointer is returned.
     */
    virtual IDigitalIO* getReferenceFor4to20() = 0;

    /**
     * trigger actual conversion, returning ADC units direct from ADC
     * @return 16 bit representation of % reference voltage
     */
    virtual int32_t convert() = 0;


protected:
    IAnalogIO(IHardwareIO::PORTTYPE portType) : IHardwareIO(portType) {}
};

} /* namespace hal */
} /* namespace zios */

#endif /* IANALOGIO_H_ */
