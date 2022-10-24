#pragma once

#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <cstdint>
#include <mutex>
#include <thread>
#include "JobQueue.h"
#include "threadpool.h"
#include "WatchdogClient.h"


namespace dios::foundation
{

class EventSchedule;
class SchedulerRunner;
class JobWrapper;

class Scheduler
{
public:
    const uint32_t WATCHDOG_TIMEOUT_SECONDS = 10;

    Scheduler(WatchdogClient&& watchdogClient);
    Scheduler(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;
    ~Scheduler();

    uint32_t submit(Job* job);
    uint32_t submit(Job* job, time_t startTime);
    uint32_t submit(Job* job, time_t startTime, int intervalSeconds);
    uint32_t submit(Job* job, time_t startTime, int intervalSeconds, int durationSeconds);

    void start();
    void shutdown(int timeoutSeconds);

    friend class SchedulerTest;
    friend class JobWrapper;

private:
    JobQueue _jobQueue;
    bool _shutdown;
    std::thread* _thread;
    WatchdogClient& _watchdogClient;
    std::mutex _mutex;
    threadpool<> _threadPool;

    void _mainLoop(int timeoutSeconds);
    void _run();
    void _handleJobCompletion(JobWrapper* jobWrapper);
};

}

#endif  //  __SCHEDULER_H_
