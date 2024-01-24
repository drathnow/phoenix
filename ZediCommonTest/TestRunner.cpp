#include <log4cplus/configurator.h>
#include "gtest/gtest.h"

using namespace log4cplus;
using namespace std;

int main(int argc, char **argv) {
	try {
		PropertyConfigurator::doConfigure("log4cplus.properties");
    } catch (...) {
    	cout << "exception occured while opening log4cplus.properties" << endl;
    }

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
