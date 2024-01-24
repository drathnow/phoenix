/** @file LibraryActivator.h
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
#ifndef LIBRARYACTIVATOR_H_
#define LIBRARYACTIVATOR_H_

#include <cstdio>

namespace zios {
namespace common {

class Configuration;

class LibraryActivator {
public:
    ~LibraryActivator();

    static const char* LIB_KEY;
    static const char* NAME_KEY;
    static const char* CONFIG_KEY;
    static const char* MAIN_KEY;

    static const char* DEFAULT_MAIN_FUNC;

    static void activateLibraries(zios::common::Configuration& config);

    /**
     * Activates a shared library and invokes the specific main function
     *
     * @param libraryName - The name of the library. In order for this library to be activated, it must
     * be on the LD_LIBRARY_PATH.
     *
     * @param mainFunction - The name of the main fuction to invoke
     *
     * @param argument - Argument passed to the main function
     *
     * @return 0 if everything is happy. -1 if something went wrong, errno contains details
     */
    static int activateLibrary(const char* libraryName, const char* mainFunction, void* argument = NULL);

private:
    LibraryActivator();

};

}
} /* namespace zios */

#endif /* LIBRARYACTIVATOR_H_ */
