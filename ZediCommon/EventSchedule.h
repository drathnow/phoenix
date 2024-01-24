/** @file EventSchedule.h
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
#ifndef EVENTSCHEDULE_H_
#define EVENTSCHEDULE_H_
#include <ctime>
#include <stdint.h>

#include "Utl.h"

namespace zios {
namespace common {

class MonotonicTime;

class EventSchedule {
public:

    /**
     * Constructs an EventSchedule with a start time
     *
     * @param startTime - A start time
     *
     * @param intervalSeconds - interval of event, in seconds.
     *
     * @param monotonicTime - Pointer to heap allocate monotonicTime object. This is a transfer of
     * ownership.
     */
    EventSchedule(time_t startTime, int32_t intervalSeconds, MonotonicTime* monotonicTime = NULL);

    /**
     *
     * @param intervalSeconds - interval of event, in seconds.
     *
     * @param monotonicTime - Pointer to heap allocate monotonicTime object. This is a transfer of
     * ownership.
     */
    EventSchedule(int32_t intervalSeconds, MonotonicTime* monotonicTime = NULL);


    /**
     * A copy constructor. Everything will be copied
     * from the other object to the new one, with the exception of the _monotonicTime member.  If it was
     * assigned a differnet object at construction time, that member will not be copied.  Instead, this
     * object will be assigned the global MonotonicTime object.
     *
     * @param otherEventSchedule - Reference to another copy constructor
     */
    EventSchedule(const EventSchedule& otherEventSchedule);
    virtual ~EventSchedule();

    /**
     * Returns the next due time for this schedule.
     *
     * @return time_t - the next due time.
     */
    virtual time_t nextDueTime() const;

    /**
     * Returns the number of seconds until the next event is due
     *
     * @return seconds from now until the next event
     */
    virtual int secondsUntilNextDueEvent() const;

    /**
     * Assignement operator.
     *
     * @param otherEventSchedule - Reference to another copy constructor.  Everything will be copied
     * from the other object to the new one, with the exception of the _monotonicTime member.  If it was
     * assigned a differnet object at construction time, that member will not be copied.  Instead, this
     * object will be assigned the global MonotonicTime object.
     *
     * @return EventSchedule& - Reference to this
     */
    EventSchedule& operator=(const EventSchedule& otherEventSchedule);

    /**
     * Returns the start time as an absolute time
     *
     * @return the absolute start time.
     */
    virtual int32_t startTime() const { return _startTime; }

    /**
     * Returns the interval (in seconds)
     *
     * @return the interval
     */
    virtual int32_t intervalSeconds() const { return _intervalSeconds; }

    /**
     * Changes the start time for this event
     *
     * @param startTime
     */
    virtual void setStartTime(time_t startTime) { _startTime = startTime; }

    /**
     * Changes the interval for this EventSchdule
     *
     * @param intervalSeconds
     */
    virtual void setIntervalSeconds(int32_t intervalSeconds) { _intervalSeconds = intervalSeconds; }

private:
    time_t _startTime;
    int32_t _intervalSeconds;
    MonotonicTime* _monotonicTime;
};

} /* namespace zap */
} /* namespace zios */

#endif /* EVENTSCHEDULE_H_ */
