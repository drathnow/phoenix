/** @file Lpc3250ADC8344.h 
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
#ifndef LPC3250ADC8344_H_
#define LPC3250ADC8344_H_

#include <stdint.h>
#include <iio.h>

#include "lpc3250.h"

namespace zios {
namespace lpc3250 {

#define LPC3250_AI_DEVICE1                      "iio:device1"   //!< RTDs Input device containing LPS channel
#define LPC3250_AI_DEVICE3                      "iio:device3"   //!< Analog Input device containing LPS channel

#define LPC3250_LPF_ANALOG_CHANNEL_NAME         "voltage0"      //!< Filtered channel (CH1) from ADC 8344 on iio::device3
#define LPC3250_LPF_RTD_CHANNEL_NAME            "voltage3"      //!< Filtered channel (CH3) from ADC 8344 on iio::device1

/**
 * The LPC3250 board has a single ADS8344 chip.  The chip has 4 of channels: two analog going to analog
 * inputs and 2 going to RTDs. The input to these lines is selected with the LPC3250AnalogInputMux class.
 *
 * The kernel builders spread the 4 channels over 3 difference sysfs device (no explanation was ever given)
 * so reading them can ba bit confusing.
 *
 * Notes: This class is not thread safe.
 *
 */
class Lpc3250ADC8344
{
public:
    /**
     * Constructs and Lpc3250ADC8344 object.
     *
     * @param chip - The chip that controls the GPIO selector lines for the 8344.
     *
     * @param iioContext - The iio_context object.  This is a transfer of ownership.
     * This object will closed when this object is destroy.
     *
     * @throws Exception will be thrown if the the ADC8344 convertion line could not be selected.
     */
    Lpc3250ADC8344(struct iio_context* iioContext);
    ~Lpc3250ADC8344();

    /**
     * Reads a value from the ADS8344 filtered analog channel.
     *
     * @param retValue
     * @return 0 if the read went well, -1 if not. errno contains details
     */
    float scaledValueFromFilteredAnalogChannel();

    /**
     * Reads a value from the ADS8344 filtered RTD channel.
     *
     * @param retValue
     * @return 0 if the read went well, -1 if not. errno contains details
     */
    uint32_t rawValueFromFilteredRtdChannel();


    /**
     * Reads a value from the ADS8344 filtered analog channel.
     *
     * @param retValue
     * @return 0 if the read went well, -1 if not. errno contains details
     */
    float scaledValueFromAnalogChannel();

    float rawValueAnalogChannel();

    /**
     * Reads a value from the ADS8344 filtered RTD channel.
     *
     * @param retValue
     * @return 0 if the read went well, -1 if not. errno contains details
     */
    float rawValueFromRtdChannel();

    float scaledValueFromRtdChannel();
private:
    struct iio_context* _iioContext;
    struct iio_channel* _filteredAnalogChannel;
    struct iio_channel* _filteredRtdChannel;
    double _analogScale;
    double _rtdScale;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250ADC8344_H_ */
