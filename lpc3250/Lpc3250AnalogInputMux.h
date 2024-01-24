/** @file Lpc3250AnalogInputMultiplexer.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov. 15, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250ANALOGINPUTMUX_H_
#define LPC3250ANALOGINPUTMUX_H_

#include "lpc3250.h"
#include <Mutex.h>

namespace zios {
namespace lpc3250 {

/**
 * The Lpc3250AnalogInputMux represents an SN74LV4051APWR 8 line multiplexer that allows any
 * of the 8 channel to be selected.  There are a number of these units on the LPC3250 that
 * are all controlled by three select lines (see above).  The output from the mux goes typically
 * goes to an ADS8344 ADC.
 *
 * To use this class, clients must call lock() before calling select channel and then unlock when finshed.
 */
class Lpc3250AnalogInputMux
{
public:
    /**
     * Constructs an LPc3250 AnalogInputMux.
     *
     * @param selectorLine0 - The first selector line
     *
     * @param selectorLine1 - The second selector line
     *
     * @param selectorLine2 - The...wait for it...THIRD selector line
     */
    Lpc3250AnalogInputMux(gpiod_line* selectorLine0, gpiod_line* selectorLine1, gpiod_line* selectorLine2);
    ~Lpc3250AnalogInputMux();

    /**
     * Selects which channel is to be read on the multiplexer.  Before this method is called,
     * clients must call "lock()" to lock the class.  When finished, clients must call unlock().
     *
     * @param channelNumber
     * @return
     */
    int selectChannel(uint32_t channelNumber);

    void lock();
    void unlock();

private:
    struct gpiod_line_bulk _bulkOLines;
    zios::common::Mutex _lock;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250ANALOGINPUTMUX_H_ */
