#ifndef ALARMSPUNGE_H_
#define ALARMSPUNGE_H_

#include <ctime>
#include <type_traits>
#include <Clock.h>
#include <cstdint>

#include "AlarmConfiguration.h"

namespace dios::domain
{


enum AlarmStatus {
    ALARM_STATUS_OK = 0x00,
    ALARM_STATUS_HIGH_HIGH = 0x0C,
    ALARM_STATUS_HIGH = 0x14,
    ALARM_STATUS_LOW_LOW = 0x18,
    ALARM_STATUS_LOW = 0x20,
};

enum AlarmType {
    ALARM_TYPE_HIGH_HIGH = 0x0C,
    ALARM_TYPE_HIGH = 0x14,
    ALARM_TYPE_LOW_LOW = 0x18,
    ALARM_TYPE_LOW = 0x20
};

enum AlarmState {
    CLEAR_STATE = 0x00,
    SET_PENDING_STATE = 0x01,
    ALARM_STATE = 0x02,
    CLEAR_PENDING_STATE = 0x03
};


template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
bool compare_greater_equal(T lhs, T rhs)
{
    return lhs >= rhs;
}

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
bool compare_less_equal(T lhs, T rhs)
{
    return lhs <= rhs;
}

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Threshold
{
public:
    Threshold() = default;
    Threshold(T threasholdValue, bool (*comp_func)(T lhs, T rhs)) : _thresholdValue(threasholdValue), _comp_func(comp_func) {}

    Threshold(const Threshold<T>& other) :
        _thresholdValue(other._thresholdValue),
        _comp_func(other._comp_func)
    {
    }
    ~Threshold() = default;

    bool isExceeded(T value) const
    {
        return _comp_func(value, _thresholdValue);
    }

    T thresholdValue() const
    {
        return _thresholdValue;
    }

    bool operator<=(const Threshold& rhs) const
    {
        return _thresholdValue <= rhs._thresholdValue;
    }

    bool operator>=(const Threshold& rhs) const
    {
        return _thresholdValue >= rhs._thresholdValue;
    }

private:
    T _thresholdValue{T()};
    bool (*_comp_func)(T lhs, T rhs){nullptr};
};


/*
* Explanation of the AlarmMask:
*
* <------- 1 Byte -------->
* 7                       0
* +--+--+--+--+--+--+--+--+
* |  |  |  |  |  |  |  |  |
* +--+--+--+--+--+--+--+--+
*        <- Type  ->  <   >
*                 ^    ^
*                 |    |
*                 |   State
*                 |
*                Range (Set = High, Unset = Low)
*
*  Type - Defines the type of alarm we are monitoring HIGH_HIGH, HIGH, etc.
*  Range - What range are we monitoring. Set indicates high, unset indicates low. BIt is part
*          of the AlarmType enum value.
*  State - The current state we are in
*/

const char ALARM_STATE_MASK = 0x03;
const char RANGE_MASK = 0x04;
const char ALARM_TYPE_MASK = 0x3C;

struct alarm_state
{
    char state;

    uint8_t alarmState()
    {
        return state & ALARM_STATE_MASK;
    }

    void setAlarmState(uint8_t state)
    {
        state |= state;
    }

    uint8_t alarmType()
    {
        return state & ALARM_TYPE_MASK;
    }

    void setAlarmType(uint8_t type)
    {
        state |= type;
    }

    uint8_t alarmStatus()
    {
        return state & ALARM_TYPE_MASK;
    }

