#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <cstdint>
#include <mutex>
#include <thread>
#include "JobQueue.h"

#pragma once


namespace zios::foundation
{

class EventSchedule;
class SchedulerRunner;

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    uint32_t submit(Job* job, time_t startTime);
    uint32_t submit(Job* job, time_t startTime, int intervalSeconds);
    uint32_t submit(Job* job, time_t startTime, int intervalSeconds, int durationSeconds);

    void start();
    void shutdown(int timeoutSeconds);

    friend class SchedulerTest;

private:
    JobQueue _jobQueue;
    bool _shutdown;
    std::mutex _mutex;
    std::thread* _thread;

    void mainLoop();
    void run();
};

}

#endif  //  __SCHEDULER_H_
