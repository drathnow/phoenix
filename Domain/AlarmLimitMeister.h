/** @file AlarmLimitMeister.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Dec 8, 2015 by daver:
 * IncrDev Jan 05, 2016 by eb: added in return code for no creation of alarms because none requested.
 * BugFix_ Jan 07, 2015 by eb:  fix problem where alarm does not validate and double delete on alarm config happens.
 * @endhistory
 */
#ifndef ALARMMEISTER_H_
#define ALARMMEISTER_H_
#include <map>
#include <string>
#include <Clock.h>
#include <memory.h>

#include "AlarmRange.h"
#include "AlarmConfiguration.h"
#include "AlarmConfigurationSerializer.h"

namespace dios::domain
{

using namespace dios::foundation;

/**
 * The AlarmLimitMeister is responsible for comparing a value against a set of ranges and then tracking
 * which is active.  The AlarmLimitMeister can be constructed with one or two ranges.  While the ranges are
 * named 'primary' and 'secondary', their names are arbitrary.  The name only imply an order to they way
 * the value object is evaluated against the two ranges.  A value will always be evaluated against the
 * primary range first and then against the secondary range, if it is specified.  The AlarmLimitMeister must
 * be constructed with at least a primary range.  The secondary range is optional.
 *
 * When two ranges are specified, the AlarmLimitMeister will 'remembers' when a value triggers an alarm
 * state in one the ranges. That triggering range will be marked as 'active. After that, the AlarmLimitMeister
 * will pass values only to the active range until the range returns to a normal state.  What this means is that
 * if a value varies dramatically, the AlarmLimitMeister will not move that alarm status from one range to the
 * other in a single call.  For example, if the AlarmLimitMeister has two ranges:
 *
 *  Primary:
 *      SetLimit = 100
 *      ClearLimit = 80
 *
 *  Secondary:
 *      SetLimit = 30
 *      ClearLimit = 40
 *
 *  and alarmStatusForValue is called three times with the value 90, 20, and 10, the AlarmLimitMeister will not indicate an
 *  alarm status until the call with the value 10.  The AlarmLimitMeister must allow the primary state to go from active to
 *  inactive before it will pass a value to the secondary range.
 *
 */
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class AlarmLimitMeister
{
public:
    AlarmLimitMeister() = delete;
    AlarmLimitMeister(IAlarmRange<T> &primaryRange, IAlarmRange<T> *secondaryRange, uint16_t setTimeSeconds, uint16_t clearTimeSeconds, dios::foundation::Clock &systemTime = dios::foundation::MonotonicClock::globalMonotonicClock) :
        _primaryRange(primaryRange),
        _secondaryRange(secondaryRange),
        _setTimeSeconds(setTimeSeconds),
        _clearTimeSeconds(clearTimeSeconds),
        _systemTime(systemTime)
    {
    }

    ~AlarmLimitMeister()
    {
        delete &_primaryRange;
        delete _secondaryRange;
    }

    AlarmLimitMeister(const AlarmLimitMeister<T> &rhs) = delete;
    AlarmLimitMeister<T>& operator=(const AlarmLimitMeister<T> &rhs) = delete;

    /**
     * Determines the alarm status of a value.
     *
     * @param[in] value - current value to evaluate against.
     * @return
     */
    AlarmStatus alarmStatusForValue(T value)
    {
        AlarmStatus result;
        if (_secondaryRange == NULL)
        {
            result = _primaryRange.alarmStatusForValue(value, _systemTime, _setTimeSeconds, _clearTimeSeconds);
        } else
        {
            if (_primaryRange.isActive())
            {
                result = _primaryRange.alarmStatusForValue(value, _systemTime, _setTimeSeconds, _clearTimeSeconds);
            } else if (_secondaryRange != NULL && _secondaryRange->isActive())
            {
                result = _secondaryRange->alarmStatusForValue(value, _systemTime, _setTimeSeconds, _clearTimeSeconds);
            } else
            {
                result = _primaryRange.alarmStatusForValue(value, _systemTime, _setTimeSeconds, _clearTimeSeconds);
                if (_primaryRange.isActive() == false)
                    result = _secondaryRange->alarmStatusForValue(value, _systemTime, _setTimeSeconds, _clearTimeSeconds);
            }
        }
        return result;
    }

    /**
     * Clears all alarm ranges.
     */
    void clear()
    {
        _primaryRange.clear();
        if (_secondaryRange != NULL)
            _secondaryRange->clear();
    }

    /**
     * Determines if this AlarmLimitMeister is valid. To be valid, the boundaries between the
     * low and high ranges must be mutually exclusive.
     */
    bool isValid() const
    {
        bool result = false;
        if (_secondaryRange != NULL) {
            if (_primaryRange.isValid() && _secondaryRange->isValid()) {
                T primarySetLimit = _primaryRange.highestSetLimit();
                T primaryClearLimit = _primaryRange.lowestClearLimit();
                T secondarySetLimit = _secondaryRange->highestSetLimit();
                T secondaryClearLimit = _secondaryRange->lowestClearLimit();
                if (primarySetLimit > secondarySetLimit)
                    result = primaryClearLimit >= secondaryClearLimit;
                else
                    result = secondaryClearLimit >= primaryClearLimit;
            }
        } else
            result = _primaryRange.isValid();

        return result;
    }

    /**
     * Adds values to the alarm configuration object
     *
     * @param alarmConfiguration reference to alarm configuraiton
     */
    void addValuesToAlarmConfiguration(AlarmConfiguration<T> &alarmConfiguration) const
    {
        alarmConfiguration.setClearTime(_clearTimeSeconds);
        alarmConfiguration.setSetTime(_setTimeSeconds);
        _primaryRange.addValuesToAlarmConfiguration(alarmConfiguration);
        if (NULL != _secondaryRange)
            _secondaryRange->addValuesToAlarmConfiguration(alarmConfiguration);
    }

    /**
     * Validates the alarm configuration and returns an AlarmLimitMeister object. The validation ensure that
     * the individual ranges (those that are present) are valid, and that the ranges are mutual exclusive.
     *
     * @param[in] ptrAlarmConfig - pointer to Alarm configuration object that contains the values for various ranges.
     *                         Ownership is ALWAYS transferred to alarmMeister and is set to NULL)
     *
     * @return AlarmLimitMeister* - Pointer to a heap allocated AlarmLimitMeister object.  This is a transfer of ownership for alarm config.
     * If an object is returned, the value of returnCode is undefined.  If null is returned, the return code will contain the error.  NULL
     * will also be returned if no alarms are active, but the return code will indicate SUCCESS.
     *
     */
    static AlarmLimitMeister* alarmLimitMeisterForAlarmConfiguration(const AlarmConfiguration<T>& alarmConfiguration)
    {
       AlarmLimitMeister* theMeister = NULL;

       if (alarmConfiguration.isAnyLimitAlarmActive())
       {
           std::unique_ptr<AlarmRange<T>> highAlarmRange(_highAlarmRangeForConfig(alarmConfiguration));
           std::unique_ptr<AlarmRange<T>> lowAlarmRange(_lowAlarmRangeForConfig(alarmConfiguration));

           // need additional check for if range did not instantiate but "should" have i.e. did not "validate"
           if (highAlarmRange.get() != NULL || lowAlarmRange.get() != NULL) {
               if (((alarmConfiguration.isHighAlarmActive() || alarmConfiguration.isHighHighAlarmActive()) && NULL == highAlarmRange.get())
                       || ((alarmConfiguration.isLowAlarmActive() || alarmConfiguration.isLowLowAlarmActive()) && NULL == lowAlarmRange.get())
                   ) {
                   LOG4CPLUS_ERROR(Logger::getInstance("dios.realtimeEngine.AlarmLimitMeister"), "High and Low alarm ranges are invalid.");
               } else {

                   if (highAlarmRange.get() == NULL)
                       theMeister = new AlarmLimitMeister(*(lowAlarmRange.release()), NULL, alarmConfiguration.setTimeSeconds(), alarmConfiguration.clearTimeSeconds());
                   else
                       theMeister = new AlarmLimitMeister(*(highAlarmRange.release()), lowAlarmRange.release(), alarmConfiguration.setTimeSeconds(), alarmConfiguration.clearTimeSeconds());

                   if (theMeister->isValid() == false) {
                       LOG4CPLUS_ERROR(Logger::getInstance("dios.realtimeEngine.AlarmLimitMeister"), "High and Low alarm limit parameters are invalid.");
                       delete theMeister;  // note this also deletes alarmconfig (ownership was transferred)
                       theMeister = NULL;
                   }
               }
           }
       }

       return theMeister;
   }

    /**
     * Builds an AlarmLimitMeister from a serialized map of key/value pairs.
     *
     * @param serializedMap - An map<string, string>
     *
     * @param[in] dataType - The data type of the IO Point that will own this AlarmLimitMeister
     *
     * @param[out] returnCode - If this method returns NULL, the return code contains the error.
     *
     *      INVALID_ALARM_SETTING - The alarm settings are invalid.
     *      ALARM_SETTING_MISSING - The AlarmConfiguration object contains no alarm settings.
     *
     * @param[in] factory - Factory object used to build the AlarmConfiguration.  This parameter is
     *      optional. It can be used for testing purposes of to allow the construction of an AlarmConfiguration
     *      object to be done by an object other than the AlarmConfiguration class.  If specified, this is NOT
     *      a transfer of ownership.
     *
     * @return Point to an AlarmLimitMeister.  This is a transfer of ownership.
     */
    static AlarmLimitMeister* alarmLimitMeisterForSerializedMap(const std::map<std::string, std::string> serializedMap, DataType dataType, uint32_t &returnCode)
    {
       AlarmLimitMeister* result = NULL;
       AlarmConfiguration<T> alarmConfig;
       if (NULL != AlarmConfigurationSerializer::alarmConfigurationFromPersistenceMap(alarmConfig, serializedMap))
       {
           if (alarmConfig->hasActiveAlarms())
               result = AlarmLimitMeister<T>::alarmLimitMeisterForAlarmConfiguration(alarmConfig, returnCode);
       }

       return result;
   }

protected:
    IAlarmRange<T>& _primaryRange;
    IAlarmRange<T>* _secondaryRange;
    uint16_t _setTimeSeconds;
    uint16_t _clearTimeSeconds;
    dios::foundation::Clock &_systemTime;

    static AlarmRange<T>* _highAlarmRangeForConfig(const AlarmConfiguration<T> &alarmConfig)
    {
        AlarmRange<T> *alarmRange = NULL;
        if (alarmConfig.isHighHighAlarmActive())
        {
            Threshold<T> setLimit(alarmConfig.highHighSetLimit(), &compare_greater_equal);
            Threshold<T> clearLimit(alarmConfig.highHighClearLimit(), &compare_greater_equal);
            alarmRange = new AlarmRange<T>(setLimit, clearLimit, ALARM_TYPE_HIGH_HIGH);
        }
        if (alarmConfig.isHighAlarmActive())
        {
            Threshold<T> setLimit(alarmConfig.highSetLimit(), &compare_greater_equal);
            Threshold<T> clearLimit(alarmConfig.highClearLimit(), &compare_greater_equal);
            AlarmRange<T> *tmp = new AlarmRange<T>(setLimit, clearLimit, ALARM_TYPE_HIGH, alarmRange);
            alarmRange = tmp;
        }

        if (alarmRange != NULL && alarmRange->isValid() == false)
        {
            LOG4CPLUS_ERROR(Logger::getInstance("dios.realtimeEngine.AlarmLimitMeister"), "High alarm limit parameters are invalid");
            delete alarmRange;
            alarmRange = NULL;
        }
        return alarmRange;
    }

    static AlarmRange<T>* _lowAlarmRangeForConfig(const AlarmConfiguration<T> &alarmConfig)
    {
        AlarmRange<T> *alarmRange = NULL;
        if (alarmConfig.isLowLowAlarmActive())
        {
            Threshold<T> setLimit(alarmConfig.lowLowSetLimit(), &compare_less_equal);
            Threshold<T> clearLimit(alarmConfig.lowLowClearLimit(), &compare_less_equal);
            alarmRange = new AlarmRange<T>(setLimit, clearLimit, ALARM_TYPE_LOW_LOW);
        }

        if (alarmConfig.isLowAlarmActive())
        {
            Threshold<T> setLimit(alarmConfig.lowSetLimit(), &compare_less_equal);
            Threshold<T> clearLimit(alarmConfig.lowClearLimit(), &compare_less_equal);
            AlarmRange<T> *tmp = new AlarmRange<T>(setLimit, clearLimit, ALARM_TYPE_LOW, alarmRange);
            alarmRange = tmp;
        }

        if (alarmRange != NULL && alarmRange->isValid() == false)
        {
            LOG4CPLUS_ERROR(Logger::getInstance("dios.realtimeEngine.AlarmLimitMeister"), "Low alarm limit parameters are invalid");
            delete alarmRange;
            alarmRange = NULL;
        }
        return alarmRange;
    }
};

} /* namespace dios */

#endif /* ALARMMEISTER_H_ */
