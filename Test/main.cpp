#include <iostream>
#include <functional>
#include <chrono>
#include <iterator>
#include <mutex>
#include <thread>
#include <Clock.h>
#include <deadband.h>
#include <Scheduler.h>
#include <vector>
#include <WatchdogClient.h>
#include <cstring>
#include <Measurement.h>
#include <sstream>
#include <map>
#include <domain.h>
#include <ExtendedPriorityQueue.h>

#include "String.h"
#include "AlarmLimit.h"

using namespace std;
using namespace zios::domain;
using namespace zios::foundation;

ostream& operator<<(ostream &out, const String &s)
{
    out << s._str;
    return out;
}

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
static const std::string& numberToString(T number, std::string &output)
{
    std::ostringstream oss;
    oss << number;
    output = std::move(oss.str());
    return output;
}

static std::map<index_id_t, IMeasurement*> measurementMap;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
void foo(ThresholdEx<T> threshold)
{
    cout << "Foo: " << threshold.isExceeded(20) << " ---value: " << threshold.thresholdValue();
    cout << endl;
}

template<typename T, typename Compare_func>
void foox(ThresholdExx<T, Compare_func>& threshold)
{
    cout << "Foo: " << threshold.isExceeded(20) << " ---value: " << threshold.thresholdValue();
    cout << endl;
}


int main(int argc, char **argv)
{
    //ThresholdExx greaterEqThreshold(5, std::greater_equal<int32_t>{ });
    ThresholdExx<int32_t, std::greater_equal<int32_t>> lessEqThreshold(10);

    //foo(greaterEqThreshold);
    foox(lessEqThreshold);

//    AlarmLimit<int32_t, std::less_equal<int32_t>> alarmLimit(setLimit, clearLimit);
//    alarmLimit.alarmStatusForValue(12, MonotonicClock::globalMonotonicClock, 0, 0);

    return 0;
}
