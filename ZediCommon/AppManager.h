/** @file AppManager.h 
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
#ifndef APPMANAGER_H_
#define APPMANAGER_H_
#include <map>
#include <list>
#include <stdint.h>
#include <iostream>

namespace zios {
namespace common {

class StartupInitializer;


typedef std::list<StartupInitializer*>    framework_list_t;
typedef std::map<uint8_t, framework_list_t*> startup_map_t;

/**
 * The AppManage is a singleton that manages the startup and shutdown of the application. The AppManager
 * manages a list of FrameworkIntializer that are called to perform the functions of initializing, starting,
 * and shutting down.  To make this singleton instance accessable to static initializers, the instance is not
 * controlled by the applications IOC framework but is, instead, accessed by the old-school single method of
 * using a static method to return the single instance.
 *
 * The AppManager divides startup and shutdown phases into three phases:
 *
 *      PHASE_1
 *      PHASE_2
 *      PHASE_3
 *
 *  The AppManager maintains a list of StartupInitializer objects that are responsible for managing the startup
 *  and shutdown of their frameworks.  When a client registers a StartupInitializer, it must specify a startup
 *  phase that the start and shutdown methods should be called. The initialize() method is called before start()
 *  and must be independant of the phase.
 *
 *  When AppManager::start is called, it will iterate over each StartupInitializer starting with PHASE_1 and
 *  call StartupInitializer::start().  It will then do the same for all StartupInitializer in PHASE_2 and PHASE_3.
 *  The order StartupInitializer are called in each phase cannot be controlled and should not be controlled.  If a
 *  StartupInitializer has a dependancy on one or more other frameworks, then the StartupInitializer for the various
 *  frameworks must be regitered in other startup phases.  All StartupInitializer should execute independant of
 *  other frameworks within that phase.
 *
 *  When AppManager::shutdown is called, it will call each StartupInitializer::shutdown() in the reverse order as
 *  startup.
 *
 */
class AppManager {
public:
    enum STARTUP_PHASE {
        PHASE_1 = 1,
        PHASE_2 = 2,
        PHASE_3 = 3
    };

    AppManager();
    ~AppManager();
    void registerFrameworkInitializer(StartupInitializer* frameworkInitializer, STARTUP_PHASE startupPhase);

    /**
     * Initializes the application by calling all FrameworkInitialziers::initialize() method.
     */
    void initialize();

    void start();

    void shutdown();

    void dumpFrameworks(std::ostream& stream) const;

private:
    startup_map_t _startupMap;
};


} /* namespace evthdlr */
} /* namespace zios */

#endif /* APPMANAGER_H_ */
