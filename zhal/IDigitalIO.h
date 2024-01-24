/** @file IDigitalIO.h 
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
#ifndef IDIGITALIO_H_
#define IDIGITALIO_H_

#include "IHardwareIO.h"

namespace zios {
namespace hal {


class IDigitalIO : public IHardwareIO
{
public:
    IDigitalIO(IHardwareIO::PORTTYPE portType) : IHardwareIO(portType) {}
    virtual ~IDigitalIO() {}

    virtual IDigitalIO* getReferenceForScan() = 0;

    /**
     * force write of underlying port for this DO.
     */
    virtual void forceUpdate() = 0;

    /**
     * force read of underlying port for this DO
     */
    virtual void forceRead() = 0;

};


} /* namespace hal */
} /* namespace zios */

#endif /* IDIGITALIO_H_ */
