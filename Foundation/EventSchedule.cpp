#include "EventSchedule.h"

namespace zios::foundation
{

EventSchedule::EventSchedule(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock) :
        _startTime(startTime),
        _intervalSeconds(intervalSeconds),
        _durationSeconds(durationSeconds),
        _clock(clock)
{
}

EventSchedule::EventSchedule(const EventSchedule& otherEventSchedule) :
                _startTime(otherEventSchedule._startTime),
                _intervalSeconds(otherEventSchedule._intervalSeconds),
                _durationSeconds(otherEventSchedule._durationSeconds),
                _clock(otherEventSchedule._clock)
{

}

EventSchedule::~EventSchedule()
{
}

time_t EventSchedule::nextDueTime() const
{
    return nextDueTime(_startTime, _intervalSeconds, _durationSeconds, _clock);
}

time_t EventSchedule::nextDueTime(time_t startTime, int32_t intervalSeconds, int32_t durationSeconds, const Clock& clock)
{
    time_t now = clock.currentTime();

    if (now <= startTime)
        return startTime;

    if (durationSeconds > 0 && now > startTime + durationSeconds)
        return 0;

    return now + (intervalSeconds - ((now - startTime) % intervalSeconds));
}

}
