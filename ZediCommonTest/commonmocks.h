/** @file commonmocks.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 25, 2013 by daver:
 * IncrDev Jan 08, 2015 by eb: add in fakeTimerQueue to allow cleanup of DTO.
 * IncrDev Jan 28, 2015 by eb: remove mock notification center
 * @endhistory
 */

#ifndef COMMONMOCKS_H_
#define COMMONMOCKS_H_
#include <gmock/gmock.h>
#include <stdint.h>
#include <ByteBuffer.h>
#include <SystemTime.h>
#include <MonotonicTime.h>
#include <Thread.h>
#include <Runnable.h>
#include <ThreadPool.h>
#include <Configuration.h>
#include <File.h>
#include <timerq.h>
#include <Utl.h>

#include "MockConditionVariable.h"

namespace zios {
namespace common_tests {

using namespace zios::common;

using ::testing::_;
using ::testing::Invoke;
using ::testing::AnyNumber;

class MockPthreadWrapper : public PthreadWrapper {
public:
    MockPthreadWrapper() {}
    ~MockPthreadWrapper() { destroy(); }

    MOCK_METHOD0(destroy, void());
    MOCK_METHOD3(pthread_cond_timedwait, int(pthread_cond_t* conditionVariable, pthread_mutex_t* mutex, struct timespec *timeToWait));
    MOCK_METHOD2(pthread_cond_wait, int(pthread_cond_t* conditionVariable, pthread_mutex_t* mutex));
    MOCK_METHOD2(pthread_cond_init, int(pthread_cond_t* conditionVariable, pthread_condattr_t *cond_attr));
    MOCK_METHOD1(pthread_cond_signal, int(pthread_cond_t* conditionVariable));
    MOCK_METHOD1(pthread_cond_broadcast, int(pthread_cond_t* conditionVariable));
    MOCK_METHOD1(pthread_cond_destroy, int(pthread_cond_t* conditionVariable));
    MOCK_METHOD1(pthread_mutex_destroy, int(pthread_mutex_t* mutex));
};

class MockTimerQueueEntry : public TimerQueueEntry {
public:
    MockTimerQueueEntry() : TimerQueueEntry(0, NULL) {}
    ~MockTimerQueueEntry() { destroyMockTimerQueueEntry(); }

    MOCK_METHOD0(destroyMockTimerQueueEntry, void());
    MOCK_CONST_METHOD0(dueTime, int64_t());
    MOCK_CONST_METHOD0(uniqueId, tqe_id_t());
    MOCK_METHOD0(execute, void());

    bool operator==(const MockTimerQueueEntry& otherEntry) {
        return this == &otherEntry;
    }
};

class MockThreadPool : public ThreadPool {
public:
    MockThreadPool() : ThreadPool(2) {};
    ~MockThreadPool() { destroyMockThreadPool(); }

    MOCK_METHOD0(destroyMockThreadPool, void());
    MOCK_METHOD1(execute, void(Runnable* runnable));
};

class FakeTimeQueue : public TimerQueue {
public:
    FakeTimeQueue(): tmpQE(NULL) {}
    ~FakeTimeQueue() {if (tmpQE != NULL) delete tmpQE;}
    void removeTimerQueueEntry(tqe_id_t tqeId) {}
    void insertTimerQueueEntry(TimerQueueEntry* entry) { if (tmpQE != NULL) delete tmpQE;tmpQE = entry;} // so it is cleaned up.
    TimerQueueEntry* removeNextDueTimerQueueEntry(){ return NULL;}
    void waitForNextEvent(){}
private:
    TimerQueueEntry* tmpQE;
};

class MockTimerQueue : public TimerQueue {
public:
    MockTimerQueue() {}
    ~MockTimerQueue() { destroyMockTimerQueue(); }

    MOCK_METHOD0(destroyMockTimerQueue, void());
    MOCK_METHOD1(removeTimerQueueEntry, void(tqe_id_t tqeId));
    MOCK_METHOD1(insertTimerQueueEntry, void(TimerQueueEntry* entry));
    MOCK_METHOD0(removeNextDueTimerQueueEntry, TimerQueueEntry*());
    MOCK_METHOD0(waitForNextEvent, void());

    void DelegateToFake(){
        ON_CALL(*this, insertTimerQueueEntry(_))
        .WillByDefault(Invoke(&fake_, &FakeTimeQueue::insertTimerQueueEntry));
        }

private:
    FakeTimeQueue fake_;
};

class MockTimerService : public TimerService {
public:
    MockTimerService() {}
    ~MockTimerService() {}

