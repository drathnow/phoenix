/** @file MockMonotonicTime.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 8, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKMONOTONICTIME_H_
#define MOCKMONOTONICTIME_H_
#include <gmock/gmock.h>
#include <MonotonicTime.h>

namespace zios {
namespace common {

class MockMonotonicTime : public MonotonicTime {
public:
    MockMonotonicTime() {}
    ~MockMonotonicTime() { destroyMockMonotonicTime(); }

    MOCK_METHOD0(destroyMockMonotonicTime, void());
    MOCK_CONST_METHOD1(getTime, int(struct timespec *tp));
    MOCK_METHOD0(getTimeMsec, int64_t(void));
    MOCK_METHOD0(getTimeSec, time_t());

    void expectToBeDestroyed() {
        EXPECT_CALL(*this, destroyMockMonotonicTime());
    }

    bool operator==(const MockMonotonicTime& otherTime) const {
        return this == &otherTime;
    }
};

}
}


#endif /* MOCKMONOTONICTIME_H_ */
