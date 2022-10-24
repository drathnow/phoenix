#ifndef CLOCK_H_
#define CLOCK_H_

#include <ctime>
#include <cstdint>
#include <string>

namespace dios::foundation
{

class Clock
{
public:
    virtual ~Clock() {}

    virtual time_t currentTime() const = 0;
    virtual bool isDst() const = 0;
	virtual clock_t currentTimeMS(void) const = 0;
};

class SystemClock : public Clock
{
public:
    static SystemClock globalSystemTime;

	SystemClock() = default;
	~SystemClock();

    time_t currentTime() const;
    bool isDst() const;
    clock_t currentTimeMS(void) const;
};

class MonotonicClock : public Clock
{
public:
    static MonotonicClock globalMonotonicClock;

    MonotonicClock() = default;
	~MonotonicClock();

    time_t currentTime() const;
    bool isDst() const;
    clock_t currentTimeMS(void) const;
};


class DeltaTime {
public:
    ~DeltaTime();

    /**
     * Parses a delta time specification.  The time can be specified as follows:
     *
     *    [days] HH:MM:SS
     *
     *    where
     *        days - Is optional. It is a number of days
     *        HH   - Number of hours
     *        DD   - Number of days
     *        SS   - Number of seconds
     *
     * With the exceptoin of days, all values must be specified. There is no restriction
     * on the values specified for hours, minutes, and seconds other than they must be valid
     * signed numbers.  So the following are valid:
     *
     *    00:00:60  - 60 seconds
     *    100:00:15 - 100 hours, 0 Minitues, 15 seconds = 360015 seconds.
     *
     * @return -1 if the format string was invalid or could not be parsed or
     * number of seconds;
     */
    static int32_t secondsForDeltaTimeString(const char* deltaTimeStr);

    /**
     * Converts number of seconds, as an int, into a delta time string
     *
     * @param[in] seconds
     *
     * @return A delta time string
     */
    static std::string deltaTimeStringForSeconds(int seconds);

private:
    DeltaTime();
};

class AbsoluteTime {
public:
    ~AbsoluteTime();

    /**
     * Parses an absolute time value expressed as an ISO date.  That time
     * can be either a full date specification, i.e.:
     *
     *      YYYY-MM-DDTHH:MM:SS e.g. 2015-03-23T10:23:45
     *
     *  or just a time spec:
     *
     *      HH:MM:SS e.g. 10:23:45
     *
     *  If only a time is specified, then the start date is assumed to be January 1, 1970.
     *
     * @param[in] timeStr - A time expressed as ISO time.
     *
     * @return time_t - The time_t representation of the time string. -1 if the
     * timeStr argument is invalid
     */
    static time_t absoluteTimeForString(const char* timeStr);

    /**
     * Formats a given time as a IOS standard time. (i.e. YYYY-MM-DDTHH:mm:ssZ)
     *
     * @param[in] aTime the time to format
     * @param[out] buf the buffer to return the formatted time
     * @param[in] bufSize the size of the buf argument
     * @return pointer to a formatted time string (buf).  NULL if the buffer is too small
     */
    static const char* formattedISOTimeForTime(time_t aTime, char* buf, size_t bufSize);

    /**
     * Formats a given time as a non-IOS standard time. (i.e. YYYY-MM-DD HH:mm:ss)
     *
     * @param[in] aTime the time to format
     * @param[out] buf the buffer to return the formatted time
     * @param[in] bufSize the size of the buf argument
     * @return pointer to a formatted time string (buf).  NULL if the buffer is too small
     */
    static const char* formattedTimeForTime(time_t aTime, char* buf, size_t bufSize);

private:
    AbsoluteTime();
};

}

#endif /* CLOCK_H_ */
