/** @file HardwareClockService.cpp
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
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <cstdlib>
#include <ctime>
#include "HardwareClockService.h"

namespace zios {
namespace common {

using namespace log4cplus;

static Logger logger = Logger::getInstance("zios.common.HardwareClockService");

const int32_t HardwareClockService::DefaultTimeSetThreadholdSeconds;

HardwareClockService::HardwareClockService(int32_t timeSetThresholdSeconds) :
        _lastSetTime(0),
        _timeSetThresholdSeconds(timeSetThresholdSeconds) {
}

HardwareClockService::~HardwareClockService() {
}

int HardwareClockService::updateSystemTimeToHardwareClock() {
    if (_lastSetTime == 0 || (::time(NULL) - _lastSetTime > _timeSetThresholdSeconds))
        return forceSystemTimeToHardwareClock();
    LOG4CPLUS_DEBUG(logger, "Insufficient time has elapsed since last HW clock change. No change being done.");
    return 1;
}

int HardwareClockService::forceSystemTimeToHardwareClock() {
    LOG4CPLUS_DEBUG(logger, "Pushing system time to the hardware clock");
    _lastSetTime = ::time(NULL);
    return ::system("/sbin/hwclock --systohc --utc");
}

FakeHardwareClockService::FakeHardwareClockService(int32_t timeSetThresholdSeconds) : HardwareClockService(timeSetThresholdSeconds) {
}

FakeHardwareClockService::~FakeHardwareClockService() {
}

int FakeHardwareClockService::forceSystemTimeToHardwareClock() {
    _lastSetTime = ::time(NULL);
    LOG4CPLUS_DEBUG(logger, "(Would be) Pushing system time to the hardware clock");
    return 0;
}


} /* namespace zap */
} /* namespace zios */
