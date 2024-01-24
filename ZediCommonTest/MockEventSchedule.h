/** @file MockEventSchedule.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 24, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKEVENTSCHEDULE_H_
#define MOCKEVENTSCHEDULE_H_
#include <gmock/gmock.h>
#include "EventSchedule.h"

namespace zios {
namespace common {

class MockEventSchedule : public EventSchedule {
public:
    MockEventSchedule() : EventSchedule(0, 0, NULL) {}
    virtual ~MockEventSchedule() {destroyMockEventSchedule(); }

    MOCK_METHOD0(destroyMockEventSchedule, void());
    MOCK_CONST_METHOD0(nextDueTime, time_t());
};


}
}

#endif /* MOCKEVENTSCHEDULE_H_ */
