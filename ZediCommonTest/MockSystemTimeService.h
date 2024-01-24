/** @file MockSystemTimeService.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 30, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKSYSTEMTIMESERVICE_H_
#define MOCKSYSTEMTIMESERVICE_H_
#include <gmock/gmock.h>
#include <SystemTimeService.h>

namespace zios {
namespace common {


class MockSystemTimeService : public zios::common::SystemTimeService {
public:
    MockSystemTimeService() : SystemTimeService(NULL) {}
    ~MockSystemTimeService() { destroyMockSystemTimeService(); }

    MOCK_METHOD0(destroyMockSystemTimeService, void());
    MOCK_METHOD1(updateTime, int(time_t toTime));
    MOCK_METHOD1(adjustTime, int(time_t toTime));
    MOCK_METHOD0(updateSystemTimeToHardwareClock, int());
};

}  // namespace  common;
}

#endif /* MOCKSYSTEMTIMESERVICE_H_ */
