#include <iostream>
#include <cassert>
#include "EventSchedule.h"

namespace zios::foundation
{

using namespace std;

EventSchedule EventSchedule::NoEventSchedule{0, 0, 0};


EventSchedule::EventSchedule(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock* clock) :
        _startTime(startTime),
        _intervalSeconds(intervalSeconds),
        _durationSeconds(durationSeconds),
        _clock(clock)
{
    assert(clock != nullptr);
}

EventSchedule::EventSchedule(const EventSchedule& otherEventSchedule) :
                _startTime(otherEventSchedule._startTime),
                _intervalSeconds(otherEventSchedule._intervalSeconds),
                _durationSeconds(otherEventSchedule._durationSeconds),
                _clock(otherEventSchedule._clock)
{
    assert(otherEventSchedule._clock != nullptr);
}


EventSchedule::EventSchedule(EventSchedule&& otherEventSchedule) :
                _startTime(otherEventSchedule._startTime),
                _intervalSeconds(otherEventSchedule._intervalSeconds),
                _durationSeconds(otherEventSchedule._durationSeconds),
                _clock(otherEventSchedule._clock)
{
    assert(otherEventSchedule._clock != nullptr);
}

EventSchedule::~EventSchedule()
{
}

EventSchedule& EventSchedule::operator=(const EventSchedule& other)
{
    _startTime = other._startTime;
    _intervalSeconds = other._intervalSeconds;
    _durationSeconds = other._durationSeconds;
    _clock = other._clock;
    return *this;
}

EventSchedule& EventSchedule::operator=(EventSchedule&& other)
{
    _startTime = other._startTime;
    _intervalSeconds = other._intervalSeconds;
    _durationSeconds = other._durationSeconds;
    _clock = other._clock;
    return *this;
}

time_t EventSchedule::nextDueTime() const
{
    return nextDueTime(_startTime, _intervalSeconds, _durationSeconds, *_clock);
}

time_t EventSchedule::nextDueTimeFromTime(time_t time) const
{
    return nextDueTimeFromTime(_startTime, _intervalSeconds, _durationSeconds, time);
}

time_t EventSchedule::nextDueTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock)
{
    time_t now = clock.currentTime();
    return nextDueTimeFromTime(startTime, intervalSeconds, durationSeconds, now);
}

time_t EventSchedule::nextDueTimeFromTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, time_t fromTime)
{
    //
    // No start time? Then we start now
    //
    if (startTime == 0)
        return fromTime;

    if (fromTime <= startTime)
        return startTime;

    //
    // If the duration has expired, then we are done
    //
    if (durationSeconds > 0 && fromTime > startTime + durationSeconds)
        return 0;

    return fromTime + (intervalSeconds - ((fromTime - startTime) % intervalSeconds));
}

}
