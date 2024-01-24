/** @file NotificationHandler.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 9, 2013 by daver:
 * @endhistory
 */

#ifndef NOTIFICATIONHANDLER_H_
#define NOTIFICATIONHANDLER_H_
#include <string>
#include "Notification.h"

namespace zios {
namespace common {

class NotificationHandler {
public:
	virtual ~NotificationHandler() {}

	/**
	 * Invoked by the NotificationCenter to handle a notification.
	 *
	 * @param notificationName - Reference to the name of the notification.  Since it is possible for a
	 * NotificationHandler to register for more than one notification, implemenations can use this
	 * argument to determine which notification is being delivered.
	 *
	 * @param notification - Pointer to an optional Notification object. This parameter is optional and
     * could be NULL depending on the notification.  Receivers should not hold references to this object as it
     * could be delete after the method returns. See details of the notificaitonName (where-ever it may be
     * documented ;) for details of the nofication object.
	 */
	virtual void handleNotification(const std::string& notificationName, Notification* notification) = 0;

	bool operator==(const NotificationHandler& otherHandler) const {
	    return this == &otherHandler;
	}
};

}
} /* namespace zios */
#endif /* NOTIFICATIONHANDLER_H_ */
