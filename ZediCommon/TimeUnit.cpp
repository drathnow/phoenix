/** @file TimeUnit.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 25, 2015 by daver:
 * @endhistory
 */
#include "TimeUnit.h"

namespace zios {
namespace common {

const int64_t C0 = 1L;
const int64_t C1 = C0 * 1000LL;
const int64_t C2 = C1 * 1000LL;
const int64_t C3 = C2 * 1000LL;
const int64_t C4 = C3 * 60LL;
const int64_t C5 = C4 * 60LL;
const int64_t C6 = C5 * 24LL;

const NanosecondsTimeUnit TimeUnit::NANOSECONDS;
const MicrosecondsTimeUnit TimeUnit::MICROSECONDS;
const MilliSecondsTimeUnit TimeUnit::MILLISECONDS;
const SecondsTimeUnit TimeUnit::SECONDS;
const MinutesTimeUnit TimeUnit::MINUTES;
const HoursTimeUnit TimeUnit::HOURS;
const DaysTimeUnit TimeUnit::DAYS;

static const int64_t LONG_MAX_VALUE = 0x7fffffffffffffffLL;
static const int64_t LONG_MIN_VALUE = 0x8000000000000000LL;

static int64_t x(int64_t d, int64_t m, int64_t over) {
       if (d >  over) return LONG_MAX_VALUE;
       if (d < -over) return LONG_MIN_VALUE;
       return d * m;
}

TimeUnit::TimeUnit() {
}

TimeUnit::~TimeUnit() {
}

NanosecondsTimeUnit::NanosecondsTimeUnit() {}
NanosecondsTimeUnit::~NanosecondsTimeUnit() {}
int64_t NanosecondsTimeUnit::toNanos(int64_t d) const   { return d; }
int64_t NanosecondsTimeUnit::toMicros(int64_t d) const  { return d/(C1/C0); }
int64_t NanosecondsTimeUnit::toMillis(int64_t d) const  { return d/(C2/C0); }
int64_t NanosecondsTimeUnit::toSeconds(int64_t d) const { return d/(C3/C0); }
int64_t NanosecondsTimeUnit::toMinutes(int64_t d) const { return d/(C4/C0); }
int64_t NanosecondsTimeUnit::toHours(int64_t d) const   { return d/(C5/C0); }
int64_t NanosecondsTimeUnit::toDays(int64_t d) const    { return d/(C6/C0); }
int64_t NanosecondsTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toNanos(d); }


MicrosecondsTimeUnit::MicrosecondsTimeUnit() {}
MicrosecondsTimeUnit::~MicrosecondsTimeUnit() {}
int64_t MicrosecondsTimeUnit::toNanos(int64_t d) const  { return x(d, C1/C0, LONG_MAX_VALUE/(C1/C0)); }
int64_t MicrosecondsTimeUnit::toMicros(int64_t d) const { return d; }
int64_t MicrosecondsTimeUnit::toMillis(int64_t d) const { return d/(C2/C1); }
int64_t MicrosecondsTimeUnit::toSeconds(int64_t d) const{ return d/(C3/C1); }
int64_t MicrosecondsTimeUnit::toMinutes(int64_t d) const{ return d/(C4/C1); }
int64_t MicrosecondsTimeUnit::toHours(int64_t d) const  { return d/(C5/C1); }
int64_t MicrosecondsTimeUnit::toDays(int64_t d) const   { return d/(C6/C1); }
int64_t MicrosecondsTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toMicros(d); }

MilliSecondsTimeUnit::MilliSecondsTimeUnit() {}
MilliSecondsTimeUnit::~MilliSecondsTimeUnit() {}
int64_t MilliSecondsTimeUnit::toNanos(int64_t d) const   { return x(d, C2/C0, LONG_MAX_VALUE/(C2/C0)); }
int64_t MilliSecondsTimeUnit::toMicros(int64_t d) const  { return x(d, C2/C1, LONG_MAX_VALUE/(C2/C1)); }
int64_t MilliSecondsTimeUnit::toMillis(int64_t d) const  { return d; }
int64_t MilliSecondsTimeUnit::toSeconds(int64_t d) const { return d/(C3/C2); }
int64_t MilliSecondsTimeUnit::toMinutes(int64_t d) const { return d/(C4/C2); }
int64_t MilliSecondsTimeUnit::toHours(int64_t d) const   { return d/(C5/C2); }
int64_t MilliSecondsTimeUnit::toDays(int64_t d) const    { return d/(C6/C2); }
int64_t MilliSecondsTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toMillis(d); }

