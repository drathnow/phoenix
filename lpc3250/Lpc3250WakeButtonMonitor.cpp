/** @file Lpc3250WakeButtonMonitor.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 9, 2019 by daver:
 * @endhistory
 */
#include "Lpc3250WakeButtonMonitor.h"
#include "lpc3250.h"

#include <zhal.h>
#include <stdlib.h>
#include <gpiod.h>
#include <cerrno>
#include <cstring>
#include <log4cIncludes.h>

namespace zios {
namespace lpc3250 {

#define WAKE_BUTTON_CONSUMER    "lpc3250_wake"

using namespace log4cplus;
using namespace zios::hal;

enum STATE { WAITING_FOR_BUTTON_PRESS, WAITING_FOR_BUTTON_RELEASE };


Lpc3250WakeButtonMonitor::Lpc3250WakeButtonMonitor(zios::hal::WakeButtonListener* buttonListener) :
        _buttonListener(buttonListener),
        _done(true),
        _thread(*this, "Wake Button Monitor")
{
    assert(_buttonListener != NULL);
}

Lpc3250WakeButtonMonitor::~Lpc3250WakeButtonMonitor()
{
    if (_thread.isRunning()) {
        _done = true;
        _thread.join(1);
        delete _buttonListener;
    }
}

int Lpc3250WakeButtonMonitor::start()
{
    assert(_buttonListener != NULL);
    assert(_thread.isRunning() == false);
    return _thread.start();
}

int Lpc3250WakeButtonMonitor::stop()
{
    _done = true;
    _thread.join(1); // It shouldn't take more than a second for this thread to exit
    return 0;
}

void Lpc3250WakeButtonMonitor::run()
{
    int eventType = 0;
    STATE state = WAITING_FOR_BUTTON_PRESS;
    struct gpiod_chip* chip = NULL;
    struct gpiod_line* line = NULL;
    const struct timespec timeout = {1, 0};

    _done = true;

    if ((line = ::gpiod_line_find(LPC3250_WAKE_BUTTON_LINE_NAME)) == NULL)
        LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Unable to get line: " << ::strerror(errno));
    else if (0 != ::gpiod_line_request_falling_edge_events(line, WAKE_BUTTON_CONSUMER))
        LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Unable to request falling edge event: " << ::strerror(errno));
    else
        _done = false;

    if (_done)
        LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Lpc3250WakeButtonMonitor will not start");

    //
    // Enter the main loop. There is currently no way to break from gpiod_line_event_wait so we have to use
    // a timeout and check the _done flag each time through the loop.  This amounts to polling, which is not
    // very good, but it's all we have.
    //
    while (!_done && eventType >= 0) {

        //
        // Wait for something to happen
        //
        eventType = ::gpiod_line_event_wait(line, &timeout);

        //
        // Return code values are:
        // -1 - Error
        //  0 - Wait timed out
        //  1 - Event happened
        //
        switch (eventType) {
            case -1:
                LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"),  "Error during event wait - " << ::strerror(errno));
                _done = true;
                break;

            case 0 : // Nothing to do
                break;

            default:
            case 1 : {
                ::gpiod_line_release(line);
                switch (state) {
                    case WAITING_FOR_BUTTON_PRESS :
                        _buttonListener->buttonPressed();
                        if (0 != ::gpiod_line_request_rising_edge_events(line, WAKE_BUTTON_CONSUMER)) {
                            LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"),  "Unable to request rising edge - " << ::strerror(errno));
                            _done = true;
                        }
                        state = WAITING_FOR_BUTTON_RELEASE;
                        break;

                    case WAITING_FOR_BUTTON_RELEASE :
                        _buttonListener->buttonReleased();
                        if (0 != ::gpiod_line_request_falling_edge_events(line, WAKE_BUTTON_CONSUMER)) {
                            LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Unable to request falling edge - " << ::strerror(errno));
                            _done = true;
                        }
                        state = WAITING_FOR_BUTTON_PRESS;
                        break;
                }
            }
        }
    }

    if (chip != NULL)
        ::gpiod_chip_close(chip);
}

} /* namespace lpc3250 */
} /* namespace zios */
