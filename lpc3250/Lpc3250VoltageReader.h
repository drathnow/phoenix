/** @file Lpc3250VoltageReader.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 3, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250VOLTAGEREADER_H_
#define LPC3250VOLTAGEREADER_H_

#include <iio.h>

#define LPC3250_AI_DEVICE0                      "iio:device0"
#define LPC3250_VOLTAGE_CHANNEL                 "voltage0"
#define LPC3250_VOLTAGE_REFERNCE                12.5F

namespace zios {
namespace lpc3250 {


/**
 * Class to read the voltage from the LPC3250 board. According to the schematics, the voltage is
 * measured by the ADC and multiplied by a resistor divider of 12.5 (1/0.08).
 */
class Lpc3250VoltageReader
{
public:
    /**
     * Constructs a Lpc3250VoltageReader that will read voltage from the main boards ADC.
     *
     * @param iioContext - This is a transfer of ownership, it will be close when this object
     * is destroyed.
     */
    Lpc3250VoltageReader(struct iio_context* iioContext);
    ~Lpc3250VoltageReader();

    /**
     * Retrieves the system voltage
     *
     * @param returnVoltage - Reference to var where voltage is returnes.
     *
     * @return true if the voltage could be read.  False otherwise.
     */
    bool systemVoltage(float& returnVoltage);

private:
    struct iio_context* _iioContext;
    struct iio_channel* _iioChannel;
    double _scale;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250VOLTAGEREADER_H_ */
