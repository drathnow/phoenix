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

    alarm_limits_id_t createAlarmLimits(const alarm_limits_t& alarmLimits);
    int updateAlarmLimits(const alarm_limits_t& alarmLimits);
    int deleteAlarmLimitsWithOid(alarm_limits_id_t oid);
    alarm_limits_t* alarmLimitsForOid(alarm_limits_id_t oid);
};

} /* namespace dios */

#endif /* __ALARMLIMITSREPOSITORY_H___ */
