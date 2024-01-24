/** @file SystemTime.h
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
 * IncrDev: May  8, 2014 by kevinl: added a few more methods to the class
 * IncrDev: May  1, 2014 by kevinl: added getSysTimeMS and checkTimeout methods to the class
 * Created Nov 12, 2013 by daver:
 * @endhistory
 */

#ifndef SYSTEMTIME_H_
#define SYSTEMTIME_H_
#include <ctime>
#include <sys/time.h>
#include "templates.h"

namespace zios {
namespace common {

class SystemTime {
public:
    static SystemTime globalSystemTime;

    SystemTime();
    virtual ~SystemTime();

    static const int MICROSECONDS_PER_MS;
    static const int MILLISECONDS_PER_SEC;

    virtual time_t currentTime() const;
    virtual int timeOfDay(struct timeval *tv, struct timezone* tz = NULL) const;

    virtual bool isDst() const;

    /*
     * to get current system time
     *
     * @return in microseconds
     */
	virtual clock_t currentTimeMS(void) const;

};

} /* namespace reporting */
} /* namespace zios */
#endif /* SYSTEMTIME_H_ */