    MOCK_METHOD3(schedule, tqe_id_t(TimerTask* timerTask, time_t dueTime, bool deleteTask));
    MOCK_METHOD1(removeTimerQueueEntry, void(tqe_id_t tqeId));
};



class MockConfigurationFileManager : public ConfigurationFileManager {
public:
    MockConfigurationFileManager() : ConfigurationFileManager("foo", 0) {}
    MOCK_METHOD1(makeLatestConfigurationFile, File&(File& newConfigFile));

    bool operator==(const MockConfigurationFileManager& otherManager) const {
        return this == &otherManager;
    }

};

class MockSystemTime : public SystemTime {
public:
    MockSystemTime() {}
    virtual ~MockSystemTime() {destroy();}

    MOCK_METHOD0(destroy, void());
    MOCK_CONST_METHOD0(currentTime, time_t());
    MOCK_CONST_METHOD2(timeOfDay, int(struct timeval *tv, struct timezone* tz));

};

class MockMonotonicTime : public MonotonicTime {
public:
    MockMonotonicTime() {}
    virtual ~MockMonotonicTime() {destroy();}

    MOCK_METHOD0(destroy, void());
    MOCK_CONST_METHOD1(getTime, int(struct timespec *tp));
};

class MockByteBuffer : public ByteBuffer {
public:
    MockByteBuffer() : ByteBuffer(1) {}
    MockByteBuffer(const MockByteBuffer& otherBuffer) : ByteBuffer(otherBuffer) {
    }

    virtual ~MockByteBuffer() {}

    MOCK_CONST_METHOD0(slice, ByteBuffer*());
    MOCK_METHOD1(setPosition, void(int newPosition));
    MOCK_METHOD1(putString, void(const std::string& str));
    MOCK_METHOD1(putS8, void(int8_t value) );
    MOCK_METHOD1(putU8, void(uint8_t value) );
    MOCK_METHOD1(putS16, void(int16_t value) );
    MOCK_METHOD1(putU16, void(uint16_t value) );
    MOCK_METHOD1(putS32, void(int32_t value) );
    MOCK_METHOD1(putU32, void(uint32_t value) );
    MOCK_METHOD1(putS64, void(int64_t value) );
    MOCK_METHOD1(putU64, void(uint64_t value) );
    MOCK_METHOD1(putFloat, void(float value) );
    MOCK_METHOD1(putDouble, void(double value) );
    MOCK_METHOD2(put, uint32_t(const int8_t* buffer, uint32_t bufferLength));
    MOCK_METHOD0(getString, std::string());
    MOCK_METHOD0(getS8, int8_t());
    MOCK_METHOD0(getU8, uint8_t());
    MOCK_METHOD0(getS16, int16_t());
    MOCK_METHOD0(getU16, uint16_t());
    MOCK_METHOD0(getS32, int32_t());
    MOCK_METHOD0(getU32,  uint32_t());
    MOCK_METHOD0(getS64,  int64_t());
    MOCK_METHOD0(getU64,  uint64_t());
    MOCK_METHOD0(getFloat, float());
    MOCK_METHOD0(getDouble, double());
    MOCK_CONST_METHOD0(buffer, const unsigned char*());
    MOCK_METHOD2(getAll, int(int8_t* buffer, uint32_t bufferLength));
    MOCK_METHOD2(get, int(int8_t* buffer, uint32_t bufferLength));

    bool operator==(const MockByteBuffer& otherBuffer) const {
        return this == &otherBuffer;
    }
};

class MockRunnable : public Runnable {
public:
    MockRunnable() {}
    MockRunnable(const MockRunnable& otherMock) {}
    virtual ~MockRunnable() { destroyMockRunnable(); }

    MOCK_METHOD0(destroyMockRunnable, void());
    MOCK_METHOD0(run, void());

    MockRunnable& operator==(const MockRunnable& otherMock) {
        return *this;
    }
};

class MockThread : public Thread {
public:
    MockThread(Runnable& runnable) : Thread(runnable) {}
    virtual ~MockThread() {}

    MOCK_METHOD0(start, int());
    MOCK_METHOD1(setDetached, void(bool detached));
    MOCK_CONST_METHOD1(join, int(long waitTimeSeconds));
};

class MockThreadFactory : public ThreadFactory {
public:
    MockThreadFactory() {}
    virtual ~MockThreadFactory() {}

    MOCK_CONST_METHOD2(newThread, Thread*(Runnable& runnable, const char* name));
};

}
}

#endif /* COMMONMOCKS_H_ */
