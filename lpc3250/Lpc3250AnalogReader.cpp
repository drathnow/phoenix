/** @file Lpc3250AnalogConverter.cpp 
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
#include "Lpc3250AnalogReader.h"

#include <cassert>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <log4cIncludes.h>
#include <Exception.h>
#include <iostream>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

Lpc3250AnalogReader::Lpc3250AnalogReader(gpiod_line* adcPowerLine, Lpc3250ADC8344 &adc8344, Lpc3250AnalogInputMux &analogInputMux) :
        _adc8344(adc8344),
        _analogInputMux(analogInputMux),
        _powerLine(adcPowerLine)
{
}

Lpc3250AnalogReader::~Lpc3250AnalogReader()
{
    delete &_adc8344;
    delete &_analogInputMux;
}

int Lpc3250AnalogReader::turnOn()
{
    assert(_powerLine != NULL);
    if (-1 == ::gpiod_line_set_value(_powerLine, 1))
        return -1;
    return 0;
}

int Lpc3250AnalogReader::turnOff()
{
    assert(_powerLine != NULL);
    if (-1 == ::gpiod_line_set_value(_powerLine, 0))
        return -1;
    return 0;
}

int Lpc3250AnalogReader::valueFromAdcChannel(float &retValue, int adcChannelNumber, int muxChannelNumber, int delayMicroSeconds)
{
    int retCode = 0;

    assert(adcChannelNumber >= 0 && adcChannelNumber <= 3);
    assert(muxChannelNumber >= 0 && muxChannelNumber <= 7);

    _mutex.lock();
    _analogInputMux.lock();

    if (0 == (retCode = _analogInputMux.selectChannel(muxChannelNumber)))
    {
        ::usleep(delayMicroSeconds);
        switch (adcChannelNumber)
        {
            case LPC3250_FILT_ANALOG_CHANNEL_NUMBER:
                retValue = _adc8344.scaledValueFromAnalogChannel();
                break;

            case LPC3250_FILT_RTD_CHANNEL_NUMBER:
                retValue = _adc8344.scaledValueFromRtdChannel();
                break;
        }
    }

    _analogInputMux.unlock();
    _mutex.unlock();

    return retCode;
}

int Lpc3250AnalogReader::voltageFromAdcChannel(double &retValue, int adcChannelNumber, int muxChannelNumber, int delayMicroSeconds)
{
    int retCode = 0;
    float value;

     if (0 == (retCode = valueFromAdcChannel(value, adcChannelNumber, muxChannelNumber, delayMicroSeconds )))
         retValue = value / 1000.0;

     return retCode;
}


} /* namespace lpc3250 */
} /* namespace zios */
