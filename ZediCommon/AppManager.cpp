/** @file AppManager.cpp 
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
#include <cstdlib>
#include <cassert>
#include <log4cplus/loggingmacros.h>

#include "AppManager.h"
#include "StartupInitializer.h"

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;


AppManager::AppManager() {
}

AppManager::~AppManager() {
}

void AppManager::registerFrameworkInitializer(StartupInitializer* frameworkInitializer, STARTUP_PHASE startupPhase) {
    assert(frameworkInitializer != NULL);
    framework_list_t* theList = NULL;
    startup_map_t::const_iterator iter = _startupMap.find(startupPhase);
    if (_startupMap.end() == iter) {
        theList = new framework_list_t;
        _startupMap.insert(pair<STARTUP_PHASE, framework_list_t*>(startupPhase, theList));
    } else
        theList = iter->second;
    theList->push_back(frameworkInitializer);
}

void AppManager::initialize() {
    for (startup_map_t::const_iterator startupIter = _startupMap.begin(); startupIter != _startupMap.end(); ++startupIter) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter)
            (*frameworkIter)->initialize();
    }
}

void AppManager::dumpFrameworks(std::ostream& stream) const {
    startup_map_t::const_iterator startupIter;

    stream << "Startup Phase 1" << endl;
    startupIter = _startupMap.find(PHASE_1);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter)
            stream << "    " << (*frameworkIter)->name() << endl;
    }

    stream << endl << "Startup Phase 2" << endl;
    startupIter = _startupMap.find(PHASE_2);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter)
            stream << "    " << (*frameworkIter)->name() << endl;
    }

    stream << endl << "Startup Phase 3" << endl;
    startupIter = _startupMap.find(PHASE_3);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter)
            stream << "    " << (*frameworkIter)->name() << endl;
    }
}

void AppManager::start() {
    startup_map_t::const_iterator startupIter;

    startupIter = _startupMap.find(PHASE_1);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Starting " << (*frameworkIter)->name());
            (*frameworkIter)->start();
        }
    }

    startupIter = _startupMap.find(PHASE_2);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Starting " << (*frameworkIter)->name());
            (*frameworkIter)->start();
        }
    }

    startupIter = _startupMap.find(PHASE_3);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Starting " << (*frameworkIter)->name());
            (*frameworkIter)->start();
        }
    }
}

void AppManager::shutdown() {
    startup_map_t::const_iterator startupIter;

    startupIter = _startupMap.find(PHASE_3);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Shutting down " << (*frameworkIter)->name());
            (*frameworkIter)->shutdown();
        }
    }

    startupIter = _startupMap.find(PHASE_2);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Shutting down " << (*frameworkIter)->name());
            (*frameworkIter)->shutdown();
        }
    }

    startupIter = _startupMap.find(PHASE_1);
    if (startupIter != _startupMap.end()) {
        for (framework_list_t::iterator frameworkIter = startupIter->second->begin(); frameworkIter != startupIter->second->end(); ++frameworkIter) {
            LOG4CPLUS_TRACE(Logger::getInstance("zios.AppManager"), "Shutting down " << (*frameworkIter)->name());
            (*frameworkIter)->shutdown();
        }
    }
}

} /* namespace evthdlr */
} /* namespace zios */
