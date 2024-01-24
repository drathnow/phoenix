/** @file SystemTimeService.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 30, 2015 by daver:
 * @endhistory
 */

#include <log4cplus/loggingmacros.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include "HardwareClockService.h"
#include "SystemTimeService.h"

namespace zios {
namespace common {

using namespace log4cplus;

Logger FakeSystemTimeService::_logger = Logger::getInstance("zios.common.FakeSystemTimeService");
Logger SystemTimeService::_logger = Logger::getInstance("zios.common.SystemTimeService");

static const char* FMT_ERROR = "FMT-ERROR";

static void logTimeAction(Logger& logger, const char* action, time_t fromTime, time_t toTime) {
    char oldTimeBuf[32];
    char newTimeBuf[32];

    struct tm *oldTm = ::localtime(&fromTime);
    int len = 0;
    if ((oldTm == NULL) || ((len = ::strftime(oldTimeBuf, sizeof(oldTimeBuf), "%FT%X", oldTm)) == 0)) {
        ::strncpy(oldTimeBuf, FMT_ERROR, sizeof(oldTimeBuf));
        oldTimeBuf[::strlen(FMT_ERROR)] = '\0';
    } else
        oldTimeBuf[len] = '\0';

    struct tm *newTm = ::localtime(&toTime);
    if ((newTm == NULL) || ((len = ::strftime(newTimeBuf, sizeof(newTimeBuf), "%FT%X", newTm)) == 0)) {
        ::strncpy(newTimeBuf, FMT_ERROR, sizeof(newTimeBuf));
        newTimeBuf[::strlen(FMT_ERROR)] = '\0';
    } else
        newTimeBuf[len] = '\0';
    LOG4CPLUS_INFO(logger, action << " system time from " << oldTimeBuf << " to " << newTimeBuf);
}


SystemTimeService::SystemTimeService(HardwareClockService* hardwareClockService) : _hardwareClockService(hardwareClockService) {
}

SystemTimeService::~SystemTimeService() {
}

int SystemTimeService::adjustTime(time_t deltaSeconds) {
    time_t now = ::time(NULL);
    time_t adjustedNow = now + deltaSeconds;

    logTimeAction(_logger, "ADJUSTING", now, adjustedNow);

    int status = 0;
    struct timeval tv;
    tv.tv_sec = deltaSeconds;
    tv.tv_usec = 0;
    if ((status = ::adjtime(&tv, NULL)) == 0) {
        status = _hardwareClockService->updateSystemTimeToHardwareClock();
        return (status >= 0) ? 0 : status;
    }
    return status;
}

int SystemTimeService::updateTime(time_t toTime) {
    logTimeAction(_logger, "SETTING", ::time(NULL), toTime);

    int status = 0;
    struct timeval tv;
    tv.tv_sec = toTime;
    tv.tv_usec = 0;
    if ((status = ::settimeofday(&tv, NULL)) != 0)
        return status;
    return _hardwareClockService->forceSystemTimeToHardwareClock();
}

int SystemTimeService::updateSystemTimeToHardwareClock() {
    int status = _hardwareClockService->updateSystemTimeToHardwareClock();
    return status >= 0 ? 0 : status;
}

FakeSystemTimeService::FakeSystemTimeService(HardwareClockService* hardwareClockService) : SystemTimeService(hardwareClockService){
}

FakeSystemTimeService::~FakeSystemTimeService() {
}

int FakeSystemTimeService::adjustTime(int32_t deltaSeconds) {
    time_t now = ::time(NULL);
    time_t adjustedNow = now + deltaSeconds;
    logTimeAction(_logger, "(Would be) ADJUSTING", now, adjustedNow);
    return _hardwareClockService->updateSystemTimeToHardwareClock();
}

int FakeSystemTimeService::updateTime(time_t toTime) {
    logTimeAction(_logger, "(Would be) SETTING", ::time(NULL), toTime);
    return _hardwareClockService->forceSystemTimeToHardwareClock();
}


} /* namespace zap */
} /* namespace zios */
