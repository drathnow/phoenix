#ifndef __EVENTSCHEDULE_H_
#define __EVENTSCHEDULE_H_

#include <cstdint>
#include <ctime>

#include "Clock.h"

#pragma once

namespace dios::foundation
{

class EventSchedule 
{
public:
    static EventSchedule NoEventSchedule;

    EventSchedule() = default;
    EventSchedule(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock* clock = &MonotonicClock::globalMonotonicClock);
    EventSchedule(const EventSchedule& otherEventSchedule);
    EventSchedule(EventSchedule&& otherEventSchedule);
    ~EventSchedule();

    EventSchedule& operator=(EventSchedule&& other);
    EventSchedule& operator=(const EventSchedule& other);

    time_t nextDueTime() const;
    time_t nextDueTimeFromTime(time_t time) const;

    static time_t nextDueTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock = MonotonicClock::globalMonotonicClock);
    static time_t nextDueTimeFromTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, time_t fromTime);

private:
    time_t _startTime{0};
    int32_t _intervalSeconds{0};
    int32_t _durationSeconds{0};
    const Clock* _clock{nullptr};
};

}

#endif  //  __EVENTSCHEDULE_H_