    void setlarmStatus(uint8_t status)
    {
        state |= status;
    }
};

using alarm_state_t = alarm_state;

/**
 * An AlarmRange defines a range of values that determine if a values is in an alarm condition.  The
 * range is defined by two value: a SetLimit and a ClearLimit.  The relationship between these two
 * values depends on whether the Alarm Range is for an Upper or Lower alarm boundary. The SetLimit
 * defines the upper/lower most value for the alarm range. If an value goes above/below this value
 * the value is considered to be in an alarm condition. Once the value exceeds the SetLimit, it will
 * remain in an alarm condition until the value goes below/above the ClearLimit.  A simple graph for
 * an upper limit:
 *
 *         Value
 *           ^
 *           |
 *         + |------------- Set Limit
 *  Alarm  | |
 *  Range  | |
 *         + |------------- Clear Limit
 *           |
 *           V
 *
 * To prevent situations where a value might go in and out of alarm because of slight changes in
 * value around the SetLimit or ClearLimit, clients can specify a SetTime and ClearTime.  The SetTime
 * defines the number of seconds a value must be above the SetLimit before the value will be
 * considered to be in an alarm condition. Similarly, the ClearTime defines the number of seconds a
 * value must be below/above the ClearLimit before the value is considered to be out of an alarm
 * state.  The comparison functions used for these values is:
 *
 *    +--------------+-------------+-------------+
 *    | Type         |  Set Limit  | Clear Limit |
 *    +--------------+-------------+-------------+
 *    |  Upper Range |      >=     |     <       |
 *    |  Lower Range |      <=     |     >       |
 *    +--------------+-------------+-------------+
 *
 * An AlarmRange is implemented as a state machine with four states:
 *
 *      CLEAR - No alarm
 *
 *      SET_PENDING - A value has exceeded a SetLimit but the amount of time (seconds) specified
 *      by setTimeSeconds has not elapased.
 *
 *      ALARM - The value is in an alarm condition but the amount of time (seconds) specified
 *      by clearTimeSeconds has not elapased.
 *
 *      CLEAR_PENDING - The value has exceeded ClearLimit bu
 *
 * Alarm Ranges can be combined in a hierarchy, so that values can be compared over multiple ranges
 * to give different alarm statuses depending on the value.
 *
 *   +-------------+       +-------------+       +-------------+
 *   | Alarm Range |------>| Alarm Range |------>| Alarm Range |
 *   +-------------+       +-------------+       +-------------+
 *
 */

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class IAlarmRange
{
public:
    IAlarmRange() = default;
    virtual ~IAlarmRange() = default;

    virtual AlarmStatus alarmStatusForValue(T value, dios::foundation::Clock& clock, uint16_t setTimeSeconds, uint16_t clearTimeSeconds) = 0;
    virtual bool isValid() const = 0;
    virtual void clear() = 0;
    virtual bool isGreaterThan(const IAlarmRange<T>& otherRange) const = 0;
    virtual bool isLessThan(const IAlarmRange<T>& otherRange) const = 0;
    virtual bool isActive() const = 0;
    virtual void addValuesToAlarmConfiguration(AlarmConfiguration<T>& alarmConfiguration) const = 0;

    virtual T highestSetLimit() const = 0;
    virtual T lowestClearLimit() const = 0;
    virtual T setLimit() const = 0;
    virtual T clearLimit() const = 0;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class AlarmRange : public IAlarmRange<T>
{
public:
    /**
     * Constructs an AlarmRange object.
     *
     * @param[in] setLimit - The SetLimit that defines the boundary where a value is considered to be in alarm.  This is a transfer
     * of ownership;
     *
     * @param[in] clearLimit - The ClearLimit that defines the boundary where a value is considered no longer in alarm. This is a transfer
     * of ownership.
     *
     * @param[in] monitoredAlarmType - The type of range this is.  Either ALARM_TYPE_HIGH_HIGH or ALARM_TYPE_HIGH will
     * indicate this is an "upper" alarm range.  ALARM_TYPE_LOW_LOW or ALARM_TYPE_LOW indicates a lower alarm range.
     *
     * @param[in] nextRange -The next, subordinate range.
     */
    AlarmRange(Threshold<T> setLimit, Threshold<T> clearLimit, AlarmType monitoredAlarmType, IAlarmRange<T>* nextRange = NULL) :
        _nextRange(nextRange),
        _setLimitThreshold(setLimit),
        _clearLimitThreshold(clearLimit)
    {
        _alarmState = monitoredAlarmType;
    }

    ~AlarmRange()
    {
        delete _nextRange;
    }

    AlarmRange(const AlarmRange& rhs) = delete;
    AlarmRange(AlarmRange&& rhs) = delete;
    AlarmRange& operator=(const AlarmRange& rhs) = delete;
    AlarmRange& operator=(AlarmRange&& rhs) = delete;

    AlarmStatus alarmStatusForValue(T value, dios::foundation::Clock &clock, uint16_t setTimeSeconds, uint16_t clearTimeSeconds)
    {
        AlarmStatus higherStatus = ALARM_STATUS_OK;

        switch (_alarmState & ALARM_STATE_MASK)
        {
            case CLEAR_STATE:
            {
                if (_setLimitThreshold.isExceeded(value))
                {
                    //
                    // If we have a higher range, we need to pass the value on.
                    //
                    if (_nextRange != NULL)
                        higherStatus = _nextRange->alarmStatusForValue(value, clock, setTimeSeconds, clearTimeSeconds);

                    //
                    // If we have a set time, then we move to SET_PENDING state, otherwise we go directly to ALARM state
                    // and return the either our alarm status or the higher range status.  Higher range status always has
                    // precidence over ours.
                    //
                    if (setTimeSeconds == 0)
                    {
                        _alarmState |= ALARM_STATE;

                        //
                        // If we have a higher range, then pass the value up and return its status if it's not OK.
                        //
                        if (higherStatus == ALARM_STATUS_OK)
                            higherStatus = _monitoredStatus();
                    } else
                    {
                        //
                        // If we have a set time, we need to transition and start a timer
                        //
                        _startTime = clock.currentTime();
                        _alarmState |= SET_PENDING_STATE;
                    }
                }
                break;
            }

            case SET_PENDING_STATE:
            {
                if (_setLimitThreshold.isExceeded(value))
                {
                    //
                    // Pass the value to the next range, if we have one.
                    //
                    if (_nextRange != NULL)
                        higherStatus = _nextRange->alarmStatusForValue(value, clock, setTimeSeconds, clearTimeSeconds);

                    //
                    // Check to see how long we've been here.  If the set time has expired, we move to ALARM state
                    // and return the higher range status if it's not OK, or ours.
                    //
                    if ((clock.currentTime() - _startTime) >= setTimeSeconds)
                    {
                        _transitionToState(ALARM_STATE);
                        higherStatus = (higherStatus == ALARM_STATUS_OK) ? _monitoredStatus() : higherStatus;
                    }
                } else
                {
                    //
                    // Our value has returned to normal range, so clear all pending status.
                    //
                    clear();
                }
                break;
            }

            case ALARM_STATE:
            {
                //
                // We alway pass to higher range, if we have one.
                //
                if (_nextRange != NULL)
                    higherStatus = _nextRange->alarmStatusForValue(value, clock, setTimeSeconds, clearTimeSeconds);

                //
                // Are we still above set limit?
                //
                if (_setLimitThreshold.isExceeded(value) == false && _clearLimitThreshold.isExceeded(value))
                {
                    //
                    // We are below set limit, and below clear limit.  If we have no clear time, then dismiss (clear)
                    // the alarm
                    //
                    if (clearTimeSeconds == 0)
                    {
                        clear();
                        higherStatus = ALARM_STATUS_OK;
                    } else
                    {
                        //
                        // Start a Timer;
                        //
                        _startTime = clock.currentTime();
                        _transitionToState(CLEAR_PENDING_STATE);
                        higherStatus = higherStatus == ALARM_STATUS_OK ? _monitoredStatus() : higherStatus;
                    }
                } else
                {
                    higherStatus = higherStatus == ALARM_STATUS_OK ? _monitoredStatus() : higherStatus;
                }
                break;
            }

            case CLEAR_PENDING_STATE:
            {
                //
                // We alway pass to higher range, if we have one.
                //
                if (_nextRange != NULL)
                    higherStatus = _nextRange->alarmStatusForValue(value, clock, setTimeSeconds, clearTimeSeconds);

                //
                // If we exceed the clear limit, then we have to see how long we've been here
                //
                if (_clearLimitThreshold.isExceeded(value))
                {
                    if ((clock.currentTime() - _startTime) >= clearTimeSeconds)
                    {
                        clear();
                        return ALARM_STATUS_OK;
                    }
                    higherStatus = higherStatus == ALARM_STATUS_OK ? _monitoredStatus() : higherStatus;
                } else
                {
                    //
                    // If we don't exceed the clear limit, we return to Alarm
                    //
                    _transitionToState(ALARM_STATE);
                    higherStatus = higherStatus == ALARM_STATUS_OK ? _monitoredStatus() : higherStatus;
                }
                break;
            }
        }
        return higherStatus;
    }

    /**
     * Clears this alarm range by putting it back to a CLEAR state
     */
    void clear()
    {
       if (_nextRange != NULL)
           _nextRange->clear();
       _transitionToState(CLEAR_STATE);
    }

    /**
     * Indicates if this alarm range is active. A range is active if it is not in a CLEAR state.
     * @return true if range is active
     */
    bool isActive() const
    {
        return (_alarmState & ALARM_STATE_MASK) != CLEAR_STATE;
    }

    /**
     * Whether a range is valid depends on whether it is an upper or lower range, and how it
     * relates to any subranges. When the method is called, it will traverse the AlarmRange
     * graph and make the following comparison:
     *
     *  An Upper Range is valid if:
     *
     *      SetLimit >= Clear Limit
     *      AND
     *      subordinateRange > this range (see ">" operator)
     *
     *  An Lower Range is valid if:
     *
     *      SetLimit <= Clear Limit
     *      AND
     *      subordinateRange < this range (see "<" operator)
     *
     * @return true - the range is valid.  false otherwise.
     */
    bool isValid() const
    {
       bool valid;
       if (0 != (_alarmState & RANGE_MASK)) {
           valid = _setLimitThreshold >= _clearLimitThreshold;
           if (_nextRange != NULL) {
               valid = valid && _nextRange->isValid() && (_nextRange->isGreaterThan(*this));
           }
       } else {
           valid = _setLimitThreshold <= _clearLimitThreshold;
           if (_nextRange != NULL)
               valid = valid && _nextRange->isValid() && (_nextRange->isLessThan(*this));
       }
       return valid;
   }

    /**
     * Equality operator.  Since this class cannot be copied, the comparison
     * depends on the memory address of each object.
     *
     * @param[in] otherRange - The other AlarmRange
     * @return true if they are the same object.  falso othersise.
     */
    bool operator==(const AlarmRange& otherRange) const
    {
        return this == &otherRange;
    }

    /**
     * Compares this range to another to make sure that its bounds
     * are greater than the other range's bounds:
     *
     * @param[in] otherRange
     * @return
     */
    inline bool isGreaterThan(const IAlarmRange<T>& otherRange) const
    {
        return _setLimitThreshold.thresholdValue() > otherRange.setLimit()
                && _setLimitThreshold.thresholdValue() > otherRange.clearLimit()
                && _clearLimitThreshold.thresholdValue() >= otherRange.setLimit()
                && _clearLimitThreshold.thresholdValue() >= otherRange.clearLimit();
     }


    /**
     * Compares this range to another to make sure that its bounds
     * are less than the other range's bounds:
     *
     * @param[in] otherRange
     * @return
     */
    inline bool isLessThan(const IAlarmRange<T>& otherRange) const
    {
       return _setLimitThreshold.thresholdValue() < otherRange.setLimit()
               && _setLimitThreshold.thresholdValue() < otherRange.clearLimit()
               && _clearLimitThreshold.thresholdValue() <= otherRange.setLimit()
               && _clearLimitThreshold.thresholdValue() <= otherRange.clearLimit();
    }

    inline void addValuesToAlarmConfiguration(AlarmConfiguration<T> &alarmConfiguration) const
    {
        switch (_alarmState & ALARM_TYPE_MASK)
        {
        case ALARM_TYPE_HIGH_HIGH:
            alarmConfiguration.setHighHighLimits(_setLimitThreshold.thresholdValue(), _clearLimitThreshold.thresholdValue());
            break;
        case ALARM_TYPE_HIGH:
            alarmConfiguration.setHighLimits(_setLimitThreshold.thresholdValue(), _clearLimitThreshold.thresholdValue());
            break;
        case ALARM_TYPE_LOW_LOW:
            alarmConfiguration.setLowLowLimits(_setLimitThreshold.thresholdValue(), _clearLimitThreshold.thresholdValue());
            break;
        case ALARM_TYPE_LOW:
            alarmConfiguration.setLowLimits(_setLimitThreshold.thresholdValue(), _clearLimitThreshold.thresholdValue());
            break;
        }

        if (NULL != _nextRange)
            _nextRange->addValuesToAlarmConfiguration(alarmConfiguration);
    }

    /**
     * Will return the highest SetLimit for this alarm range and it's subranges.  This term
     * "highest" is used to the maxium or minimum value depending on whether this is
     * a HIGH or LOW range. To determine the highest Set Limit, this method will travers
     * the AlarmRange graph until it reaches an end node and return its SetLimit value.
     *
     * @return Reference to a Value
     */
    inline T highestSetLimit() const
    {
       if (_nextRange == NULL)
           return _setLimitThreshold.thresholdValue();
       return _nextRange->highestSetLimit();
    }

    /**
     * Returns the lowest Clear Limit value for this Alarm Range.
     * @return
     */
    inline T lowestClearLimit() const
    {
       return _clearLimitThreshold.thresholdValue();
    }

    inline T setLimit() const
    {
        return _setLimitThreshold.thresholdValue();
    }

    inline T clearLimit() const
    {
        return _clearLimitThreshold.thresholdValue();
    }

    friend class AlarmRangeTest;

private:
    unsigned char currentState() const;
    T _setLimit{T()};
    T _clearLimit{T()};
    time_t _startTime{0};
    IAlarmRange<T>* _nextRange{nullptr};
    unsigned char _alarmState{0};
    Threshold<T> _setLimitThreshold;
    Threshold<T> _clearLimitThreshold;

    inline AlarmStatus _monitoredStatus() const
    {
       return static_cast<AlarmStatus>(_alarmState & ALARM_TYPE_MASK);
    }

    inline void _transitionToState(char newState)
    {
       _alarmState &= ~ALARM_STATE_MASK;
       _alarmState |= newState;
    }
};

} /* namespace dios */

#endif /* ALARMSPUNGE_H_ */
