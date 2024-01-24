/** @file EventSchedule.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 19, 2015 by daver:
 * @endhistory
 */
#include <cstring>
#include <ctime>
#include <cstdio>
#include "MonotonicTime.h"
#include "EventSchedule.h"

namespace zios {
namespace common {

EventSchedule::EventSchedule(time_t startTime, int32_t intervalSeconds, MonotonicTime* monotonicTime) :
        _startTime(startTime),
        _intervalSeconds(intervalSeconds),
        _monotonicTime(monotonicTime == NULL ? &MonotonicTime::globalSystemTime : monotonicTime) {
}


EventSchedule::EventSchedule(int32_t intervalSeconds, MonotonicTime* monotonicTime) :
        _startTime(0),
        _intervalSeconds(intervalSeconds),
        _monotonicTime(monotonicTime == NULL ? &MonotonicTime::globalSystemTime : monotonicTime) {
        _startTime = _monotonicTime->getTimeSec();
}

EventSchedule::EventSchedule(const EventSchedule& otherEventSchedule)  :
        _startTime(otherEventSchedule._startTime),
        _intervalSeconds(otherEventSchedule._intervalSeconds),
        _monotonicTime(&MonotonicTime::globalSystemTime) {
}

EventSchedule& EventSchedule::operator=(const EventSchedule& otherEventSchedule) {
    _startTime = otherEventSchedule._startTime;
    _intervalSeconds = otherEventSchedule._intervalSeconds;
    _monotonicTime = &MonotonicTime::globalSystemTime;
    return *this;
}

EventSchedule::~EventSchedule() {
    if (_monotonicTime != &MonotonicTime::globalSystemTime)
        delete _monotonicTime;
}

int EventSchedule::secondsUntilNextDueEvent() const {
    time_t now =_monotonicTime->getTimeSec();
    if (_startTime >= now)
        return _startTime + _intervalSeconds;
    return (_intervalSeconds - ((now - _startTime) % _intervalSeconds));
}


time_t EventSchedule::nextDueTime() const {
    time_t now =_monotonicTime->getTimeSec();
    if (_startTime >= now)
        return _startTime + _intervalSeconds;
    return now + (_intervalSeconds - ((now - _startTime) % _intervalSeconds));
}

} /* namespace zap */
} /* namespace zios */
