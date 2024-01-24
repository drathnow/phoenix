/** @file Lpc3250WakeButtonMonitor.h 
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
#ifndef LPC3250WAKEBUTTONMONITOR_H_
#define LPC3250WAKEBUTTONMONITOR_H_

#include <Thread.h>
#include <Runnable.h>
#include <zhal.h>

namespace zios {
namespace lpc3250 {

class Lpc3250Chip;
class Lpc3250WakeButtonMonitor : public zios::common::Runnable
{
public:
    Lpc3250WakeButtonMonitor(zios::hal::WakeButtonListener* buttonListener);
    virtual ~Lpc3250WakeButtonMonitor();


    void run();
    int start();
    int stop();

    friend class Lpc3250HardwareManager;

private:
    zios::hal::WakeButtonListener* _buttonListener;
    bool _done;
    zios::common::Thread _thread;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250WAKEBUTTONMONITOR_H_ */
