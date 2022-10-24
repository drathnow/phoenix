#ifndef __EVENTSCHEDULE_H_
#define __EVENTSCHEDULE_H_

#include <cstdint>
#include <ctime>

#include "Clock.h"

#pragma once

namespace zios::foundation
{

class EventSchedule 
{
public:
    EventSchedule(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock = MonotonicClock::globalMonotonicClock);
    EventSchedule(const EventSchedule& otherEventSchedule);
    ~EventSchedule();

    time_t nextDueTime() const;

    static time_t nextDueTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock = MonotonicClock::globalMonotonicClock);

private:
    time_t _startTime;
    int32_t _intervalSeconds;
    int32_t _durationSeconds;
    const Clock& _clock;
};

}

#endif  //  __EVENTSCHEDULE_H_
