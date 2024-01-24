/** @file NotificationCenter.cpp
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

#include <utility>

namespace zios { namespace common { class Notification; } }

#include "NotificationCenter.h"
#include "NotificationHandler.h"
#include "templates.h"

namespace zios {
namespace common {

const char* NotificationDescriptions::TCP_MAPPED_PORT_CHANGE_NOTIFICATION("TCPPortMappingChange");
const char* NotificationDescriptions::NET_SERVER_NAME_UPDATE_NOTIFICATION("NetServerNameUpdate");
const char* NotificationDescriptions::NET_STATUS_UPDATE_NOTIFICATION("NetStatusUpdate");
const char* NotificationDescriptions::TIME_CHANGED_NOTIFICATION("SystemTimeChanged");
const char* NotificationDescriptions::ZIOS_CHECK_NOTIFICATION("ZIOSRestartOK");
const char* NotificationDescriptions::CONNECTION_SHUTTING_DOWN("ConnectionShuttingDown");
const char* NotificationDescriptions::CONNECTION_LOST_NOTIFICATION("ConnectionLost");
const char* NotificationDescriptions::CONNECTION_CONNECTED_NOTIFICATION("ConnectionConnected");
const char* NotificationDescriptions::NETWORK_ADAPTER_CONNECTED("NetworkAdapterConnected");
const char* NotificationDescriptions::NETWORK_ADAPTER_DISCONNECTED("NetworkAdapterDisconnected");
const char* NotificationDescriptions::TURN_ON_POLL_VOTE_LED("TurnOnPollVoteLED");
const char* NotificationDescriptions::TURN_OFF_POLL_VOTE_LED("TurnOffPollVoteLED");
const char* NotificationDescriptions::TURN_ON_NETWORK_LED("TurnOnNetworkLED");
const char* NotificationDescriptions::TURN_OFF_NETWORK_LED("TurnOffNetworkLED");
const char* NotificationDescriptions::OTAD_STARTING("OTADStarting");
const char* NotificationDescriptions::OTAD_FAILED("OTADFailed");
const char* NotificationDescriptions::EXTERNAL_VALUE_RECEIVED_NOTIFICATION_NAME("extValRcv");
const char* NotificationDescriptions::APP_IOPORT_WRITE_NOTIFICATION_NAME("appIOWrite");
const char* NotificationDescriptions::VALUE_PROPERTY_NAME("value");
const char* NotificationDescriptions::CFG_POINT_UPDATED("CfgPointUpdated");

NotificationCenter::NotificationCenter() {
}

NotificationCenter::~NotificationCenter() {
    for (handler_map_t::iterator iter = _handlerMap.begin(); iter != _handlerMap.end(); ++iter)
        delete iter->second;
    _handlerMap.clear();
}

void NotificationCenter::postNotification(const std::string& notificationName, Notification* notification) {
    ScopedReadLock<ReadWriteLock> lock(_lock);
    std::map<std::string, std::set<NotificationHandler*>*>::iterator mapIter = _handlerMap.find(notificationName);
    if (mapIter != _handlerMap.end()) {
        for (std::set<NotificationHandler*>::iterator iter = mapIter->second->begin();
                iter != mapIter->second->end();
                    ++iter) {
            (*iter)->handleNotification(notificationName, notification);
        }
    }
}

void NotificationCenter::registerHandler(NotificationHandler* handler, const std::string& notificationName) {
	std::set<NotificationHandler*>* handlerSet;
	ScopedWriteLock<ReadWriteLock> lock(_lock);
	std::map<std::string, std::set<NotificationHandler*>*>::iterator iterator = _handlerMap.find(notificationName);
	if (iterator == _handlerMap.end()) {
		handlerSet = new std::set<NotificationHandler*>();
		std::pair<std::string, std::set<NotificationHandler*>*> newPair(notificationName, handlerSet);
		_handlerMap.insert(newPair);
	} else
		handlerSet = iterator->second;
	handlerSet->insert(handler);
}

void NotificationCenter::removeHandler(NotificationHandler* handler, const std::string& notificationName) {
    ScopedWriteLock<ReadWriteLock> lock(_lock);
	std::map<std::string, std::set<NotificationHandler*>*>::iterator iterator = _handlerMap.find(notificationName);
	if (iterator != _handlerMap.end()) {
		std::set<NotificationHandler*>* handlerSet = iterator->second;
		std::set<NotificationHandler*>::iterator iterator = handlerSet->find(handler);
		handlerSet->erase(iterator);
		// if last handler for this name, clean up set....
		if (0 == handlerSet->size()){
		    delete handlerSet;
		    _handlerMap.erase(notificationName);
		}
	}
}

}
} /* namespace zios */
