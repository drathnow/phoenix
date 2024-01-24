/** @file WakeButtonMonitor.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 9, 2019 by daver:
 * @endhistory
 */
#ifndef WAKEBUTTONLISTENER_H_
#define WAKEBUTTONLISTENER_H_

namespace zios {
namespace hal {

class WakeButtonListener
{
public:
    virtual ~WakeButtonListener()
    {
    }

    virtual void buttonPressed() = 0;
    virtual void buttonReleased() = 0;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* WAKEBUTTONLISTENER_H_ */
