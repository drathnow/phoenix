/** @file MockThreadPool.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 1, 2020 by daver:
 * @endhistory
 */
#ifndef MOCKTHREADPOOL_H_
#define MOCKTHREADPOOL_H_

#include <gmock/gmock.h>
#include <Runnable.h>
#include <ThreadPool.h>

using namespace zios::common;

class MockThreadPool: public ThreadPool
{
public:
    MockThreadPool(bool callDestroy = true) :
            ThreadPool(0),
            _callDestroy(callDestroy)
    {
    }

    ~MockThreadPool()
    {
        if (_callDestroy)
            destroyMockThreadPool();
    }

    MOCK_METHOD0(destroyMockThreadPool, void());
    MOCK_METHOD0(executingCount, int());
    MOCK_METHOD0(activeCount, int());
    MOCK_METHOD2(execute, bool(Runnable* runnable, bool deleteIt));

    bool _callDestroy;
};

#endif /* MOCKTHREADPOOL_H_ */