SecondsTimeUnit::SecondsTimeUnit() {}
SecondsTimeUnit::~SecondsTimeUnit() {}
int64_t SecondsTimeUnit::toNanos(int64_t d) const   { return x(d, C3/C0, LONG_MAX_VALUE/(C3/C0)); }
int64_t SecondsTimeUnit::toMicros(int64_t d) const  { return x(d, C3/C1, LONG_MAX_VALUE/(C3/C1)); }
int64_t SecondsTimeUnit::toMillis(int64_t d) const  { return x(d, C3/C2, LONG_MAX_VALUE/(C3/C2)); }
int64_t SecondsTimeUnit::toSeconds(int64_t d) const { return d; }
int64_t SecondsTimeUnit::toMinutes(int64_t d) const { return d/(C4/C3); }
int64_t SecondsTimeUnit::toHours(int64_t d) const   { return d/(C5/C3); }
int64_t SecondsTimeUnit::toDays(int64_t d) const    { return d/(C6/C3); }
int64_t SecondsTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toSeconds(d); }

MinutesTimeUnit::MinutesTimeUnit() {}
MinutesTimeUnit::~MinutesTimeUnit() {}
int64_t MinutesTimeUnit::toNanos(int64_t d) const   { return x(d, C4/C0, LONG_MAX_VALUE/(C4/C0)); }
int64_t MinutesTimeUnit::toMicros(int64_t d) const  { return x(d, C4/C1, LONG_MAX_VALUE/(C4/C1)); }
int64_t MinutesTimeUnit::toMillis(int64_t d) const  { return x(d, C4/C2, LONG_MAX_VALUE/(C4/C2)); }
int64_t MinutesTimeUnit::toSeconds(int64_t d) const { return x(d, C4/C3, LONG_MAX_VALUE/(C4/C3)); }
int64_t MinutesTimeUnit::toMinutes(int64_t d) const { return d; }
int64_t MinutesTimeUnit::toHours(int64_t d) const   { return d/(C5/C4); }
int64_t MinutesTimeUnit::toDays(int64_t d) const    { return d/(C6/C4); }
int64_t MinutesTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toMinutes(d); }


HoursTimeUnit::HoursTimeUnit() {}
HoursTimeUnit::~HoursTimeUnit() {}
int64_t HoursTimeUnit::toNanos(int64_t d) const  { return x(d, C5/C0, LONG_MAX_VALUE/(C5/C0)); }
int64_t HoursTimeUnit::toMicros(int64_t d) const { return x(d, C5/C1, LONG_MAX_VALUE/(C5/C1)); }
int64_t HoursTimeUnit::toMillis(int64_t d) const { return x(d, C5/C2, LONG_MAX_VALUE/(C5/C2)); }
int64_t HoursTimeUnit::toSeconds(int64_t d) const{ return x(d, C5/C3, LONG_MAX_VALUE/(C5/C3)); }
int64_t HoursTimeUnit::toMinutes(int64_t d) const{ return x(d, C5/C4, LONG_MAX_VALUE/(C5/C4)); }
int64_t HoursTimeUnit::toHours(int64_t d) const  { return d; }
int64_t HoursTimeUnit::toDays(int64_t d) const   { return d/(C6/C5); }
int64_t HoursTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toHours(d); }

DaysTimeUnit::DaysTimeUnit() {}
DaysTimeUnit::~DaysTimeUnit() {}
int64_t DaysTimeUnit::toNanos(int64_t d) const  { return x(d, C6/C0, LONG_MAX_VALUE/(C6/C0)); }
int64_t DaysTimeUnit::toMicros(int64_t d) const { return x(d, C6/C1, LONG_MAX_VALUE/(C6/C1)); }
int64_t DaysTimeUnit::toMillis(int64_t d) const { return x(d, C6/C2, LONG_MAX_VALUE/(C6/C2)); }
int64_t DaysTimeUnit::toSeconds(int64_t d) const{ return x(d, C6/C3, LONG_MAX_VALUE/(C6/C3)); }
int64_t DaysTimeUnit::toMinutes(int64_t d) const{ return x(d, C6/C4, LONG_MAX_VALUE/(C6/C4)); }
int64_t DaysTimeUnit::toHours(int64_t d) const  { return x(d, C6/C5, LONG_MAX_VALUE/(C6/C5)); }
int64_t DaysTimeUnit::toDays(int64_t d) const   { return d; }
int64_t DaysTimeUnit::convert(int64_t d, const TimeUnit& timeUnit) const { return timeUnit.toDays(d); }

} /* namespace common */
} /* namespace zios */
