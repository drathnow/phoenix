/** @file MonotonicTime.h
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
 * @endhistory
 */
#ifndef MONOTONICTIME_H_
#define MONOTONICTIME_H_
#include <ctime>
#include <sys/time.h>
#include "templates.h"
#include <stdint.h>

namespace zios {
namespace common {

class MonotonicTime {
public:
    static MonotonicTime globalSystemTime;


    MonotonicTime();
    virtual ~MonotonicTime();

    /**
     * Retrieves the time from the CLOCK_MONOTONIC source and returns it in the structure
     * specified by tp.
     * See clock_gettime from MAN 2 for more details
     * @param tp - A pointer to a struct timespec
     * @return 0 on success, -1 for failure
     */
    virtual int getTime(struct timespec *tp) const;

    /**
     * Retrieves the time from the CLOCK_MONOTONIC source and returns the value as the
     * number of msec.
     *
     * @throw Exception if unable to read the time
     * @return Number of msec.
     */
    virtual int64_t getTimeMsec(void);

    /**
     * Retrieves the time from the CLOCK_MONOTONIC source and returns the value as the
     * number of seconds since (time_t)
     *
     * @throw Exception if unable to read the time
     * @return Number of sec.
     */
    virtual time_t getTimeSec();

    bool operator==(const MonotonicTime& otherTime) const {
        return this == &otherTime;
    }
};

}}  // namespace

#endif /* MONOTONICTIME_H_ */
