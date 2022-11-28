#pragma once

#ifndef __REPORTITEM_H___
#define __REPORTITEM_H___
#include <foundation.h>
#include <string>

#include "domain.h"
#include "AlarmRange.h"

namespace dios::domain
{

using namespace dios::foundation;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr>
struct ReportItem
{
    T currentValue;
    AlarmStatus alarmStatus;
    time_t lastUpdateTime;
};

} /* namespace dios */

#endif /* __REPORTITEM_H___ */
