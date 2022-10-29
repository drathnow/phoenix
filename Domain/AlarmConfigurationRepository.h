#pragma once

#ifndef __ALARMCONFIGURATIONPERSISTER_H___
#define __ALARMCONFIGURATIONPERSISTER_H___

#include <Entity.h>
#include <persist.h>

#include "AlarmConfiguration.h"

namespace dios::domain
{

class AlarmConfigurationRepository : public EntityPersister
{
public:
    AlarmConfigurationRepository() = default;
    ~AlarmConfigurationRepository() = default;

    template<typename T>
    void saveAlarmConfiguration(AlarmConfiguration<T>& alarmConfiguration)
    {

    }

    template<typename T>
    void populateDto(const dios::persist::alarm_limits_t& dto, const AlarmConfiguration<T>& alarmConfiguration)
    {

    }
};

}
#endif /* __ALARMCONFIGURATIONPERSISTER_H___ */
