/** @file CommonFactory.h
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

#ifndef COMMONFACTORY_H_
#define COMMONFACTORY_H_

#include "StartupInitializer.h"

namespace zios {
namespace common {

class AppManager;

class CommonFactory : public StartupInitializer {
public:
     CommonFactory();
    ~CommonFactory();

    void registerWithAppManager(AppManager& appManager);

    void initialize();
    void start();
    void shutdown();
    const char* name();
};

} /* namespace utl */
} /* namespace zios */
#endif /* COMMONFACTORY_H_ */
