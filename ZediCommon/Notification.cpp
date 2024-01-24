/** @file Notification.cpp
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
#include "Notification.h"

namespace zios {
namespace common {

using namespace std;

const char* NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM("deltaTimeSeconds");
const char* NotificationPropertyNames::NETWORK_SERVER_NAME_PARAMETER("NetworkServerName");
const char* NotificationPropertyNames::NETWORK_STATUS_PARAMETER("NetworkStatus");

Notification::Notification() : _name(""), _tag(NULL) {
}

Notification::Notification(const std::string& name, void* tag) : _name(name), _tag(tag)  {
}

Notification::Notification(const Notification& otherNotification) :
    _name(otherNotification._name),
    _properties(otherNotification._properties),
    _tag(otherNotification._tag) {
}

Notification::~Notification() {
}

const string* Notification::valueForPropertyName(const string& propertyName) {
	map<string, string>::iterator iter = _properties.find(propertyName);
	return (iter == _properties.end()) ? NULL : &iter->second;
}

void Notification::addProperty(const string& propertyName, const string& value) {
	pair<string, string> newPair(propertyName, value);
	_properties.insert(newPair);
}

Notification& Notification::operator=(const Notification& otherNotification) {
    _name = otherNotification._name;
    _properties.insert(otherNotification._properties.begin(), otherNotification._properties.end());
    _tag = otherNotification._tag;
    return *this;
}

}
} /* namespace zios */
