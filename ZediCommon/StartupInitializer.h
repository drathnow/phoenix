/** @file FrameworkInitializer.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 8, 2018 by daver:
 * @endhistory
 */
#ifndef STARTUPINITIALIZER_H_
#define STARTUPINITIALIZER_H_

namespace zios {
namespace common {

class AppManager;

class StartupInitializer
{
public:
    virtual ~StartupInitializer() {}

    virtual void registerWithAppManager(AppManager& appManager) = 0;

    virtual void initialize() = 0;
    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual const char* name() = 0;
};

}
}



#endif /* STARTUPINITIALIZER_H_ */
