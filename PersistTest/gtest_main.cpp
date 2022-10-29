#include <gtest/gtest.h>
#include <iostream>
#include <log4cplus/log4cplus.h>
#include <Clock.h>
#include <MockClock.h>

using namespace log4cplus;
using namespace testing;

int main(int argc, char **argv)
{
    log4cplus::BasicConfigurator config;
    config.configure();
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
