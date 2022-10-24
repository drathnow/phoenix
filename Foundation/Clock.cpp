#include "Clock.h"

#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <cmath>

namespace zios::foundation
{

static const int MILLISECONDS_PER_SEC = 1000;

SystemClock SystemClock::globalSystemTime;
MonotonicClock MonotonicClock::globalMonotonicClock;

SystemClock::SystemClock()
{
}

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

MonotonicClock::MonotonicClock()
{
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


}
