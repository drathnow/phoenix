/** @file somain.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 20, 2016 by daver:
 * @endhistory
 */

#include <AppManager.h>

#include "Lpc3250Initializer.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::common;

extern "C" int lpc3250_soinit(void* arg) {
    Lpc3250Initializer* initializer = new Lpc3250Initializer();
    AppManager* appManager = (AppManager*)arg;
    appManager->registerFrameworkInitializer(initializer, AppManager::PHASE_1);
    return 0;
}

}
}
