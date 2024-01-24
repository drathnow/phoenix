/** @file SystemTimeAdjuster.h
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
#ifndef SYSTEMTIMEADJUSTER_H_
#define SYSTEMTIMEADJUSTER_H_
#include <log4cplus/logger.h>
#include <stdint.h>

#include "Utl.h"
#include "SystemTimeService.h"

namespace zios {
namespace common {

class SystemTime;
class NotificationCenter;

/**
 * Class that adjusts system time according to the difference between the current time
 * and the time from a server.  The class required to parameters to do adjustements:
 *
 *      adjustTimeThreshold
 *      maxTimeThreshold
 *
 * Assume diff = currentTime - serverTime
 *
 * If (abs(diff) < adjustTimeThreshold) do nothing
 * If (abs(diff) >= adjustTimeThreshold) && (abs(diff) < _maxTimeThreshold) adjust time gradually
 * If (abs(diff) >= maxTimeThreshold) set the time abruptly
 *
 * An abrupt change in time will result in a notification being broadcast.  The notification
 * will contain the amount of time (positive or negative) that the systme clock has changed by.
 *
 * The SystemTimeAdjuster class it not thread-safe. Owners of the object must take care
 * to synchronize access if the class is used by multiple threads.
 *
 */
class SystemTimeAdjuster : Noncopyable {
public:

    /**
     * This is a default value for the difference between the server time and
     * device time.
     */
    static const int32_t DefaultAdjustTimeThreshold = 15;
    static const int32_t DefaultMaxTimeThreshold = 600;   // 10 Min.
    static const int32_t DefaultRoundTripThreshold = 600; // 10 Min.

    /**
     *
     * @param adjustTimeThreshold - If the absolute difference between the curren
     *
     * @param maxTimeThreshold
     *
     * @param systemTimeService - Pointer to a SystemTimeService object.  This is a transfer of ownership.
     *
     * @param roundTripThreshold - If the number of seconds taken to obtain the sever time exceeds this value,
     * then the server time is assumed to be stale and no time change is done.
     *
     * @param notificationCenter - Pointer to a NotificationCenter object.  This is NOT a transfer of ownership.
     *
     * @param systemTime - An optional pointer to a heap allocated SystemTime object.  This is a transfer of ownership.
     */
    SystemTimeAdjuster(int32_t adjustTimeThreshold,
                       int32_t maxTimeThreshold,
                       int32_t roundTripThreshold,
                       SystemTimeService* systemTimeService,
                       NotificationCenter* notificationCenter,
                       SystemTime* systemTime = NULL);
    virtual ~SystemTimeAdjuster();

    /**
     * Adjusts the system time to match the sever time. Obtaining the server time requires sending a request
     * to the server and waiting for the response to be sent back. The time taken to send and receive the
     * server time must be taken into account. We will assume that half of the time required to do this
     * will account for the time for the response to get from the server to the device. This value will
     * be added to the server time.
     *
     * @param originalDeviceTime - The time when the request was initiated.
     *
     * @param serverTime - The time received from the server.
     *
     * @return 0 if the time was set correctly.
     *         -1 if an error occurs.
     *          1 if the In the event that the amount of time taken to receive the system time
     *          exceeds the roundTripThreshold, the time will not be set but a warning message
     *          will be printed and zero (0) will be returned.
     *
     */
    virtual int adjustTimeWithServerTime(time_t originalDeviceTime, time_t serverTime);

private:
    int32_t _adjustTimeThreshold;
    int32_t _maxTimeThreshold;
    int32_t _roundTripThreshold;
    SystemTimeService* _systemTimeService;
    NotificationCenter* _notificationCenter;
    SystemTime* _systemTime;

    static const log4cplus::Logger _logger;
};

} /* namespace zap */
} /* namespace zios */

#endif /* SYSTEMTIMEADJUSTER_H_ */
