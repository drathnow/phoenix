/** @file CommonFactory.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 9, 2014 by daver:
 * @endhistory
 */
#include <sstream>

#include "CppInject.h"
#include "Hasher.h"
#include "Configuration.h"
#include "CommonFactory.h"
#include "SystemTime.h"
#include "NotificationCenter.h"
#include <cjson/cJSON.h>
#include "timerq.h"
#include "io.h"
#include "AppManager.h"
#include "ThreadPool.h"

namespace zios {
namespace common {

using namespace CppInject;
using namespace std;

static TimerService* makeTimerService(Zone& zone)
{
    TimerQueue* timerQueue = new TimerQueue();
    TimerServiceProvider* timerService = new TimerServiceProvider(timerQueue);
    timerService->start();
    return timerService;
}

static NotificationCenter* makeNotificationCenter(Zone& zone)
{
    NotificationCenter* notificationCenter = new NotificationCenter();
    return notificationCenter;
}

static Configuration* makeConfiguration(Zone &zone)
{
    NotificationCenter* notificationCenter = supply<NotificationCenter>::fetch();
    ConfigurationFileManager* configFileManager = supply<ConfigurationFileManager>::fetch();
    return new FileConfiguration(*notificationCenter, *configFileManager);
}

static Poller* makePoller(Zone& zone) {
    return Poller::open();
}

static EventProcessor* makeEventProcessor(Zone& zone) {
    Poller* poller = supply<Poller>::fetch();
    return new DefaultEventProcessor(poller);
}

ThreadPool* makeGlobalThreadPool(Zone& zone)
{
    return new ThreadPool(4);
}

void destroyGlobalThreadPool(ThreadPool* threadPool)
{
    threadPool->shutdown(5);
    delete threadPool;
}

CommonFactory::CommonFactory()
{
}

CommonFactory::~CommonFactory()
{
}

void CommonFactory::registerWithAppManager(AppManager& appManager)
{
    appManager.registerFrameworkInitializer(this, AppManager::PHASE_1);
}

void CommonFactory::initialize()
{
    supply<NotificationCenter>::configure(makeNotificationCenter);
    supply<Configuration>::configure(makeConfiguration);
    supply<TimerService>::configure(makeTimerService);
    supply<Poller>::configure(makePoller);
    supply<EventProcessor>::configure(makeEventProcessor);
    supply<ThreadPool>::configure(makeGlobalThreadPool, destroyGlobalThreadPool);
}

void CommonFactory::start()
{
}

void CommonFactory::shutdown()
{
}

const char* CommonFactory::name()
{
    return "ZediCommand";
}

} /* namespace utl */
} /* namespace zios */
