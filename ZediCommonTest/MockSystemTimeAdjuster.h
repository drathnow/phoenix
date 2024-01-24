/** @file MockSystemTimeAdjuster.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 1, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKSYSTEMTIMEADJUSTER_H_
#define MOCKSYSTEMTIMEADJUSTER_H_
#include <gmock/gmock.h>
#include <cstdio>
#include <SystemTimeAdjuster.h>

namespace zios {
namespace common {

class MockSystemTimeAdjuster : public SystemTimeAdjuster {
public:
    MockSystemTimeAdjuster() : SystemTimeAdjuster(0, 0, 0, NULL, NULL, NULL) {}
    ~MockSystemTimeAdjuster() {destroyMockSystemTimeAdjuster(); }

    MOCK_METHOD0(destroyMockSystemTimeAdjuster, void());
    MOCK_METHOD2(adjustTimeWithServerTime, int(time_t originalDeviceTime, time_t serverTime));
};


}
}

#endif /* MOCKSYSTEMTIMEADJUSTER_H_ */
