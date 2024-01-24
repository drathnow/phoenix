/** @file Lpc3250ADC8344.cpp 
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
#include "Lpc3250ADC8344.h"

#include <Exception.h>
#include <cassert>
#include <iostream>
#include <cstring>
#include <log4cIncludes.h>
#include <unistd.h>
#include <fcntl.h>

#include <iio.h>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

Lpc3250ADC8344::Lpc3250ADC8344(struct iio_context *iioContext) :
        _iioContext(iioContext)
{
    struct iio_device* rtdDevice = ::iio_context_find_device(_iioContext, LPC3250_AI_DEVICE1);
    THROW_EXCEPTION_IF(NULL == rtdDevice, "Aanalog input device " << LPC3250_AI_DEVICE1 << "not found");

    struct iio_device* analogInputDevice = ::iio_context_find_device(_iioContext, LPC3250_AI_DEVICE3);
    THROW_EXCEPTION_IF(NULL == analogInputDevice, "Aanalog input device " << LPC3250_AI_DEVICE3 << "not found");

    _filteredAnalogChannel = ::iio_device_find_channel(analogInputDevice, LPC3250_LPF_ANALOG_CHANNEL_NAME, false);
    THROW_EXCEPTION_IF(NULL == _filteredAnalogChannel, "Can't find filterd channel " << LPC3250_LPF_ANALOG_CHANNEL_NAME);

    _filteredRtdChannel = ::iio_device_find_channel(rtdDevice, LPC3250_LPF_RTD_CHANNEL_NAME, false);
    THROW_EXCEPTION_IF(_filteredRtdChannel == NULL, "Can't find filterd channel " << LPC3250_LPF_RTD_CHANNEL_NAME);

    ::iio_channel_attr_read_double(_filteredAnalogChannel, "scale", &_analogScale);
    ::iio_channel_attr_read_double(_filteredRtdChannel, "scale", &_rtdScale);
}

Lpc3250ADC8344::~Lpc3250ADC8344()
{
    ::iio_context_destroy(_iioContext);
}

static float avg_value_from_channel(struct iio_channel* channel, int readingCount)
{
    uint32_t sum = 0;
    long long retValue;

    for (int i = 0; i < readingCount; i++)
    {
        ::iio_channel_attr_read_longlong(channel, "raw", &retValue);
        sum += retValue;
    }

    return (sum/readingCount);
}

float Lpc3250ADC8344::rawValueAnalogChannel()
{
    return avg_value_from_channel(_filteredAnalogChannel, 16);
}

float Lpc3250ADC8344::scaledValueFromAnalogChannel()
{
    return rawValueAnalogChannel() * _analogScale;
}

float Lpc3250ADC8344::rawValueFromRtdChannel()
{
    return avg_value_from_channel(_filteredRtdChannel, 16);
}

float Lpc3250ADC8344::scaledValueFromRtdChannel()
{
    return rawValueFromRtdChannel() * _rtdScale;
}

} /* namespace lpc3250 */
} /* namespace zios */
