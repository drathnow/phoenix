/** @file MockSystemTime.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 16, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKSYSTEMTIME_H_
#define MOCKSYSTEMTIME_H_
#include <gmock/gmock.h>
#include <SystemTime.h>

class MockSystemTime : public zios::common::SystemTime {
public:
    MockSystemTime(bool careAboutDestroy = true) : _careAboutDestroy(careAboutDestroy) {}
    ~MockSystemTime() {
        if (_careAboutDestroy)
            destroyMockSystemTime();
    }

    MOCK_METHOD0(destroyMockSystemTime, void());
    MOCK_CONST_METHOD0(currentTime, time_t());
    MOCK_CONST_METHOD2(timeOfDay, int(struct timeval *tv, struct timezone* tz));
    MOCK_CONST_METHOD0(isDst, bool());

    void expectToBeDestroyed() {
        EXPECT_CALL(*this, destroyMockSystemTime());
    }

    bool _careAboutDestroy;
};


#endif /* MOCKSYSTEMTIME_H_ */
