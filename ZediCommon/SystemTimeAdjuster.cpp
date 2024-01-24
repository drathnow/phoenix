/** @file SystemTimeAdjuster.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 26, 2015 by daver:
 * IncrDev May 27, 2015 by eb: changed naming on notification as part of unification
 * @endhistory
 */
#include <log4cplus/loggingmacros.h>
#include <cmath>
#include <cstdlib>
#include "Notification.h"
#include "NotificationCenter.h"
#include "SystemTime.h"
#include "SystemTimeAdjuster.h"

namespace zios {
namespace common {

const int32_t SystemTimeAdjuster::DefaultAdjustTimeThreshold;
const int32_t SystemTimeAdjuster::DefaultMaxTimeThreshold;
const int32_t SystemTimeAdjuster::DefaultRoundTripThreshold;

const log4cplus::Logger SystemTimeAdjuster::_logger = log4cplus::Logger::getInstance("zios.common.SystemTimeAdjuster");

SystemTimeAdjuster::SystemTimeAdjuster(int32_t adjustTimeThreshold,
                                       int32_t maxTimeThreshold,
                                       int32_t roundTripThreshold,
                                       SystemTimeService* systemTimeService,
                                       NotificationCenter* notificationCenter,
                                       SystemTime *systemTime) :
        _adjustTimeThreshold(adjustTimeThreshold),
        _maxTimeThreshold(maxTimeThreshold),
        _roundTripThreshold(roundTripThreshold),
        _systemTimeService(systemTimeService),
        _notificationCenter(notificationCenter),
        _systemTime(systemTime == NULL ? new SystemTime() : systemTime) {
}

SystemTimeAdjuster::~SystemTimeAdjuster() {
    delete _systemTimeService;
    delete _systemTime;
}

int SystemTimeAdjuster::adjustTimeWithServerTime(time_t originalDeviceTime, time_t serverTime) {
    time_t now = _systemTime->currentTime();

    // If the original time is later than current time, then something is really messed up.
    time_t d = originalDeviceTime - now;
    if (originalDeviceTime > now) {
        LOG4CPLUS_WARN(_logger, "Round trip time is negative! (" << d << ")  No time adjustment will be done.");
        return 0;
    }

    // If the rountrip time is beyond threshold, then don't use it.
    time_t roundTripTime = now - originalDeviceTime;
    if (roundTripTime > _roundTripThreshold) {
        LOG4CPLUS_WARN(_logger, "Round trip time " << roundTripTime << " exceeds threshold. Server time is stale so no time adjustment will be done");
        return 1;
    }

    // Check to see if the difference between the server time and our time is more than threshold.  If it is
    // NOT, then we won't change the time.
    time_t adjustedServerTime = serverTime + roundTripTime/2;
    time_t deltaSeconds = adjustedServerTime - now;
    float absDelta = std::abs((float)deltaSeconds);
    if (absDelta >= _adjustTimeThreshold) {
        if (absDelta >= _maxTimeThreshold) {
            int status = _systemTimeService->updateTime(adjustedServerTime);
            if (status == 0) {
                char intStr[20];
                ::sprintf(intStr, "%d", (int)deltaSeconds);
                std::string deltaStr(intStr);
                Notification notification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION);
                notification.addProperty(NotificationPropertyNames::DELTA_TIME_SECONDS_PARAM, deltaStr);
                _notificationCenter->postNotification(NotificationDescriptions::TIME_CHANGED_NOTIFICATION, &notification);
            }
            return status;
        }
        else
            return _systemTimeService->adjustTime(deltaSeconds);
    } else {
        LOG4CPLUS_DEBUG(_logger, "System time is within threshold difference of server time.  System time not updated.");
        return _systemTimeService->updateSystemTimeToHardwareClock();
    }
    return -1;

}

} /* namespace zap */
} /* namespace zios */
