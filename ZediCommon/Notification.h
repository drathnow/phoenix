/** @file Notification.h
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
 * IncrDev May 27, 2015 by eb: added notification parameters class to hold notification parameter key names
 * IncrDev Jul 15, 2015 by eb: due to strangeness? in ARM GCC optimizer, changed const string to const char *
 * @endhistory
 */

#ifndef NOTIFICATION_H_
#define NOTIFICATION_H_

#include <string>
#include <map>

namespace zios {
namespace common {

using namespace std;

/**
 * Class NotificationPropertyNames
 *
 * This class acts as a collection for constants used as parameter keys for
 * particular notification objects.
 */
class NotificationPropertyNames {
public:
    /**
     * The TIME_CHANGED_NOTIFICATION will be accompanied by a Notification object
     * that contains the delta time applied to the system clock.  That value is
     * stored as a property indexed by this field.
     */
    static const char* DELTA_TIME_SECONDS_PARAM;
    /**
     * The NET_SERVER_NAME_UPDATE_NOTIFICATION will be accompanied by a Notification object
     * that contains the updated server name (address).  That value is
     * stored as a property indexed by this field.
     */
    static const char* NETWORK_SERVER_NAME_PARAMETER;
    /**
     * The NET_STATUS_UPDATE_NOTIFICATION will be accompanied by a Notification object
     * that contains the updated server status.  That value is
     * stored as a property indexed by this field.
     */
    static const char* NETWORK_STATUS_PARAMETER;
};

/** class Notification
 *
 * A Notification object contains information specific to a notification event.  The object
 * contains a map of key/value pairs, both strings, that are set by the source of the notification.
 * Optionally, the notification object can also contain a void* pointer containing notification
 * specific information.
 *
 */
class Notification {
public:
	Notification();

	/**
	 * Constructs a Notification object
	 *
	 * @param name - The name associated with this notification object.
	 *
	 * @param tag - An optional notification specific, void pointer value.  This is NOT
	 * a transfer of ownership.  Caller is respsonsible for cleanup.
	 */
	Notification(const std::string& name, void* tag = NULL);
	Notification(const Notification& otherNotification);
	virtual ~Notification();

	/** addProperty
	 *
	 * Adds a property to the notification
	 *
	 * @param[in] propertyName - name of the property
	 * @param[in] value - value for the property
	 */
	virtual void addProperty(const string& propertyName, const string& value);

	/** valueForPropertyName
	 *
	 * Returns the value for a property name
	 *
	 * @param[in] propertyName
	 *
	 * @return pointer to the property value. NULL if the value does not exit.
	 */
	virtual const string* valueForPropertyName(const string& propertyName);

	/** properties
	 *
	 * Returns the map of properties.
	 *
	 * @return map<string, string>& - Reference to the Notification's property map.
	 *
	 */
	virtual inline const map<string, string>& properties() const { return _properties; }

	/**
	 * Returns a void pointer associated with this Notification.
	 *
	 * void* a void pointer that could be NULL.
	 */
	virtual inline const void* tag() const { return _tag; }

	/**
	 * Name of the notification this Notification object is associated with
	 *
	 * @return the name
	 */
	virtual inline const string& name() const { return _name; }

    Notification& operator=(const Notification& otherNotification);

private:
	std::string _name;
	map<string, string> _properties;
	void* _tag;
};

} // namespace common
} // namespace zios
#endif /* NOTIFICATION_H_ */
