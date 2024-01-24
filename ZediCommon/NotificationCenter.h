/** @file NotificationCenter.h
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
 * IncrDev May 27, 2015 by eb: added notification Description class to hold notification names
 * IncrDev Jun 03, 2015 by eb: change CONNECTION_NOTIFICATION to NotificationCenter.h
 * IncrDev Jul 15, 2015 by eb: due to strangeness? in ARM GCC optimizer, changed const string to const char *
 * @endhistory
 */

#ifndef NOTIFICATIONCENTER_H_
#define NOTIFICATIONCENTER_H_

#include <map>
#include <set>
#include <string>

#include "NotificationHandler.h"
#include "ReadWriteLock.h"

namespace zios {
namespace common {

class NotificationHandler;
class Notification;

class NotificationDescriptions {
public:

    /**
     * Notification name that is broadcast to indicate the TCP port mapping to some port has changed.
     * Allows Passthrough to re-initialize its mapping
     */
    static const char* TCP_MAPPED_PORT_CHANGE_NOTIFICATION;

    /**
     * Notification name that is broadcast to indicate that the Server Name has been updated (possibly to same value)
     */
    static const char* NET_SERVER_NAME_UPDATE_NOTIFICATION;

    /**
     * Notification name that is broadcast to indicate that the Network Manager status has been updated (possibly to same value)
     */
    static const char* NET_STATUS_UPDATE_NOTIFICATION;

    /**
     * Notification name that is broadcast whenever an abrupt change is made to the
     * system time.
     */
    static const char* TIME_CHANGED_NOTIFICATION;

    /**
     * Notification name that is broadcast whenever zios starts ok after OTAD.
     */
    static const char* ZIOS_CHECK_NOTIFICATION;

    /**
     * Sent to indicate the connection is about to be shutdown.  Receivers must halt
     * all operations that might initiate activity over the current connection.  Any
     * activity that is currently in progress might be allowed to complete, depending
     * on the way the connector is stopped.  See the stop() and disconnect() methods.
     */
    static const char* CONNECTION_SHUTTING_DOWN;

    /**
     *  Notification event sent if connection to the server is lost abnormally.  This notification
     *  differs from CONNECTION_SHUTTING_DOWN in that it indicates an abrupt communications failure.
     *  Receivers of this notification should immediately stop activities and take necessary actions
     *  to cleanup and wait for the connection to reconnect.
     */
    static const char* CONNECTION_LOST_NOTIFICATION;

    /**
     * Notification event sent when the connector connects to the remote server and is read
     * to start processing incoming and outgoing messages.
     */
    static const char* CONNECTION_CONNECTED_NOTIFICATION;

    /**
     * Notification posted by the NetworkAdapter when an initial socket connection is successfully made.
     * There is no Notification object associated with this notification.
     */
    static const char* NETWORK_ADAPTER_CONNECTED;

    /**
     * Notification posted by the NetworkAdapter a socket is disconnected
     * There is no Notification object associated with this notification.
     */
    static const char* NETWORK_ADAPTER_DISCONNECTED;

    /**
     * Turns on the POLL indicator
     */
    static const char* TURN_ON_POLL_VOTE_LED;

    /**
     * Turns off the POLL indicator
     */
    static const char* TURN_OFF_POLL_VOTE_LED;

    /**
     * Turn on the Network LED
     */
    static const char* TURN_ON_NETWORK_LED;

    /**
     * Turn OFF the Network LED
     */
    static const char* TURN_OFF_NETWORK_LED;

    /**
     * Sent when OTAD starts
     */
    static const char* OTAD_STARTING;

    /**
     * Sent when OTAD fails.
     */
    static const char* OTAD_FAILED;

    /**
     * Notification broadcast when an extern value has been read an needs to be
     * processed into ZIOS.  The notification will be accompanied by a Notification object
     * containing a 'tag' object that is a pointer to an ExternalReportValue object.
     * See documentation for the ExternalReportValue for lifetime scope rules.
     *
     */
    static const char* EXTERNAL_VALUE_RECEIVED_NOTIFICATION_NAME;

    /**
     * Notification broadcast when an application IO point is written. The notification will
     * contain be accompanied by a Notificaiton object containing a 'tag' object that is the pointer
     * to the IOPoint object.  The notification will contain a string property key called 'value' whose
     * value is a string representation of the value to be written.  Handlers of this notification
     * must NOT hold onto the IO point after returning from the handler routine.
     */
    static const char* APP_IOPORT_WRITE_NOTIFICATION_NAME;
    static const char* VALUE_PROPERTY_NAME;

    /**
     * Boadcast when a configuration point is updated.  The Notification object's tag property
     * will contain a point to this configuration point that was updated.  Note that you should
     * never hold the reference to the point beyond the scope of your notification handler.
     *
     * NOTE: This notification is recieved with the IO Point Managers main lock held.  For that
     * reason, handlers must not take out any other locks and finish as quickly as possible.
     */
    static const char* CFG_POINT_UPDATED;
};

typedef std::map<std::string, std::set<NotificationHandler*>*>  handler_map_t;

/**
 * The NotificationCenter is a main controller object use to implement an Observer pattern.  Observers,
 * or NotificationHandlers, can register themselves to receive notification that are identified by
 * string names.  Other objects can then post notifications using that name to notify observers
 * of events.
 *
 * The NotificationCenter is thread safe. Multiple threads can access the NotificationCenter at the same
 * time. However, care must be taken in cases where notifications are being posted by one thread at the same
 * time NotificationHandlers are being added or removed by another thread.  The NotificationCenter is proteced
 * by a ReadWriteLock that allows multiple threads to post notification simultaniously but if NotificationHandlers
 * are added or removed, then only a single thread can aquire the lock.  Callers need to be sure
 * that when adding or removing NotificationHandlers, no other locks are held that could cause deadlock
 * with other threads posting notifications.
 *
 * When a NotificationHander is registered, it MUST NOT be destroyed until it is removed from the NotificationCenter
 * by calling removeHandler
 *
 */
class NotificationCenter {
public:
	NotificationCenter();
	virtual ~NotificationCenter();

	/**
	 * Registers a handler for a given notification name.  Callers to this method should not hold
	 * any locks.
	 *
	 * @param handler - Pointer to a NotificationHandler object.  This is NOT a transfer of ownership;
	 * however, a NotificationHandler must not be destroyed as long as it is regsitered with the
	 * NotificationCenter.  If it is to be destroyed, it MUST be removed by calling the removeHandler()
	 *
	 * @param notificationName - The name of the notification
	 */
	virtual void registerHandler(NotificationHandler* handler, const std::string& notificationName);

	/**
	 * Removes a handler from the NotificationCenter.  Callers to this method should not hold any locks.
	 *
	 * @param handler - Pointer to a NotificationHandler object.
	 *
	 * @param notificationName - The notification name.
	 */
	virtual void removeHandler(NotificationHandler* handler, const std::string& notificationName);

	/**
	 * Posts a notification.  All registered NotificationHandler will be notified of the notification.
	 *
	 * @param notificationName - The name of the notification
	 *
	 * @param notification - An optaion notification specific object that contains more information and
	 * details about the notification.
	 */
	virtual void postNotification(const std::string& notificationName, Notification* notification = NULL);

    bool operator==(const NotificationCenter& otherCenter) const {
        return this == &otherCenter;
    }

protected:
	handler_map_t _handlerMap;
	zios::common::ReadWriteLock _lock;
};

}
} /* namespace zios */
#endif /* NOTIFICATIONCENTER_H_ */
