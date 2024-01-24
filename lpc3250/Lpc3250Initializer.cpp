/** @file Lpc3250Initializer.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 4, 2019 by daver:
 * @endhistory
 */

#include "Lpc3250Initializer.h"

#include <AppManager.h>
#include <zhal.h>
#include <CppInject.h>
#include <File.h>
#include <NotificationCenter.h>

#include "lpc3250.h"
#include "Lpc3250NetworkConfiguration.h"
#include "Lpc3250HardwareManager.h"

namespace zios {
namespace lpc3250 {

using namespace CppInject;
using namespace zios::hal;
using namespace zios::common;

static IHardwareManager* makeHardwareManager(Zone& zone)
{
    NotificationCenter* notificationCenter = supply<NotificationCenter>::fetch(zone);
    return lpc3250_init_board(*notificationCenter);
}

static NetworkConfiguration* makeNetworkConfiguration(Zone& zone)
{
    File configFile(LPC3250_INTERFACES_CONFIG);
    return new Lpc3250NetworkConfiguration();
}

Lpc3250Initializer::Lpc3250Initializer()
{
}

Lpc3250Initializer::~Lpc3250Initializer()
{
}

void Lpc3250Initializer::registerWithAppManager(zios::common::AppManager& appManager)
{
    appManager.registerFrameworkInitializer(this, AppManager::PHASE_1);
}

void Lpc3250Initializer::initialize()
{
    supply<IHardwareManager>::configure(makeHardwareManager);
    supply<NetworkConfiguration>::configure(makeNetworkConfiguration);
}

void Lpc3250Initializer::start()
{
}

void Lpc3250Initializer::shutdown()
{
}

const char* Lpc3250Initializer::name()
{
    return "lpc3230";
}

} /* namespace hal */
} /* namespace zios */
