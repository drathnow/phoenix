#include <iostream>
#include <functional>
#include <chrono>
#include <iterator>
#include <mutex>
#include <thread>
#include <Clock.h>
#include <Scheduler.h>
#include <vector>
#include <WatchdogClient.h>
#include <cstring>
#include <Measurement.h>
#include <sstream>
#include <map>
#include <domain.h>
#include <Deadband.h>
#include <ExtendedPriorityQueue.h>

#include "String.h"
#include "AlarmLimit.h"

using namespace std;
using namespace dios::domain;
using namespace dios::foundation;

void foo(Deadband& deadband)
{

}

int main(int argc, char **argv)
{
    AbsoluteDeadband deadband(123);

    return 0;
}
