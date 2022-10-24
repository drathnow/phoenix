#pragma once

#ifndef __ALARMCONFIGURATIONSERIALIZER_H___
#define __ALARMCONFIGURATIONSERIALIZER_H___

#include <map>
#include <type_traits>
#include <StringHelper.h>

#include "AlarmConfiguration.h"

namespace zios::domain
{

using namespace std;
using namespace zios::foundation;

static const char *MASK_KEY = "MASK";
static const char *SET_TIME_KEY = "TRIGGER_TIME";
static const char *CLEAR_TIME_KEY = "CLEAR_TIME";
static const char *LOW_LOW_SET_LIMIT_KEY = "LOW_LOW_TRIGGER";
static const char *LOW_LOW_CLEAR_LIMIT_KEY = "LOW_LOW_RESET";
static const char *LOW_SET_LIMIT_KEY = "LOW_TRIGGER";
static const char *LOW_CLEAR_LIMIT_KEY = "LOW_RESET";
static const char *HIGH_SET_LIMIT_KEY = "HIGH_TRIGGER";
static const char *HIGH_CLEAR_LIMIT_KEY = "HIGH_RESET";
static const char *HIGH_HIGH_SET_LIMIT_KEY = "HIGH_HIGH_TRIGGER";
static const char *HIGH_HIGH_CLEAR_LIMIT_KEY = "HIGH_HIGH_RESET";

class AlarmConfigurationSerializer
{
public:
    AlarmConfigurationSerializer() = delete;
    ~AlarmConfigurationSerializer() = delete;


