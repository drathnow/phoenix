/** @file BlockingQueueTest.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct 9, 2014 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <Runnable.h>
#include <Thread.h>
#include <iostream>
#include <SystemTime.h>
#include <MonotonicTime.h>
#include <Queue.h>

namespace zios {
namespace common {


class BlockingQueueTest : public testing::Test {
};

class WaitForEntryRunner : public Runnable {
public:
    WaitForEntryRunner(BlockingQueue<char*>& queue, long waitTimeMilliseconds) :
            _queue(queue),
            _result(NULL),
            _waitTimeMilliseconds(waitTimeMilliseconds),
            _timeWaited(0),
            _running(false) {}
    ~WaitForEntryRunner() {}

    void run() {
        SystemTime systemTime;
        long now = systemTime.currentTimeMS();
        _running = true;
        _result = _queue.removeFront(_waitTimeMilliseconds);
        _timeWaited = systemTime.currentTimeMS() - now;
        _running = false;
    }

    BlockingQueue<char*>& _queue;
    char* _result;
    long _waitTimeMilliseconds;
    clock_t _timeWaited;
    bool _running;
};

TEST_F(BlockingQueueTest, shouldWakeMultipleThreadsWaitingOnQueue) {
    BlockingQueue<char*> queue;
    WaitForEntryRunner runner1(queue, 200L);
    Thread thread1(runner1);
    WaitForEntryRunner runner2(queue, 200L);
    Thread thread2(runner2);

    thread1.start();
    thread2.start();
    int cnt = 20;
    while (runner1._running == false && runner2._running == false && cnt-- > 0)
        ::usleep(100000);
    ASSERT_TRUE(cnt > 0);
    queue.wake();
    cnt = 20;
    while (runner1._running && runner2._running && cnt-- > 0)
        ::usleep(100000);
    ASSERT_TRUE(cnt != 0);
    ASSERT_EQ(0, thread1.join(5));
    ASSERT_EQ(0, thread2.join(5));
}

TEST_F(BlockingQueueTest, shouldOnlyHaveOneThreadTakeEntryWhenMultipleThreadsWaiting) {
    BlockingQueue<char*> queue;
    WaitForEntryRunner runner1(queue, 200L);
    Thread thread1(runner1);
    WaitForEntryRunner runner2(queue, 200L);
    Thread thread2(runner2);
    char foo[5];

    thread1.start();
    thread2.start();
    int cnt = 10;
    while (runner1._running == false && runner2._running == false && cnt-- > 0)
        ::usleep(100000);
    ASSERT_TRUE(cnt > 0);
    queue.addBack(foo);
    cnt = 5;
    while (runner1._result == NULL && runner2._result == NULL && cnt-- > 0)
        ::usleep(100000);
    queue.wake();
    ASSERT_TRUE((NULL == runner1._result) != (NULL == runner2._result));
    ASSERT_EQ(0, thread1.join(5));
    ASSERT_EQ(0, thread2.join(5));
}

class WhenBlockQueueIsAskedToWaitAndNoEntryIsAdded : public BlockingQueueTest {
public:
    ~WhenBlockQueueIsAskedToWaitAndNoEntryIsAdded() {}
};

TEST_F(WhenBlockQueueIsAskedToWaitAndNoEntryIsAdded, shouldTimeoutIfNoEntryAdded) {
    BlockingQueue<char*> queue;
    MonotonicTime systemTime;
    long then = systemTime.getTimeMsec();
    queue.wait(200);
    long now = systemTime.getTimeMsec();
    ASSERT_TRUE(now - then >= 200) << "Diff is " << (now - then);
}

TEST_F(BlockingQueueTest, shouldWaitForEntryToBeInserted) {
    BlockingQueue<char*> queue;
    WaitForEntryRunner runner(queue, 0);
    Thread thread(runner);
    char foo[5];

    thread.start();
    int cnt = 10;
    while (runner._running == false && cnt-- > 0)
        ::usleep(100000);
    ASSERT_TRUE(cnt > 0);

    queue.addBack(foo);
    cnt = 20;
    while (runner._result == NULL && cnt-- > 0)
        ::usleep(100000);
    ASSERT_TRUE(foo == runner._result);
    ASSERT_EQ(0, thread.join(5));
}

}
}
