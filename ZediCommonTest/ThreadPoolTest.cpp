/** @file ExecutorTest.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 21, 2013 by daver:
 * @endhistory
 */

#include <set>
#include <iostream>
#include <deque>
#include <gmock/gmock.h>
#include <log4cplus/configurator.h>
#include <deque>

#include <gtest/gtest.h>
#include <ConditionVariable.h>
#include <Thread.h>
#include <Mutex.h>
#include "commonmocks.h"

namespace zios {
namespace common_tests {


using namespace zios::common;
using namespace std;

using ::testing::_;
using ::testing::Return;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::ByRef;
using testing::Invoke;
using ::testing::StrEq;

const std::string NOTIFICATION_NAME("Hello World");

class ThreadPoolTest: public testing::Test {
};

class MockThreadPoolRunner : public ThreadPoolRunner {
public:
    MockThreadPoolRunner(BlockingQueue<runner_block_t*>& runnableQueue, int number) :
            ThreadPoolRunner(runnableQueue, number, NULL) {}
    virtual ~MockThreadPoolRunner() {}

    MOCK_CONST_METHOD0(isActive, bool());
    MOCK_METHOD0(shutdown, void());

    bool operator==(const MockThreadPoolRunner& otherRunner) const {
        return this == &otherRunner;
    }
};

class MockThreadPoolRunnerFactory : public ThreadPoolRunnerFactory {
public:
    MockThreadPoolRunnerFactory() {}
    virtual ~MockThreadPoolRunnerFactory() {}

    MOCK_CONST_METHOD3(newThreadPoolRunner, ThreadPoolRunner*(BlockingQueue<runner_block_t*>& runnableQueue, int number, ThreadPool* threadPool));
};

class LockRunnable : public Runnable {
public:
    LockRunnable() {}
    ~LockRunnable() {}

    void run() {
        _lock.lock();
        _lock.unlock();
    }

    Mutex _lock;
};

TEST_F(ThreadPoolTest, shouldReturnActiveThreadCount) {
    LockRunnable runnable;

    ThreadPool threadPool(2);
    runnable._lock.lock();
    ASSERT_TRUE(threadPool.execute(&runnable));
    int cnt = 5;
    while (threadPool.executingCount() == 0 && cnt-- > 0)
        ::usleep(10000);
    ASSERT_EQ(1, threadPool.executingCount());

    runnable._lock.unlock();
    cnt = 5;
    while (threadPool.executingCount() == 1 && cnt-- > 0)
        ::usleep(10000);
    ASSERT_EQ(0, threadPool.executingCount());

    threadPool.shutdown();
    cnt = 5;
    while (threadPool.activeCount() > 0 && cnt-- > 0)
        ::usleep(10000);
    ASSERT_EQ(0, threadPool.activeCount());
    ASSERT_TRUE(threadPool.shutdown(5));
}


class WhenThreadPoolIsAskedToShutdown : public ThreadPoolTest {
public:
    WhenThreadPoolIsAskedToShutdown() : _count(0) {}
    ~WhenThreadPoolIsAskedToShutdown() {}

    void myRun() {
        _lock.lock();
        _count++;
        _lock.unlock();
    }

    Mutex _lock;
    int _count;
};

TEST_F(WhenThreadPoolIsAskedToShutdown, shouldShutdownTheadsForReal) {
    MockRunnable* runnable1 = new MockRunnable();
    MockRunnable* runnable2 = new MockRunnable();

    EXPECT_CALL(*runnable1, run())
        .WillOnce(Invoke(this, &WhenThreadPoolIsAskedToShutdown::myRun));
    EXPECT_CALL(*runnable1, destroyMockRunnable());
    EXPECT_CALL(*runnable2, run())
        .WillOnce(Invoke(this, &WhenThreadPoolIsAskedToShutdown::myRun));
    EXPECT_CALL(*runnable2, destroyMockRunnable());

    ThreadPool threadPool(2);

    threadPool.execute(runnable1, true);
    threadPool.execute(runnable2, true);
    while (_count < 2)
        ::usleep(10000);
    threadPool.shutdown(5);
    ASSERT_TRUE(threadPool.executingCount() == 0);
}

TEST_F(ThreadPoolTest, shouldShutdownTheads) {
    ConditionVariable conditionVariable;
    BlockingQueue<runner_block_t*> queue;
    Mutex mutex;
    MockThreadPoolRunner* runner1 = new MockThreadPoolRunner(queue, 1);
    MockThreadPoolRunner* runner2 = new MockThreadPoolRunner(queue, 2);
    MockThread* thread1 = new MockThread(*runner1);
    MockThread* thread2 = new MockThread(*runner2);
    MockThreadFactory* mockThreadFactory= new MockThreadFactory();
    MockThreadPoolRunnerFactory* mockPoolRunnerFactory = new MockThreadPoolRunnerFactory();

    EXPECT_CALL(*mockPoolRunnerFactory, newThreadPoolRunner(_, 1, _)).WillOnce(Return(runner1));
    EXPECT_CALL(*mockPoolRunnerFactory, newThreadPoolRunner(_, 2, _)).WillOnce(Return(runner2));

    EXPECT_CALL(*mockThreadFactory, newThread(Eq(ByRef(*runner1)), StrEq("ThreadPoolWorker"))).WillOnce(Return(thread1));
    EXPECT_CALL(*mockThreadFactory, newThread(Eq(ByRef(*runner2)), StrEq("ThreadPoolWorker"))).WillOnce(Return(thread2));

    EXPECT_CALL(*thread1, setDetached(true));
    EXPECT_CALL(*thread1, start()).Times(1);
    EXPECT_CALL(*thread2, setDetached(true));
    EXPECT_CALL(*thread2, start()).Times(1);

    ThreadPool threadPool(2, 0, mockPoolRunnerFactory, mockThreadFactory);

    EXPECT_CALL(*runner1, shutdown());
    EXPECT_CALL(*runner2, shutdown());
    threadPool.shutdown();
}

TEST_F(ThreadPoolTest, shouldStartTheads) {
    ConditionVariable conditionVariable;
    BlockingQueue<runner_block_t*> queue;
    Mutex mutex;
    MockThreadPoolRunner* runner1 = new MockThreadPoolRunner(queue, 1);
    MockThreadPoolRunner* runner2 = new MockThreadPoolRunner(queue, 2);
    MockThread* thread1 = new MockThread(*runner1);
    MockThread* thread2 = new MockThread(*runner2);
    MockThreadFactory* mockThreadFactory= new MockThreadFactory();
    MockThreadPoolRunnerFactory* mockPoolRunnerFactory = new MockThreadPoolRunnerFactory();

    EXPECT_CALL(*mockPoolRunnerFactory, newThreadPoolRunner(_, 1, _)).WillOnce(Return(runner1));
    EXPECT_CALL(*mockPoolRunnerFactory, newThreadPoolRunner(_, 2, _)).WillOnce(Return(runner2));

    EXPECT_CALL(*mockThreadFactory, newThread(Eq(ByRef(*runner1)), StrEq("ThreadPoolWorker"))).WillOnce(Return(thread1));
    EXPECT_CALL(*mockThreadFactory, newThread(Eq(ByRef(*runner2)), StrEq("ThreadPoolWorker"))).WillOnce(Return(thread2));

    EXPECT_CALL(*thread1, setDetached(true));
    EXPECT_CALL(*thread1, start()).Times(1);
    EXPECT_CALL(*thread2, setDetached(true));
    EXPECT_CALL(*thread2, start()).Times(1);

    ThreadPool threadPool(2, 0, mockPoolRunnerFactory, mockThreadFactory);
}

}
}
