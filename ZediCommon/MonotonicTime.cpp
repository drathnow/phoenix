/** @file MonotonicTime.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 1, 2015 by andrewj:
 * BugFix_ Jun 08, 2015 by eb:  change getTimeMsec to calculate unsigned to prevent sign extension.
 * @endhistory
 */
#include "MonotonicTime.h"
#include "SystemException.h"

#include <sys/time.h>
#include <cstring>
#include <errno.h>

namespace zios {
namespace common {

MonotonicTime MonotonicTime::globalSystemTime;


MonotonicTime::MonotonicTime(){
}

MonotonicTime::~MonotonicTime() {
}

int MonotonicTime::getTime(struct timespec *tp) const {
    return ::clock_gettime(CLOCK_MONOTONIC, tp);
}

int64_t MonotonicTime::getTimeMsec(void) {
    struct timespec tp;
    if (::clock_gettime(CLOCK_MONOTONIC, &tp) == -1)
        THROW_NAMED_EXCEPTION(SystemException, "Unable to read time: " << ::strerror(errno) << "(errno="<< errno << ")");
    //if we don't treat tb_sec as unsigned... the sign of it extends into the S64 to create a -ve number.....
    return ((uint32_t)tp.tv_sec * 1000)+ ((tp.tv_nsec+500000) / 1000000);      // note +500000 is to round up.
}

time_t MonotonicTime::getTimeSec() {
    struct timespec tp;
    if (::clock_gettime(CLOCK_MONOTONIC, &tp) == -1)
        THROW_NAMED_EXCEPTION(SystemException, "Unable to read time: " << ::strerror(errno) << "(errno="<< errno << ")");
    return tp.tv_sec;
}

} /* namespace common */
} /* namespace zios */


