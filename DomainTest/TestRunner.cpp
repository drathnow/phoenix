
#include <gtest/gtest.h>
#include <log4cplus/configurator.h>
#include <iostream>

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace std;

int main(int argc, char **argv)
{
    try
    {
        PropertyConfigurator::doConfigure("log4cplus.properties");
    }
    catch (std::exception& e)
    {
        cout << "exception occured while opening log4cplus.properties: " << e.what() << endl;
    }

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
