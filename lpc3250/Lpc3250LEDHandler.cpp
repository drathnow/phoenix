/** @file Lpc3250LEDHandler.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 22, 2019 by daver:
 * @endhistory
 */
#include "Lpc3250LEDHandler.h"
#include "Lpc3250LED.h"
#include <NotificationCenter.h>

namespace zios {
namespace lpc3250 {

using namespace zios::common;

Lpc3250LEDHandler::Lpc3250LEDHandler(zios::hal::LED &networkLed, zios::hal::LED &serverLed, zios::hal::LED &pollLed) :
        _networkLed(&networkLed),
        _serverLed(&serverLed),
        _pollLed(&pollLed)
{
}

Lpc3250LEDHandler::~Lpc3250LEDHandler()
{
    delete _networkLed;
    delete _serverLed;
    delete _pollLed;
}

void Lpc3250LEDHandler::handleNotification(const std::string& notificationName, zios::common::Notification* notification)
{
    if (notificationName == NotificationDescriptions::CONNECTION_CONNECTED_NOTIFICATION) {
        _serverLed->turnOn();
    } else if (notificationName == NotificationDescriptions::CONNECTION_LOST_NOTIFICATION || notificationName == NotificationDescriptions::CONNECTION_SHUTTING_DOWN) {
        _serverLed->turnOff();
    } else if (notificationName == NotificationDescriptions::TURN_ON_POLL_VOTE_LED) {
        _pollLed->turnOn();
    } else if (notificationName == NotificationDescriptions::TURN_OFF_POLL_VOTE_LED) {
        _pollLed->turnOff();
    } else if (notificationName == NotificationDescriptions::TURN_ON_NETWORK_LED) {
        _networkLed->turnOn();
    } else if (notificationName == NotificationDescriptions::TURN_OFF_NETWORK_LED) {
        _networkLed->turnOff();
    }
}

} /* namespace lpc3250 */
} /* namespace zios */
