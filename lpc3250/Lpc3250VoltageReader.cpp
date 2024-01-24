/** @file Lpc3250VoltageReader.cpp 
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

#include "Lpc3250VoltageReader.h"

#include <Exception.h>
#include <unistd.h>
#include <cassert>
#include <cstring>

#include "lpc3250.h"

namespace zios {
namespace lpc3250 {

using namespace zios::common;

Lpc3250VoltageReader::Lpc3250VoltageReader(struct iio_context* iioContext) :
        _iioContext(iioContext),
        _iioChannel(NULL)
{
    struct iio_device* device = ::iio_context_find_device(_iioContext, LPC3250_AI_DEVICE0);
    if (device == NULL)
        THROW_EXCEPTION("Power control device not found");

    if (NULL == (_iioChannel = ::iio_device_find_channel(device, LPC3250_VOLTAGE_CHANNEL, false)))
        THROW_EXCEPTION("Count not find voltage channel on device");

    if (0 !=::iio_channel_attr_read_double(_iioChannel, "scale", &_scale))
        THROW_EXCEPTION("Power control device not found");
}

Lpc3250VoltageReader::~Lpc3250VoltageReader()
{
    ::iio_context_destroy(_iioContext);
}

bool Lpc3250VoltageReader::systemVoltage(float& returnVoltage)
{
    //
    // The value coming from the AI is scaled but it must be multiplied by
    // a 12.5 resistor divider to get the correct voltage value, which is in
    // millivolts so we have to divide by 1000 to get voltage.
    //
    double value;
    if (0 != ::iio_channel_attr_read_double(_iioChannel, "raw", &value))
        return false;
    returnVoltage = (value * _scale * LPC3250_VOLTAGE_REFERNCE) / 1000.0F;
    return true;
}

} /* namespace lpc3250 */
} /* namespace zios */
