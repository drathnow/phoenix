/** @file MockTimerService.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 13, 2018 by daver:
 * @endhistory
 */
#ifndef MOCKTIMERSERVICE_H_
#define MOCKTIMERSERVICE_H_

#include <gmock/gmock.h>
#include <timerq.h>

namespace zios {
namespace common {

class MockTimerService : public TimerService {
public:
    MockTimerService() : TimerService() {}
    virtual ~MockTimerService() {}

    MOCK_METHOD3(schedule, tqe_id_t(TimerTask* timerTask, int64_t delay, const TimeUnit& timeUnit));
    MOCK_METHOD1(cancel, void(tqe_id_t tqeId));
};

}
}



#endif /* MOCKTIMERSERVICE_H_ */
