/** @file Lpc3250Tranceiver.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 1, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250TRANCEIVER_H_
#define LPC3250TRANCEIVER_H_

#include <zhal.h>
#include <stdint.h>

#define LPC3250_RS485_TRX_ON                    1
#define LPC3250_RS485_TRX_OFF                   0
#define LPC3250_RS485_RCV_ON                    0
#define LPC3250_RS485_RCV_OFF                   1

#define LPC3250_RS323_FORCEON_ON                1
#define LPC3250_RS323_FORCEOFF_N_ON             1
#define LPC3250_RS323_FORCEOFF_N_OFF            0

#define LPC3250_RS485_TURN_TRX_ON(lineData)     lineData[0] = LPC3250_RS485_TRX_ON; lineData[1] = LPC3250_RS485_RCV_OFF;
#define LPC3250_RS485_TURN_RCV_ON(lineData)     lineData[0] = LPC3250_RS485_TRX_OFF; lineData[1] = LPC3250_RS485_RCV_ON;
#define LPC3250_TURN_RS485_OFF(lineData)        lineData[0] = LPC3250_RS485_TRX_OFF; lineData[1] = LPC3250_RS485_RCV_OFF;

#define LPC3250_TURN_RS323_ON(lineData)         lineData[0] = LPC3250_RS323_FORCEON_ON; lineData[1] = LPC3250_RS323_FORCEOFF_N_ON;
#define LPC3250_TURN_RS323_OFF(lineData)        lineData[0] = LPC3250_RS323_FORCEON_ON; lineData[1] = LPC3250_RS323_FORCEOFF_N_OFF;

namespace zios {
namespace lpc3250 {


/**
 * The Lpc3250RS485Tranceiver controls the transmit/receive functionality of the RS485 port
 * on the LPC3250 board. Two GPO lines control these functions:
 *
 *               Rcv   Trx
 *     +-------+-----+-----+
 *     | GPO18 |  0  |  1  |
 *     +-------+-----+-----+
 *     | GPO19 |  1  |  0  |
 *     +-------+-----+-----+
 *
 *  The class can be used to
 *
 */
class Lpc3250RS485Transceiver : public zios::hal::ITransceiver
{
public:
    Lpc3250RS485Transceiver(struct gpiod_line* trxLine, struct gpiod_line* rcvNLine);
    ~Lpc3250RS485Transceiver();


    /**
     * set the default mode of the transceiver
     * @param[in] mode for default
     */
    void setDefaultMode(zios::hal::ITransceiver::Mode mode);

    /**
     * get the default mode of transceiver
     * @return default mode of transceiver
     */
    zios::hal::ITransceiver::Mode defaultMode();

    /**
     * Turns the transceiver transmit mode on or off
     *
     * @param[in] on - true to set mode on, false for off
     */
    void setTxMode(bool onOrOff);

    /**
     * return transceiver to its default (idle) mode.
     */
    void resetToDefault();

    /**
     * turn off the transceiver
     */
    void turnOff();

private:
    zios::hal::ITransceiver::Mode _mode;
    gpiod_line_bulk _chipLinesCtx;
    zios::hal::ITransceiver::Mode _defaultMode;
    int32_t _lineData[2];
};

/**
 * The Lpc3250RS323Tranceiver controls the RS323 port(s) on the LPC3250.  It is somewhat simpler
 * than the 485 transceiver in that it only turns the ports(s) off and on.  Switching between transmit
 * and receive is done in the kernel driver.  Under normal conditions, FORCEON_LINE (GPO15) and
 * FORCEOFF_N (GPO14) both be set high. If the port(s) are not in use, they can be turned off by forcing
 * FORCEOFF_N low.
 *
 * The RS323 ports are controled by a TRS3243E Multichannel Line Driver/Receiver. From the data sheet, the lines
 * are
 *
 *      FORCEON  FORCEOFF_N
 *     +--------+-----------+------------------------------------------------+
 *     |   1    |      1    | Normal operation with auto-power down DISABLED |
 *     +--------+-----------+------------------------------------------------+
 *     |   0    |      1    | Normal operation with auto-power down ENABLED  |
 *     +--------+-----------+------------------------------------------------+
 *
 * The default is to leave the RS232 powered on so we only run in the first mode (1,1)
 *
 * While it is possible to turn the RS232 port off to save power, it currently only support
 * ON mode where it is powered on all the time.
 */
class Lpc3250RS232Transceiver : public zios::hal::ITransceiver
{
public:
    Lpc3250RS232Transceiver(struct gpiod_line* forceOnLine, struct gpiod_line* forceOffNLine);
    ~Lpc3250RS232Transceiver();


    /**
     * set the default mode of the transceiver
     * @param[in] mode for default
     */
    void setDefaultMode(zios::hal::ITransceiver::Mode mode);

    /**
     * get the default mode of transceiver
     * @return default mode of transceiver
     */
    zios::hal::ITransceiver::Mode defaultMode();

    /**
     * The RS232 port does not support this function. The port is always in Transmit mode
     * so there is no need to call this.
     *
     * @param[in] on - true to set mode on, false for off
     */
    void setTxMode(bool on);

    /**
     * Return transceiver to its default (idle) mode, which is on.
     */
    void resetToDefault();

    /**
     * Turns off the RS232 ports.
     */
    void turnOff();

private:
    struct gpiod_line_bulk _bulkLineCtx;
    bool _isOn;
    int32_t _lineData[2];
};

} /* namespace hal */
} /* namespace zios */

#endif /* LPC3250TRANCEIVER_H_ */
