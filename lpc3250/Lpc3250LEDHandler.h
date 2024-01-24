/** @file Lpc3250LEDHandler.h
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
#ifndef LPC3250LEDHANDLER_H_
#define LPC3250LEDHANDLER_H_

#include <Notification.h>
#include <NotificationHandler.h>
#include <string>

namespace zios {
namespace hal {
    class LED;
}
namespace lpc3250 {

class Lpc3250LEDHandler : public zios::common::NotificationHandler
{
public:
    Lpc3250LEDHandler(zios::hal::LED& networkLed, zios::hal::LED& serverLed, zios::hal::LED& pollLed);
    virtual ~Lpc3250LEDHandler();

    void handleNotification(const std::string& notificationName, zios::common::Notification* notification);

private:
    zios::hal::LED* _networkLed;
    zios::hal::LED* _serverLed;
    zios::hal::LED* _pollLed;

    Lpc3250LEDHandler(const Lpc3250LEDHandler& otherHandler);
    Lpc3250LEDHandler& operator=(const Lpc3250LEDHandler& otherHandler);
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250LEDHANDLER_H_ */
