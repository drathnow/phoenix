/** @file RTD.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 26, 2019 by daver:
 * @endhistory
 */
#ifndef RTD_H_
#define RTD_H_

#include "IHardwareIO.h"

namespace zios {
namespace hal {

class RTD : public IHardwareIO
{
public:
    RTD() : IHardwareIO(IHardwareIO::RTD_T)
    {
    }
    virtual ~RTD()
    {
    }

    /**
     * trigger conversion, returning RealWorld units as defined by slope/offset
     * @return RTD output in deg C. or 10000 on error
     */
    virtual double convertTemperature() = 0;

};

}
}  // namespace  zios

#endif /* RTD_H_ */