    /**
     * Serializes the contents of this AlarmConfiguration object to a map. The values
     * in the map correspond to all the values in this object.  However, since an
     * AlarmConfiguration is always associated with an IO Point, we do not serialize
     * the datatype of the values.  We rely on the deserialization process to tell us
     * what kind of datatype we must deserialize.
     *
     * @param[out] toMap - The map to serialize to.
     */
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static void serialize(AlarmConfiguration<T>& alarmConfiguration, persistence_map_t &toMap)
    {
        std::ostringstream oss;

        //
        // If there are no alarms active, then we serialize nothing
        //
        if (alarmConfiguration._alarmMask > 0)
        {
            //
            // We must serialize the alarm mask
            //
            oss << (uint32_t) alarmConfiguration._alarmMask;
            toMap[MASK_KEY] = oss.str();

            //
            // If no alarm limits are set, then we do nothing more
            //
            if ((alarmConfiguration._alarmMask & LIMIT_ALARM_MASK) > 0)
            {
                string output;

                toMap.emplace(SET_TIME_KEY, StringHelper::numberToString(alarmConfiguration.setTimeSeconds(), output));
                toMap.emplace(CLEAR_TIME_KEY, StringHelper::numberToString(alarmConfiguration.clearTimeSeconds(), output));
                toMap.emplace(LOW_LOW_SET_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.lowLowSetLimit(), output));
                toMap.emplace(LOW_LOW_CLEAR_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.lowLowClearLimit(), output));
                toMap.emplace(LOW_SET_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.lowSetLimit(), output));
                toMap.emplace(LOW_CLEAR_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.lowClearLimit(), output));
                toMap.emplace(HIGH_SET_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.highSetLimit(), output));
                toMap.emplace(HIGH_CLEAR_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.highClearLimit(), output));
                toMap.emplace(HIGH_HIGH_SET_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.highHighSetLimit(), output));
                toMap.emplace(HIGH_HIGH_CLEAR_LIMIT_KEY, StringHelper::numberToString(alarmConfiguration.highHighClearLimit(), output));
            }
        }
    }

    /**
     * Creates an AlarmConfiguration from a persistence map
     *
     * @param returnAlarmConfiguration - AlarmConfiguration object that will recieve the alarm configuration values.
     * This object will not be cleared by this method.  Callers must clear the AlarmConfiguraiton object before calling
     * this method if they do not want values overwritten.
     *
     * @param serializedMap - A map containing alarm values
     *
     * @param dataType - A data type for the values used for Limit alarms.
     *
     * @return NULL if the persistence map contains invalid/missing data.  On success, the return values will
     * be a pointer to the returnAlarmConfiguration parameters.
     */
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static AlarmConfiguration<T>* alarmConfigurationFromPersistenceMap(AlarmConfiguration<T> &returnAlarmConfiguration, const persistence_map_t &serializedMap)
    {
        istringstream iss;

        persistence_map_t::const_iterator iter;
        if ((iter = serializedMap.find(MASK_KEY)) == serializedMap.end())
        {
            LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Alarm mask is missing");
            return NULL;
        }

        uint32_t mask;
        if (StringHelper::stringToNumber<uint32_t>(iter->second, mask))
        {
            LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Mask(" << iter->second << ")  is not a number");
            return NULL;
        }

        uint16_t setTime = 0;
        uint16_t clearTime = 0;
        bool limitAlarmsActive = (mask & LIMIT_ALARM_MASK) != 0;
        uint32_t tmpTime;  // used to convert to number(uint32_t) using stringutl .. then will "convert to uint16_t)

        if ((iter = serializedMap.find(SET_TIME_KEY)) == serializedMap.end())
        {
            if (limitAlarmsActive)
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Set Time is missing");
                return NULL;
            }
        } else
        {
            if (StringHelper::stringToNumber<uint32_t>(iter->second, tmpTime))
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. setTime(" << iter->second << ")  is not a number");
                return NULL;
            } else
            {
                if (tmpTime > std::numeric_limits<uint16_t>::max())
                {
                    LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. setTime(" << tmpTime << ")  is too large");
                    return NULL;
                } else
                {
                    setTime = (uint16_t)tmpTime;
                }
            }
        }

        if ((iter = serializedMap.find(CLEAR_TIME_KEY)) == serializedMap.end())
        {
            if (limitAlarmsActive)
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Clear Time is missing");
                return NULL;
            }
        } else
        {
            if (StringHelper::stringToNumber<uint32_t>(iter->second, tmpTime))
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. clearTime(" << iter->second << ")  is not a number");
                return NULL;
            } else
            {
                if (tmpTime > std::numeric_limits<uint16_t>::max())
                {
                    LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. clearTime(" << tmpTime << ")  is too large");
                    return NULL;
                } else
                {
                    clearTime = (uint16_t) tmpTime;
                }
            }
        }

        returnAlarmConfiguration.setClearTime(clearTime);
        returnAlarmConfiguration.setSetTime(setTime);

        T setLimit;
        T clearLimit;

        if ((mask & LOW_ALARM_ACTIVE) != 0)
        {
            if ((iter = serializedMap.find(LOW_SET_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Low Set Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, setLimit);

            if ((iter = serializedMap.find(LOW_CLEAR_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Low Clear Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, clearLimit);

            returnAlarmConfiguration.setLowLimits(setLimit, clearLimit);
        }

        if ((mask & LOW_LOW_ALARM_ACTIVE) != 0)
        {
            if ((iter = serializedMap.find(LOW_LOW_SET_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Low Low Set Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, setLimit);

            if ((iter = serializedMap.find(LOW_LOW_CLEAR_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. Low Low Clear Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, clearLimit);

            returnAlarmConfiguration.setLowLowLimits(setLimit, clearLimit);
        }

        if ((mask & HIGH_ALARM_ACTIVE) != 0)
        {
            if ((iter = serializedMap.find(HIGH_SET_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. High Set Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, setLimit);

            if ((iter = serializedMap.find(HIGH_CLEAR_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. High Clear Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, clearLimit);

            returnAlarmConfiguration.setHighLimits(setLimit, clearLimit);
        }

        if ((mask & HIGH_HIGH_ALARM_ACTIVE) != 0)
        {
            if ((iter = serializedMap.find(HIGH_HIGH_SET_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. High High Set Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, setLimit);

            if ((iter = serializedMap.find(HIGH_HIGH_CLEAR_LIMIT_KEY)) == serializedMap.end())
            {
                LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("zios.base.AlarmConfiguration"), "Invalid alarm configuration. High High  Clear Limit is missing");
                return NULL;
            }

            StringHelper::stringToNumber<T>(iter->second, clearLimit);

            returnAlarmConfiguration.setHighHighLimits(setLimit, clearLimit);
        }

        returnAlarmConfiguration.setNoDataEnabled((mask & NO_DATA_ACTIVE) != 0);

        return &returnAlarmConfiguration;
    }
};

}

#endif /* __ALARMCONFIGURATIONSERIALIZER_H___ */
