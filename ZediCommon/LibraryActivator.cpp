/** @file LibraryActivator.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 6, 2016 by daver:
 * @endhistory
 */
#include "LibraryActivator.h"
#include "Configuration.h"
#include <cjson/cJSON.h>

#include <dlfcn.h>
#include <cerrno>
#include <cstring>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace zios {
namespace common {

using namespace log4cplus;

typedef int (*LIB_ENTRY_FUNC)(void*);

const char* LibraryActivator::LIB_KEY = "LibLoader";
const char* LibraryActivator::NAME_KEY = "Name";
const char* LibraryActivator::CONFIG_KEY = "Config";
const char* LibraryActivator::MAIN_KEY = "Main";
const char* LibraryActivator::DEFAULT_MAIN_FUNC = "soinit";


static const Logger logger = Logger::getInstance("zios.LibraryActivator");

static int activateLibraryWithArgument(const char* libraryName, const char* mainFunction, void* argument) {
    void* handle;
    int result = -1;
    if ((handle = ::dlopen(libraryName, RTLD_NOW)) == NULL) {
        LOG4CPLUS_ERROR(logger, "Unable to activate Library " << libraryName << ": " << ::dlerror());
        return -1;
    } else {
        LIB_ENTRY_FUNC entry = (LIB_ENTRY_FUNC)::dlsym(handle, mainFunction);
        if (entry == NULL)
            LOG4CPLUS_ERROR(logger, "Unable to activate Library "
                                    << libraryName
                                    << ": Main entry function "
                                    << mainFunction
                                    << " not found (dlerror: "
                                    << ::dlerror()
                                    << ")");
        else {
            LOG4CPLUS_DEBUG(logger, "Activating library " << libraryName << " with main entry function " << mainFunction);
            entry(argument);
            LOG4CPLUS_DEBUG(logger, libraryName << " activation complete");
            result = 0;
        }
    }
    return result;
}

static void activateLibraryFromConfig(cJSON* jsonConfig, int idx)  {
    const char* mainFuncName = LibraryActivator::DEFAULT_MAIN_FUNC;
    const char* libName = NULL;

    cJSON* nameKey = cJSON_GetObjectItem(jsonConfig, LibraryActivator::NAME_KEY);
    if (nameKey == NULL)
        LOG4CPLUS_ERROR(logger, "Library config array item " << idx << " is missing '" << LibraryActivator::NAME_KEY << "' key.  Item ignored");
    else {
        libName = nameKey->valuestring;
        if (libName == NULL || ::strlen(libName) == 0)
            LOG4CPLUS_ERROR(logger, "Library config array item " << idx << " is missing '" << LibraryActivator::NAME_KEY << "' key.  Item ignored");
    }

    cJSON* mainKey = cJSON_GetObjectItem(jsonConfig, LibraryActivator::MAIN_KEY);
    if (mainKey != NULL)
        mainFuncName = mainKey->valuestring;

    cJSON* configKey = cJSON_GetObjectItem(jsonConfig, LibraryActivator::CONFIG_KEY);

    activateLibraryWithArgument(libName, mainFuncName, configKey);
}

LibraryActivator::LibraryActivator() {
}

LibraryActivator::~LibraryActivator() {
}

void LibraryActivator::activateLibraries(zios::common::Configuration& config) {
    cJSON* jsonArray = config.cjsonItemForKey(LIB_KEY);
    if (jsonArray != NULL) {
        for (int i = 0; i < cJSON_GetArraySize(jsonArray); i++) {
            cJSON* nextConfig = cJSON_GetArrayItem((cJSON*)jsonArray, i);
            if (nextConfig == NULL)
                LOG4CPLUS_ERROR(logger, "Something odd. There should be an array item but there is none!!");
            else
                activateLibraryFromConfig(nextConfig, i);
        }

    }
}

int LibraryActivator::activateLibrary(const char* libraryName, const char* mainFunction, void* argument) {
    if (NULL == libraryName && NULL == mainFunction) {
        errno = EINVAL;
        return -1;
    }

    return activateLibraryWithArgument(libraryName, mainFunction, argument);
}


}
} /* namespace zios */
