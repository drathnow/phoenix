#pragma once

#ifndef __ALARMLIMITSREPOSITORY_H___
#define __ALARMLIMITSREPOSITORY_H___

#include <sqlite3.h>
#include <cstdint>

#include "persist.h"
#include "Repository.h"

namespace dios::persist
{

class AlarmLimitsRepository: public Repository
{
public:
    AlarmLimitsRepository() = delete;
    AlarmLimitsRepository(sqlite3* dbContext);
    ~AlarmLimitsRepository() = default;

    int64_t createAlarmLimits(const alarm_limits_t& alarmLimits);
    int updateAlarmLimits(const alarm_limits_t& alarmLimits);
    int deleteAlarmLimitsWithOid(uint64_t oid);
    alarm_limits_t* alarmLimitsForAlarmLimitsId(alarm_limits_t& alarmLimits, uint64_t oid);
};

} /* namespace dios */

#endif /* __ALARMLIMITSREPOSITORY_H___ */
