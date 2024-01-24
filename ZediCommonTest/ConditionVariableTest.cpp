/** @file ConditionVariableTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 20, 2013 by daver:
 * IncrDev Oct 10, 2013 by eganb: removed warnings
 * IncrDev Apr 03, 2014 by eb:  removed extraneous log4cplus init
 * IncrDev Feb 17, 2016 by eb:  removed expect for mutex destroy as it was removed from class.
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <log4cplus/configurator.h>
#include <sys/time.h>
#include <stdexcept>

#include "Utl.h"
#include "commonmocks.h"
#include "ConditionVariable.h"
#include "Runnable.h"
#include "Thread.h"
#include "MonotonicTime.h"

using namespace zios::common;
using namespace log4cplus;
using namespace std;

namespace zios {
namespace common_tests {

using ::testing::Return;
using ::testing::Invoke;
using ::testing::_;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::MakeMatcher;
using ::testing::Matcher;


class ConditionVariableTest: public testing::Test {
protected:
};

class MyMockMonotonicTime : public MockMonotonicTime {
public:
    MyMockMonotonicTime(struct timespec& returnTime) : _returnTime(returnTime) {}
    ~MyMockMonotonicTime() {}

    int getTimeMock(struct timespec *tp) {
        tp->tv_sec = _returnTime.tv_sec;
        tp->tv_nsec = _returnTime.tv_nsec;
        return 0;
    }

    struct timespec& _returnTime;
};

class TimeSpecMatcher : public MatcherInterface<struct timespec*> {
public:
    explicit TimeSpecMatcher(struct timespec& tv) :
            _tv(tv) {
    }

    virtual bool MatchAndExplain(struct timespec* tv, MatchResultListener* listener) const {
        return _tv.tv_sec == tv->tv_sec && _tv.tv_nsec == tv->tv_nsec;
    }

    virtual void DescribeTo(::std::ostream* os) const {
        *os << "Expected the right timespec but it was wrong";
    }

    virtual void DescribeNegationTo(::std::ostream* os) const {
        *os << "Expected the right timespec but it was not wrong?";
    }

private:
    struct timespec& _tv;
};

inline Matcher<struct timespec*> timespecMatching(struct timespec& tv) {
    return MakeMatcher(new TimeSpecMatcher(tv));
}

TEST_F(ConditionVariableTest, shouldWaitLessThanASecond) {
    MonotonicTime monotonicTime;
    ConditionVariable conditionVariable;

    int64_t now = monotonicTime.getTimeMsec();
    conditionVariable.lockAndWait(500);
    int64_t then = monotonicTime.getTimeMsec();

    int64_t diff = then - now;
    ASSERT_TRUE(diff < 1000 && diff >= 500);
}

TEST_F(ConditionVariableTest, shouldThrowIfWaitReturnsError) {
    struct timespec now         = {100,   1000000L};
    struct timespec expected    = {100, 101000000L};    // includes 100 msec added to "now"
    long waitTimeMilliseconds = 100L;

    MockPthreadWrapper* pthreadWrapper = new MockPthreadWrapper();
    MyMockMonotonicTime* monotonicTime = new MyMockMonotonicTime(now);
    ConditionVariable conditionVariable(pthreadWrapper, monotonicTime);

    EXPECT_CALL(*pthreadWrapper, destroy());
    EXPECT_CALL(*monotonicTime, destroy());

    ON_CALL(*monotonicTime, getTime(_))
        .WillByDefault(Invoke(monotonicTime, &MyMockMonotonicTime::getTimeMock));

    EXPECT_CALL(*pthreadWrapper, pthread_cond_timedwait(_, _, timespecMatching(expected)))
        .WillOnce(Return(EINVAL));

    try {
        conditionVariable.lockAndWait(waitTimeMilliseconds);
        FAIL();
    } catch (std::runtime_error& e) {
    }
}

TEST_F(ConditionVariableTest, shouldNotThrowIfWaitTimesOut) {
    struct timespec now         = {100,   1000000L};
    struct timespec expected    = {100, 101000000L};    // includes 100 msec added to "now"
    long waitTimeMilliseconds = 100L;

    MockPthreadWrapper* pthreadWrapper = new MockPthreadWrapper();
    MyMockMonotonicTime* monotonicTime = new MyMockMonotonicTime(now);
    ConditionVariable conditionVariable(pthreadWrapper, monotonicTime);

    EXPECT_CALL(*pthreadWrapper, destroy());
    EXPECT_CALL(*monotonicTime, destroy());

    ON_CALL(*monotonicTime, getTime(_))
        .WillByDefault(Invoke(monotonicTime, &MyMockMonotonicTime::getTimeMock));

    EXPECT_CALL(*pthreadWrapper, pthread_cond_timedwait(_, _, timespecMatching(expected)))
        .WillOnce(Return(ETIMEDOUT));

    ASSERT_EQ(ConditionVariable::TIMED_OUT, conditionVariable.lockAndWait(waitTimeMilliseconds));
}

TEST_F(ConditionVariableTest, shouldWaitCorrectTime) {
    struct timespec now         = {100,   1000000L};
    struct timespec expected    = {100, 101000000L};    // includes 100 msec added to "now"
    long waitTimeMilliseconds = 100L;

    MockPthreadWrapper* pthreadWrapper = new MockPthreadWrapper();
    MyMockMonotonicTime* monotonicTime = new MyMockMonotonicTime(now);

    EXPECT_CALL(*pthreadWrapper, pthread_cond_init(_, _));

    ConditionVariable conditionVariable(pthreadWrapper, monotonicTime);

    EXPECT_CALL(*pthreadWrapper, destroy());
    EXPECT_CALL(*monotonicTime, destroy());
    EXPECT_CALL(*pthreadWrapper, pthread_cond_destroy(_));
    EXPECT_CALL(*monotonicTime, getTime(_));     // ADJ - not sure why this is complaining - it seems to be called

    ON_CALL(*monotonicTime, getTime(_))
        .WillByDefault(Invoke(monotonicTime, &MyMockMonotonicTime::getTimeMock));

    EXPECT_CALL(*pthreadWrapper, pthread_cond_timedwait(_, _, timespecMatching(expected)))
        .WillOnce(Return(0));

    ASSERT_EQ(ConditionVariable::OK, conditionVariable.lockAndWait(waitTimeMilliseconds));
}

}
}
