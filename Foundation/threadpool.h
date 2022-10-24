// =========================================================================
// Copyright 2022 -- present Liam Huang (Yuuki) [liamhuang0205@gmail.com].
// Author: Liam Huang
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =========================================================================

#pragma once

#include "blocking_queue.h"
#include "threadsafe_queue.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

namespace dios::foundation
{
template<typename QueueType = threadsafe_queue<std::function<void()>>>
class threadpool
{
public:
    using wlock = std::unique_lock<std::shared_mutex>;
    using rlock = std::shared_lock<std::shared_mutex>;

public:
    threadpool(int threadCount = 4);
    ~threadpool();
    threadpool(const threadpool&) = delete;
    threadpool(threadpool&&) = delete;
    threadpool& operator=(const threadpool&) = delete;
    threadpool& operator=(threadpool&&) = delete;

public:
    void terminate();  // stop and process all delegated tasks
    void cancel();     // stop and drop all tasks remained in queue

public:
    bool isRunning() const;
    int size() const;

private:
    bool _is_running() const
    {
        return !_stopped && !_cancelled;
    }

private:
    void workerMainLoop();

public:
    template<class Ret, class F, class ... Args>
    std::future<Ret> submit(F &&f, Args &&... args);

private:
    bool _stopped{ false };
    bool _cancelled{ false };
    std::vector<std::thread> _workers;
    mutable QueueType _taskQueue;
    mutable std::shared_mutex _mtx;
    mutable std::condition_variable_any _conditionVariable;
    mutable std::once_flag _once;
};

template<typename QueueType>
threadpool<QueueType>::threadpool(int threadCount)
{
    std::call_once(_once, [this, threadCount]()
    {
        wlock lock(_mtx);
        _stopped = false;
        _cancelled = false;
        _workers.reserve(threadCount);
        for (int i = 0; i < threadCount; ++i)
        {
            _workers.emplace_back(std::bind(&threadpool<QueueType>::workerMainLoop, this));
        }
    });

}

template<typename QueueType>
inline threadpool<QueueType>::~threadpool()
{
    terminate();
}

template<typename QueueType>
inline void threadpool<QueueType>::terminate()
{
    {
        wlock lock(_mtx);
        if (_is_running())
        {
            _stopped = true;
        } else
        {
            return;
        }
    }
    _conditionVariable.notify_all();
    for (auto &worker : _workers)
    {
        worker.join();
    }
}

template<typename QueueType>
inline void threadpool<QueueType>::cancel()
{
    {
        wlock lock(_mtx);
        if (_is_running())
        {
            _cancelled = true;
        } else
        {
            return;
        }
    }
    _taskQueue.clear();
    _conditionVariable.notify_all();
    for (auto &worker : _workers)
    {
        worker.join();
    }
}

template<typename QueueType>
inline bool threadpool<QueueType>::isRunning() const
{
    rlock lock(_mtx);
    return _is_running();
}

template<typename QueueType>
inline int threadpool<QueueType>::size() const
{
    rlock lock(_mtx);
    return _workers.size();
}

template<typename QueueType>
inline void threadpool<QueueType>::workerMainLoop()
{
    for (;;)
    {
        bool pop = false;
        std::function<void()> task;
        {
            wlock lock(_mtx);
            _conditionVariable.wait(lock, [this, &pop, &task]
            {
                pop = _taskQueue.pop(task);
                return _cancelled || _stopped || pop;
            });
        }
        if (_cancelled || (_stopped && !pop))
        {
            return;
        }
        task();
    }
}

template<typename QueueType>
template<class Ret, class F, class ... Args>
std::future<Ret> threadpool<QueueType>::submit(F &&f, Args &&... args)
{
    using return_t = Ret;
    using future_t = std::future<return_t>;
    using task_t = std::packaged_task<return_t()>;

    {
        rlock lock(_mtx);
        if (_stopped || _cancelled)
            throw std::runtime_error("Delegating task to a threadpool "
                    "that has been terminated or canceled.");
    }

    auto bind_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    std::shared_ptr<task_t> task = std::make_shared<task_t>(std::move(bind_func));
    future_t fut = task->get_future();
    _taskQueue.emplace([task]() -> void
    {   (*task)();});
    _conditionVariable.notify_one();
    return fut;
}

}  // namespace yuuki
