#pragma once

#ifndef ALARMMEISTERFACTORY_H_
#define ALARMMEISTERFACTORY_H_
#include <StringHelper.h>

#include "AlarmConfiguration.h"
#include "AlarmMeister.h"

namespace dios::domain
{

class AlarmMeisterFactory
{
public:
    AlarmMeisterFactory() = default;
    ~AlarmMeisterFactory() = default;


    template<typename T>
    inline AlarmMeister<T>* alarmMeisterForAlarmLimits(const alarm_limits_t& alarmLimits) const
    {
        AlarmConfiguration<T> alarmConfiguration;
        T setLimit;
        T clearLimit;
        IAlarmRange<T>* lowLowRange = nullptr;
        IAlarmRange<T>* lowRange = nullptr;
        IAlarmRange<T>* highHighRange = nullptr;
        IAlarmRange<T>* highRange = nullptr;

        alarmConfiguration.setSetTime(alarmLimits.set_time_seconds);
        alarmConfiguration.setClearTime(alarmLimits.clear_time_seconds);
        alarm_mask_t* maskPtr = (alarm_mask_t*)&alarmLimits.alarm_mask;


        if (maskPtr->low_enabled)
        {
            StringHelper::stringToNumber(alarmLimits.low_set_limit, setLimit);
            StringHelper::stringToNumber(alarmLimits.low_clear_limit, clearLimit);
            Threshold<T> setThreshold(setLimit, &compare_less_equal);
            Threshold<T> clearThreshold(clearLimit, &compare_less_equal);
            lowRange = new AlarmRange<T>(setThreshold, clearThreshold, AlarmType::ALARM_TYPE_LOW);
        }

        if (maskPtr->low_low_enabled)
        {
            StringHelper::stringToNumber(alarmLimits.low_low_set_limit, setLimit);
            StringHelper::stringToNumber(alarmLimits.low_low_clear_limit, clearLimit);
            Threshold<T> setThreshold(setLimit, &compare_less_equal);
            Threshold<T> clearThreshold(clearLimit, &compare_less_equal);
            lowLowRange = new AlarmRange<T>(setThreshold, clearThreshold, AlarmType::ALARM_TYPE_LOW_LOW, lowRange);
        }

        if (maskPtr->high_high_enabled)
        {
            StringHelper::stringToNumber(alarmLimits.high_set_limit, setLimit);
            StringHelper::stringToNumber(alarmLimits.high_clear_limit, clearLimit);
            Threshold<T> setThreshold(setLimit, &compare_less_equal);
            Threshold<T> clearThreshold(clearLimit, &compare_less_equal);
            highHighRange = new AlarmRange<T>(setThreshold, clearThreshold, AlarmType::ALARM_TYPE_HIGH_HIGH);
        }

        if (maskPtr->high_enabled)
        {
            StringHelper::stringToNumber(alarmLimits.high_high_set_limit, setLimit);
            StringHelper::stringToNumber(alarmLimits.high_high_clear_limit, clearLimit);
            Threshold<T> setThreshold(setLimit, &compare_less_equal);
            Threshold<T> clearThreshold(clearLimit, &compare_less_equal);
            highRange = new AlarmRange<T>(setThreshold, clearThreshold, AlarmType::ALARM_TYPE_HIGH);
        }

        return new AlarmLimitMeister<T>(highRange, lowRange, alarmLimits.set_time_seconds, alarmLimits.clear_time_seconds);
    }

};

} /* namespace dios */


#endif /* ALARMMEISTERFACTORY_H_ */
