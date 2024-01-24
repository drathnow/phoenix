#include <gtest/gtest.h>
#include <log4cIncludes.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>


int main(int argc, char **argv)
{
    log4cplus::BasicConfigurator config;
    config.configure();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}
