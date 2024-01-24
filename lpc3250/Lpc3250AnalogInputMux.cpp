/** @file Lpc3250AnalogInputMultiplexer.cpp 
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

#include "Lpc3250AnalogInputMux.h"

#include <gpiod.h>
#include <cstring>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <Exception.h>
#include <templates.h>
#include <Mutex.h>
#include <iostream>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::common;

Lpc3250AnalogInputMux::Lpc3250AnalogInputMux(gpiod_line* selectorLine0, gpiod_line* selectorLine1, gpiod_line* selectorLine2)
{
    int channelValues[3] = {0, 0, 0};
    ::memset(&_bulkOLines, 0, sizeof(struct gpiod_line_bulk));
    ::gpiod_line_bulk_init(&_bulkOLines);
    ::gpiod_line_bulk_add(&_bulkOLines, selectorLine0);
    ::gpiod_line_bulk_add(&_bulkOLines, selectorLine1);
    ::gpiod_line_bulk_add(&_bulkOLines, selectorLine2);
    ::gpiod_line_request_bulk_output(&_bulkOLines, "zios", channelValues);
}

Lpc3250AnalogInputMux::~Lpc3250AnalogInputMux()
{
}

int Lpc3250AnalogInputMux::selectChannel(uint32_t channelNumber)
{
    assert(channelNumber < 8);
    int channelValues[3];
    channelValues[0] = channelNumber & 0x01;
    channelValues[1] = (channelNumber & 0x02) >> 1;
    channelValues[2] = (channelNumber & 0x04) >> 2;
    return ::gpiod_line_set_value_bulk(&_bulkOLines, channelValues);
}

void Lpc3250AnalogInputMux::lock()
{
    _lock.lock();
}

void Lpc3250AnalogInputMux::unlock()
{
    _lock.unlock();
}

} /* namespace lpc3250 */
} /* namespace zios */
