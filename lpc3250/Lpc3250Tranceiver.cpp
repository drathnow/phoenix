/** @file Lpc3250Tranceiver.cpp 
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
#include "lpc3250.h"
#include "Lpc3250Tranceiver.h"

#include <Exception.h>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <templates.h>
#include <cstring>
#include <cerrno>
#include <log4cIncludes.h>

namespace zios {
namespace lpc3250 {

using namespace log4cplus;
using namespace zios::common;

Lpc3250RS485Transceiver::Lpc3250RS485Transceiver(struct gpiod_line* trxLine, struct gpiod_line* rcvNLine) :
        _mode(TCVR_TX),
        _defaultMode(TCVR_ON)
{
    ::gpiod_line_bulk_init(&_chipLinesCtx);
    ::gpiod_line_bulk_add(&_chipLinesCtx, trxLine);
    ::gpiod_line_bulk_add(&_chipLinesCtx, rcvNLine);

    LPC3250_RS485_TURN_TRX_ON(_lineData)
    THROW_EXCEPTION_IF(0 != ::gpiod_line_request_bulk_output(&_chipLinesCtx, "zios", _lineData), "gpiod_line_request_bulk_output: " << ::strerror(errno));
}

Lpc3250RS485Transceiver::~Lpc3250RS485Transceiver()
{
}

void Lpc3250RS485Transceiver::setDefaultMode(zios::hal::ITransceiver::Mode mode)
{
    _defaultMode = mode;
}

zios::hal::ITransceiver::Mode Lpc3250RS485Transceiver::defaultMode()
{
    return _mode;
}

void Lpc3250RS485Transceiver::setTxMode(bool onOrOff)
{
    if (onOrOff)
    {
        _lineData[0] = 1;
        _lineData[1] = 1;
    }
    else
    {
        _lineData[0] = 0;
        _lineData[1] = 0;
    }

    int values[2];
    int status = ::gpiod_line_set_value_bulk(&_chipLinesCtx, _lineData);
    assert(status == 0);
    ::gpiod_line_get_value_bulk(&_chipLinesCtx, values);
    assert(status == 0);
}

void Lpc3250RS485Transceiver::resetToDefault()
{
    switch (_defaultMode)
    {
        case TCVR_ON:
            LPC3250_RS485_TURN_RCV_ON(_lineData)
            ;
            break;

        case TCVR_TX:
            LPC3250_RS485_TURN_TRX_ON(_lineData)
            ;
            break;

        default:
            case TCVR_OFF:
            LPC3250_TURN_RS485_OFF(_lineData)
            ;
            break;

    }
    int status = ::gpiod_line_set_value_bulk(&_chipLinesCtx, _lineData);
    assert(status != -1);
}

void Lpc3250RS485Transceiver::turnOff()
{
    LPC3250_TURN_RS485_OFF(_lineData);
    int status = ::gpiod_line_set_value_bulk(&_chipLinesCtx, _lineData);
    assert(status != -1);
}

Lpc3250RS232Transceiver::Lpc3250RS232Transceiver(struct gpiod_line* forceOnLine, struct gpiod_line* forceOffNLine) :
        _isOn(false)
{
    ::gpiod_line_bulk_init(&_bulkLineCtx);
    ::gpiod_line_bulk_add(&_bulkLineCtx, forceOnLine);
    ::gpiod_line_bulk_add(&_bulkLineCtx, forceOffNLine);

    //
    // Turn TX mode on
    //
    LPC3250_TURN_RS323_ON(_lineData)
    THROW_EXCEPTION_IF(0 != ::gpiod_line_request_bulk_output(&_bulkLineCtx, "zios", _lineData), "gpiod_line_request_bulk_output: " << ::strerror(errno));
}

Lpc3250RS232Transceiver::~Lpc3250RS232Transceiver()
{
}

void Lpc3250RS232Transceiver::setDefaultMode(zios::hal::ITransceiver::Mode mode)
{
}

zios::hal::ITransceiver::Mode Lpc3250RS232Transceiver::defaultMode()
{
    return ITransceiver::TCVR_ON;
}

void Lpc3250RS232Transceiver::setTxMode(bool onOrOff)
{
    // We will ignore this because currently, the RS232 transceiver stays on all
    // the time.
}

void Lpc3250RS232Transceiver::resetToDefault()
{
    // We will ignore this because currently, the RS232 transceiver stays on all
    // the time.
}

void Lpc3250RS232Transceiver::turnOff()
{
    LPC3250_TURN_RS323_OFF(_lineData);
    ::gpiod_line_set_value_bulk(&_bulkLineCtx, _lineData);
    _isOn = false;
}

} /* namespace hal */
} /* namespace zios */
