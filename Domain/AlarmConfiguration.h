#pragma once

#ifndef ALARMCONFIGURATION_H_
#define ALARMCONFIGURATION_H_

#include <map>
#include <stdint.h>
#include <sstream>
#include <StringHelper.h>
#include <memory>
#include <log4cplus/log4cplus.h>

#include "domain.h"

namespace dios::domain
{

using namespace std;
using namespace log4cplus;
using namespace dios::foundation;

static const uint32_t NO_ALARM_ACTIVE = 0x0;
static const uint32_t LOW_LOW_ALARM_ACTIVE = 0x01;
static const uint32_t LOW_ALARM_ACTIVE = 0x02;
static const uint32_t HIGH_ALARM_ACTIVE = 0x04;
static const uint32_t HIGH_HIGH_ALARM_ACTIVE = 0x08;
static const uint32_t NO_DATA_ACTIVE = 0x10;

static const uint32_t LIMIT_ALARM_MASK = 0x0F;


template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class AlarmConfiguration
{
public:

    AlarmConfiguration() = default;
    ~AlarmConfiguration() = default;

    /**
     * Sets the set time.
     *
     * @param setTimeSeconds
     */
    inline void setSetTime(uint16_t setTimeSeconds)
    {
        _setTimeSeconds = setTimeSeconds;
    }

    /**
     * Sets the clear time
     *
     * @param clearTimeSeconds
     */
    inline void setClearTime(uint16_t clearTimeSeconds)
    {
        _clearTimeSeconds = clearTimeSeconds;
    }

    /**
     * Indicates if any alarm is active
     *
     * @return true if any alarm is active, false otherwise.
     */
    bool hasActiveAlarms() const
    {
        return 0 != _alarmMask;
    }

    /**
     * Indicates if any alarms are active (have values set)
     *
     * @return ture if alarms are active. false otherwise.
     */
    bool isAnyLimitAlarmActive() const
    {
        return (_alarmMask & ~NO_DATA_ACTIVE) != NO_ALARM_ACTIVE;
    }

    /**
     * Indicates if High alarm are active.  If this value returns
     * true, both the SetLimit and ClearLimit will be present.
     *
     * @return true if High alarm is active. flase otherwise.
     */
    inline bool isHighAlarmActive() const
    {
        return (_alarmMask & HIGH_ALARM_ACTIVE) != 0;
    }

    /**
     * Indicates if High High alarm are active.  If this value returns
     * true, both the SetLimit and ClearLimit will be present.
     *
     * @return true if High High alarm is active. flase otherwise.
     */
    inline bool isHighHighAlarmActive() const
    {
        return (_alarmMask & HIGH_HIGH_ALARM_ACTIVE) != 0;
    }

    /**
     * Indicates if Low alarm are active.  If this value returns
     * true, both the SetLimit and ClearLimit will be present.
     *
     * @return true if Low alarm is active. flase otherwise.
     */
    inline bool isLowAlarmActive() const
    {
        return (_alarmMask & LOW_ALARM_ACTIVE) != 0;
    }

    /**
     * Indicates if Low Low alarm are active.  If this value returns
     * true, both the SetLimit and ClearLimit will be present.
     *
     * @return true if Low Low alarm is active. flase otherwise.
     */
    inline bool isLowLowAlarmActive() const
    {
        return (_alarmMask & LOW_LOW_ALARM_ACTIVE) != 0;
    }

    /**
     * Sets the High High alarm limit parameters
     *
     * @param[in] setLimit - Set Limit
     * @param[in] clearLimit - Clear Limit
     */
    inline void setHighHighLimits(T setLimit, T clearLimit)
    {
        _highHighSetLimit = setLimit;
        _highHighClearLimit = clearLimit;
        _alarmMask |= HIGH_HIGH_ALARM_ACTIVE;
    }

    /**
     * Sets the High alarm limit parameters
     *
     * @param[in] setLimit - Set Limit
     * @param[in] clearLimit - Clear Limit
     */
    inline void setHighLimits(T setLimit, T clearLimit)
    {
        _highSetLimit = setLimit;
        _highClearLimit = clearLimit;
        _alarmMask |= HIGH_ALARM_ACTIVE;
    }

    /**
     * Sets the Low alarm limit parameters
     *
     * @param[in] setLimit - Set Limit
     * @param[in] clearLimit - Clear Limit
     */
    inline void setLowLimits(T setLimit, T clearLimit)
    {
        _lowSetLimit = setLimit;
        _lowClearLimit = clearLimit;
        _alarmMask |= LOW_ALARM_ACTIVE;
    }

    /**
     * Enables or disabled no data alarm
     *
     * @param enabled - true to enable, false to disable
     */
    inline void setNoDataEnabled(bool enabled)
    {
        if (enabled)
            _alarmMask |= NO_DATA_ACTIVE;
        else
            _alarmMask &= ~NO_DATA_ACTIVE;
    }

    /**
     * Sets the Low Low alarm limit parameters
     *
     * @param[in] setLimit - Set Limit
     * @param[in] clearLimit - Clear Limit
     */
    inline void setLowLowLimits(T setLimit, T clearLimit)
    {
        _lowLowSetLimit = setLimit;
        _lowLowClearLimit = clearLimit;
        _alarmMask |= LOW_LOW_ALARM_ACTIVE;
    }

    /**
     * Returns the clear time
     *
     * @return Clear time in seconds
     */
    inline uint16_t clearTimeSeconds() const
    {
        return _clearTimeSeconds;
    }

    /**
     * Returns the set time
     *
     * @return Set time in seconds
     */
    inline uint16_t setTimeSeconds() const
    {
        return _setTimeSeconds;
    }

    /**
     * Returns the High Set Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T highSetLimit() const
    {
        return _highSetLimit;
    }

    /**
     * Returns the High Clear Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T highClearLimit() const
    {
        return _highClearLimit;
    }

    /**
     * Returns the High High Set Limit
     *
     * @return Pointer to a value.  Null if limit is not set.
     */
    inline T highHighSetLimit() const
    {
        return _highHighSetLimit;
    }

    /**
     * Returns the High High Clear Limit
     *
     * @return Pointer to a value.  Null if limit is not set.
     */
    inline T highHighClearLimit() const
    {
        return _highHighClearLimit;
    }

    /**
     * Returns the Low Low Sset Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T lowLowSetLimit() const
    {
        return _lowLowSetLimit;
    }

    /**
     * Returns the Low Low Clear Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T lowLowClearLimit() const
    {
        return _lowLowClearLimit;
    }

    /**
     * Returns the Low Set Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T lowSetLimit() const
    {
        return _lowSetLimit;
    }

    /**
     * Returns the Low Clear Limit
     *
     * @return Pointer to a value.  Null if limit is not set.  This is NOT
     * a transfer of ownership.  Do not hold this pointer beyond the life
     * of this AlarmConfiguration object
     */
    inline T lowClearLimit() const
    {
        return _lowClearLimit;
    }

    /**
     * Indicates of no data alarm is enabled
     *
     * @return true if enabled, false otherwise
     */
    inline bool isNoDataEnabled() const
    {
        return 0 != (_alarmMask & NO_DATA_ACTIVE);
    }

    /**
     * Copy Assignement operator.  When assigned, this class does a deep
     * copy.
     *
     * @param[in] otherConfig The other AlarmConfiguration objects we take our
     * values from.
     *
     * @return Reference to this object.
     */
    AlarmConfiguration& operator=(const AlarmConfiguration &otherConfig)
    {
        _setTimeSeconds = otherConfig._setTimeSeconds;
        _clearTimeSeconds = otherConfig._clearTimeSeconds;
        _alarmMask = otherConfig._alarmMask;
        _lowSetLimit = otherConfig._lowSetLimit;
        _lowLowSetLimit = otherConfig._lowLowSetLimit;
        _highSetLimit = otherConfig._highSetLimit;
        _highHighSetLimit = otherConfig._highHighSetLimit;
        _lowClearLimit = otherConfig._lowClearLimit;
        _lowLowClearLimit = otherConfig._lowLowClearLimit;
        _highClearLimit = otherConfig._highClearLimit;
        _highHighClearLimit = otherConfig._highHighClearLimit;
        return *this;
    }

    /**
     * Move Assignement operator.  When assigned, this class does a deep
     * copy of value and clears the other AlarmConfiguration
     *
     * @param[in] otherConfig The other AlarmConfiguration objects we take our
     * values from.
     *
     * @return Reference to this object.
     */
    AlarmConfiguration& operator=(AlarmConfiguration &&otherConfig)
    {
        _setTimeSeconds = otherConfig._setTimeSeconds;
        _clearTimeSeconds = otherConfig._clearTimeSeconds;
        _alarmMask = otherConfig._alarmMask;
        _lowSetLimit = otherConfig._lowSetLimit;
        _lowLowSetLimit = otherConfig._lowLowSetLimit;
        _highSetLimit = otherConfig._highSetLimit;
        _highHighSetLimit = otherConfig._highHighSetLimit;
        _lowClearLimit = otherConfig._lowClearLimit;
        _lowLowClearLimit = otherConfig._lowLowClearLimit;
        _highClearLimit = otherConfig._highClearLimit;
        _highHighClearLimit = otherConfig._highHighClearLimit;
        otherConfig.clear();
        return *this;
    }

    /**
     * Clears all values from this AlarmConfiguration
     */
    void clear()
    {
        _alarmMask = 0;
        _lowLowSetLimit = 0;
        _lowLowClearLimit = 0;
        _lowSetLimit = 0;
        _lowClearLimit = 0;
        _highSetLimit = 0;
        _highClearLimit = 0;
        _highHighSetLimit = 0;
        _highHighClearLimit = 0;
    }

    inline bool operator==(const AlarmConfiguration &otherConfiguration) const
    {
        return this == &otherConfiguration;
    }

    friend class AlarmConfigurationRepository;
    friend class AlarmConfigurationSerializer;
    friend class AlarmConfigurationTest;

private:
// @formatter:off
    uint8_t _alarmMask{ 0 };
    uint16_t _setTimeSeconds{ 0 };
    uint16_t _clearTimeSeconds{ 0 };
    T _lowLowSetLimit{ 0 };
    T _lowLowClearLimit{ 0 };
    T _lowSetLimit{ 0 };
    T _lowClearLimit{ 0 };
    T _highSetLimit{ 0 };
    T _highClearLimit{ 0 };
    T _highHighSetLimit{ 0 };
    T _highHighClearLimit{ 0 };
// @formatter:on

};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class AlarmInfoDelegate
{
public:
    virtual ~AlarmInfoDelegate() = default;

    virtual void collectInfo(const AlarmConfiguration<T>& alarm) = 0;
};

} /* namespace dios */

#endif /* ALARMCONFIGURATION_H_ */
