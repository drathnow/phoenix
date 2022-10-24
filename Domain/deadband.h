#pragma once

#ifndef __DEADBAND_H___
#define __DEADBAND_H___

#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <ctime>
#include <cmath>

namespace dios::domain
{

    enum DeadbandType
    {
        None = 0, Absolute = 1, Percentage = 2
    };

typedef struct
{
    DeadbandType type;
    float value;
} deadband_params_t;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class deadband
{
public:
    deadband() = default;
    virtual ~deadband() = default;

    virtual bool currentValueHasChanged(T currentValue, T newValue) const = 0;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class NoDeadband : public deadband<T>
{
public:
    NoDeadband() = default;
    ~NoDeadband() = default;

    bool currentValueHasChanged(T currentValue, T newValue) const
    {
        return true;
    }
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class absolute_deadband : public deadband<T>
{
public:
    absolute_deadband(T changeAmount) :
        _changeAmount(changeAmount)
    {
    }

    ~absolute_deadband() = default;

    bool currentValueHasChanged(T currentValue, T newValue) const
    {
        if (newValue > currentValue)
            return std::fabs(newValue - currentValue) >= _changeAmount;
        else
            return std::fabs(currentValue - newValue) >= _changeAmount;
    }

private:
    T _changeAmount;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class percentage_deadband : public deadband<T>
{
public:
    percentage_deadband(float changePercentage) :
        _changedPercentage(changePercentage)
    {
    }

    ~percentage_deadband() = default;

    bool currentValueHasChanged(T currentValue, T newValue) const
    {
        T diff;
        if (newValue > currentValue)
            diff = std::fabs(newValue - currentValue);
        else
            diff = std::fabs(currentValue - newValue);
        float percent = ((float)diff / (float)currentValue) * 100.0f;
        return percent >= _changedPercentage;
    }

private:
    float _changedPercentage;
};
}

#endif /* __DEADBAND_H___ */
