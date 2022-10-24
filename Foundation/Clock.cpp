#include "Clock.h"

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <bits/types/clock_t.h>
#include <bits/types/struct_timespec.h>
#include <bits/types/struct_tm.h>
#include <bits/types/time_t.h>
#include <stddef.h>
#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace zios::foundation
{

using namespace std;

static const int MILLISECONDS_PER_SEC = 1000;

SystemClock SystemClock::globalSystemTime;
MonotonicClock MonotonicClock::globalMonotonicClock;

SystemClock::~SystemClock()
{
}

time_t SystemClock::currentTime() const {
	struct timespec tp;
	::clock_gettime(CLOCK_REALTIME, &tp);
    return tp.tv_sec;
}

bool SystemClock::isDst() const {
    time_t now = ::time(NULL);
    struct tm current;
    ::memset(&current, 0, sizeof(struct tm));
    return (::gmtime_r(&now, &current) && current.tm_isdst >= 0);
}

clock_t SystemClock::currentTimeMS(void) const {
	struct timespec tp;
	::clock_gettime(CLOCK_REALTIME, &tp);
    return (tp.tv_sec * MILLISECONDS_PER_SEC + ::round(tp.tv_nsec / 1.0e6));
}

MonotonicClock::~MonotonicClock()
{
}

time_t MonotonicClock::currentTime() const {
	struct timespec tp;
	::clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec;
}

bool MonotonicClock::isDst() const {
    time_t now = ::time(NULL);
    struct tm current;
    ::memset(&current, 0, sizeof(struct tm));
    return (::gmtime_r(&now, &current) && current.tm_isdst >= 0);
}

clock_t MonotonicClock::currentTimeMS(void) const {
	struct timespec tp;
	::clock_gettime(CLOCK_MONOTONIC, &tp);
    return (tp.tv_sec * MILLISECONDS_PER_SEC + ::round(tp.tv_nsec / 1.0e6));
}


static const uint32_t SECS_PER_MIN = 60;
static const uint32_t SECS_PER_HOUR = 60 * SECS_PER_MIN;
static const uint32_t SECS_PER_DAY = 24 * SECS_PER_HOUR;

static const char* formattedTime(const char *fmt, time_t aTime, char *buf, size_t bufSize)
{
    struct tm *localTm = ::localtime(&aTime);
    if (localTm != NULL)
        return ::strftime(buf, bufSize, fmt, localTm) > 0 ? buf : NULL;
    return NULL;
}

DeltaTime::~DeltaTime()
{
}

int32_t DeltaTime::secondsForDeltaTimeString(const char *deltaTimeStr)
{
    int32_t seconds = -1;
    int days = 0;
    int hours = 0;
    int mins = 0;
    int secs = 0;
    if (::sscanf(deltaTimeStr, "%d %d:%d:%d", &days, &hours, &mins, &secs) < 4)
    {
        if (::sscanf(deltaTimeStr, "%d:%d:%d", &hours, &mins, &secs) == 3)
            seconds = hours * SECS_PER_HOUR + mins * SECS_PER_MIN + secs;
    }
    else
        seconds = days * SECS_PER_DAY + hours * SECS_PER_HOUR + mins * SECS_PER_MIN + secs;
    return seconds;
}

string DeltaTime::deltaTimeStringForSeconds(int seconds)
{
    int days = seconds / 86400;
    ostringstream oss;
    if (days > 0)
        oss << days << " ";
    oss << std::setfill('0') << std::setw(2) << (seconds % 86400) / 3600 << ":" << std::setfill('0') << std::setw(2)
            << (seconds % 3600) / 60 << ":" << std::setfill('0') << std::setw(2) << seconds % 60;
    return oss.str();
}

AbsoluteTime::~AbsoluteTime()
{
}

const char* AbsoluteTime::formattedISOTimeForTime(time_t aTime, char *buf, size_t bufSize)
{
    return formattedTime("%FT%XZ", aTime, buf, bufSize);
}

const char* AbsoluteTime::formattedTimeForTime(time_t aTime, char *buf, size_t bufSize)
{
    return formattedTime("%F %X", aTime, buf, bufSize);
}

time_t AbsoluteTime::absoluteTimeForString(const char *timeStr)
{
    struct tm tm;
    time_t result = -1;
    time_t now = ::time(NULL);
    struct tm *localTm = ::localtime(&now);
    if (localTm != NULL)
    {
        tm = *localTm;
        if (::strptime(timeStr, "%FT%X", &tm) == NULL)
        {
            if (::strptime(timeStr, "%X", &tm) == NULL)
            {
                if ((::strptime(timeStr, "%F", &tm)) != NULL)
                {
                    tm.tm_hour = 0;
                    tm.tm_min = 0;
                    tm.tm_sec = 0;
                    tm.tm_isdst = -1;
                    result = ::mktime(&tm);
                }
            }
            else
            {
                tm.tm_year = 70;
                tm.tm_mon = 0;
                tm.tm_yday = 0;
                tm.tm_wday = 0;
                tm.tm_mday = 1;
                tm.tm_isdst = -1;
                result = ::mktime(&tm);
            }
        }
        else
        {
            tm.tm_isdst = -1;
            result = ::mktime(&tm);
        }
    }
    return result;
}


}
