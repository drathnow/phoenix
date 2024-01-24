/** @file ITransceiver.h 
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
#ifndef ITRANSCEIVER_H_
#define ITRANSCEIVER_H_

namespace zios {
namespace hal {

class ITransceiver {
public:
    enum Mode {
        TCVR_OFF = 0,
        TCVR_ON = 1,                // like RX on.....
        TCVR_TX = 2,
        TCVR_AUTO = 3,              // will power on on receive and then power off.
        TCVR_DEFAULT = 255,
    };

    virtual ~ITransceiver() {}

    /**
     * set the default mode of the transceiver
     * @param[in] mode for default
     */
    virtual void setDefaultMode(Mode mode) = 0;

    /**
     * get the default mode of transceiver
     * @return default mode of transceiver
     */
    virtual Mode defaultMode() = 0;

    /**
     * set the transceiver transmit mode on or off
     * @param[in] on - true to set mode on, false for off
     */
    virtual void setTxMode(bool on) = 0;

    /**
     * return transceiver to its default (idle) mode.
     */
    virtual void resetToDefault() = 0;

    /**
     * turn off the transceiver
     */
    virtual void turnOff() = 0;
};

} /* namespace hal */
} /* namespace zios */

#endif /* ITRANSCEIVER_H_ */
