/** @file Lpc3250AnalogConverter.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov. 13, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250ANALOGREADER_H_
#define LPC3250ANALOGREADER_H_

#include "lpc3250.h"
#include "Lpc3250ADC8344.h"
#include "Lpc3250AnalogInputMux.h"

#include <Mutex.h>

namespace zios {
namespace lpc3250 {

#define LPC3250_ANA_SAMPLE_DELAY_MICSECS        50000

/**
 * The Lpc3250AnalogReader is a mediator that orchastrates the process of reading a value from
 * the ADC8344 on the LPC3250 board. There is one ADC on the board, with four lines wired to a three
 * separate SN74LV4051APWR mutliplexers: one for the analog inputs and two for the RTDs. All the
 * multiplexers are controlled by 3 gpo selector lines so setting the values on these lines effects
 * all the muxes.
 *
 * This class is thread safe.
 *
 */
class Lpc3250AnalogReader
{
public:
    /**
     * Constructs an Lpc3250AnalogConverter.
     *
     * @param adc8344 - Reference to a single Lpc3250ADC8344.  This is a transfer of ownership.  This object
     * will be destroyed when this object is destroyed.
     *
     * @param analogInputMux - Reference to a single Lpc3250AnalogInputMux.  This is a transfer of ownership.  This object
     * will be destroyed when this object is destroyed.
     *
     * @param adcPowerLine - GPIO line controlling power to the ADC circuit.  It is assumed that this line has been
     * opened and initialized for output with a default value of zero (0).
     */
    Lpc3250AnalogReader(gpiod_line* adcPowerLine, Lpc3250ADC8344& adc8344, Lpc3250AnalogInputMux& analogInputMux);
    ~Lpc3250AnalogReader();

    /**
     * Reads a raw, unscaled value from one of the Analog Channels after selecting the line on the SN74LV4051APWR multiplexer and
     * returns that value.
     *
     * @param retValue - The return value.  This ADC8344 returns 16 bit values.
     *
     * @param adcChannelNumber - The channel on the ADC8344.  Currently, only channels 0-3 are supported.
     *
     * @param muxChannelNumber - The mux channel. Currently, only values 0-7 are supported.
     *
     * @param delayMicroSeconds - Microsecond delay between setting the mux channel and reading from the ADC8344
     *
     * @return 0 if all went well, -1 if the adcChannelNumber or muxChannelNumber channel values are invalid or an
     * error ocurrs reading from the ADC8344
     */
    int valueFromAdcChannel(float &retValue, int adcChannelNumber, int muxChannelNumber, int delayMicroSeconds = LPC3250_ANA_SAMPLE_DELAY_MICSECS);

    /**
     * Reads a raw value from one of the Analog Channels after selecting the line on the SN74LV4051APWR multiplexer and
     * applies a scaling calculation to return the voltage.
     *
     * @param retValue - The voltage reading from the ADC. This value is in volts
     *
     * @param adcChannelNumber - The channel on the ADC8344.  Currently, only channels 0-3 are supported.
     *
     * @param muxChannelNumber - The mux channel. Currently, only values 0-7 are supported.
     *
     * @param delayMicroSeconds - Microsecond delay between setting the mux channel and reading from the ADC8344
     *
     * @return 0 if all went well, -1 if the adcChannelNumber or muxChannelNumber channel values are invalid or an
     * error ocurrs reading from the ADC8344
     */
    int voltageFromAdcChannel(double &retValue, int adcChannelNumber, int muxChannelNumber, int delayMicroSeconds = LPC3250_ANA_SAMPLE_DELAY_MICSECS);

    /**
     * Turns on the Analog circuitry.
     *
     * @return 0 if it turn on.  -1 if not.  errno contains the reason.
     */
    int turnOn();

    /**
     * Turns off the Analog circuitry.
     *
     * @return 0 if it turn of.  -1 if not.  errno contains the reason.
     */
    int turnOff();

private:
    Lpc3250ADC8344& _adc8344;
    Lpc3250AnalogInputMux& _analogInputMux;
    gpiod_line* _powerLine;
    zios::common::Mutex _mutex;


};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250ANALOGREADER_H_ */
