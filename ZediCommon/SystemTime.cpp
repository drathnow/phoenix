/** @file SystemTime.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * IncrDev: Jun 18, 2014 by kevinl: reverse back, removed all changes by kl
 * IncrDev: May 13, 2014 by kevinl: added a few more methods to the class and change isExpired()'s return from int to bool
 * IncrDev May  8, 2014 by kevinl: added a few more methods to the class
 * IncrDev May  1, 2014 by kevinl: added getSysTimeMS and checkTimeout methods to the class
 * Created Nov 12, 2013 by daver:
 * @endhistory
 */
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include "SystemTime.h"

namespace zios {
namespace common {

const int SystemTime::MICROSECONDS_PER_MS = 1000;
const int SystemTime::MILLISECONDS_PER_SEC = 1000;
SystemTime SystemTime::globalSystemTime;

SystemTime::SystemTime() {
}

SystemTime::~SystemTime() {
}

time_t SystemTime::currentTime() const {
    return ::time(NULL);
}

bool SystemTime::isDst() const {
    time_t now = ::time(NULL);
    struct tm current;
    ::memset(&current, 0, sizeof(struct tm));
    return (::gmtime_r(&now, &current) && current.tm_isdst >= 0);
}

int SystemTime::timeOfDay(struct timeval *tv, struct timezone* tz) const {
    return ::gettimeofday(tv, tz);
}

clock_t SystemTime::currentTimeMS(void) const {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec * MILLISECONDS_PER_SEC + t.tv_usec / MICROSECONDS_PER_MS);
}

} /* namespace reporting */
} /* namespace zios */
