#pragma once

#ifndef __REPORTITEM_H___
#define __REPORTITEM_H___

#include "domain.h"
#include "AlarmRange.h"

namespace zios::domain
{

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr>
struct ReportItem
{
    T currentValue;
    AlarmStatus alarmStatus;
    index_id_t index;
};

} /* namespace zios */

#endif /* __REPORTITEM_H___ */
