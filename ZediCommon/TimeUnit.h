/** @file TimeUnit.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 25, 2015 by daver:
 * @endhistory
 */
#ifndef TIMEUNIT_H_
#define TIMEUNIT_H_
#include <stdint.h>

namespace zios {
namespace common {

class NanosecondsTimeUnit;
class MicrosecondsTimeUnit;
class MilliSecondsTimeUnit;
class SecondsTimeUnit;
class HoursTimeUnit;
class MinutesTimeUnit;
class DaysTimeUnit;

/**
 * Utility class for converting between different time units.
 */
class TimeUnit {
public:
    virtual ~TimeUnit();

    static const NanosecondsTimeUnit NANOSECONDS;
    static const MicrosecondsTimeUnit MICROSECONDS;
    static const MilliSecondsTimeUnit MILLISECONDS;
    static const SecondsTimeUnit SECONDS;
    static const MinutesTimeUnit MINUTES;
    static const HoursTimeUnit HOURS;
    static const DaysTimeUnit DAYS;

    virtual int64_t toNanos(int64_t d) const = 0;
    virtual int64_t toMicros(int64_t d) const = 0;
    virtual int64_t toMillis(int64_t d) const = 0;
    virtual int64_t toSeconds(int64_t d) const = 0;
    virtual int64_t toMinutes(int64_t d) const = 0;
    virtual int64_t toHours(int64_t d) const = 0;
    virtual int64_t toDays(int64_t d) const = 0;
    virtual int64_t convert(int64_t d, const TimeUnit& timeUnit) const = 0;

    bool operator==(const TimeUnit& otherUnit) const {
        return this == &otherUnit;
    }

protected:
    TimeUnit();
};

class NanosecondsTimeUnit : public TimeUnit {
public:
    ~NanosecondsTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    NanosecondsTimeUnit();
};

class MicrosecondsTimeUnit : public TimeUnit {
public:
    ~MicrosecondsTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    MicrosecondsTimeUnit();
};

class MilliSecondsTimeUnit : public TimeUnit {
public:
    ~MilliSecondsTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    MilliSecondsTimeUnit();
};

class SecondsTimeUnit : public TimeUnit {
public:
    ~SecondsTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    SecondsTimeUnit();
};


class MinutesTimeUnit : public TimeUnit {
public:
    ~MinutesTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    MinutesTimeUnit();
};

class HoursTimeUnit : public TimeUnit {
public:
    ~HoursTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    HoursTimeUnit();
};

class DaysTimeUnit : public TimeUnit {
public:
    ~DaysTimeUnit();

    int64_t toNanos(int64_t d) const;
    int64_t toMicros(int64_t d) const;
    int64_t toMillis(int64_t d) const;
    int64_t toSeconds(int64_t d) const;
    int64_t toMinutes(int64_t d) const;
    int64_t toHours(int64_t d) const;
    int64_t toDays(int64_t d) const;
    int64_t convert(int64_t d, const TimeUnit& timeUnit) const;

    friend class TimeUnit;

private:
    DaysTimeUnit();
};


} /* namespace common */
} /* namespace zios */

#endif /* TIMEUNIT_H_ */
