#pragma once

#ifndef __DEADBAND_H___
#define __DEADBAND_H___

#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <ctime>
#include <cmath>
#include <foundation.h>

namespace dios::domain
{

using namespace dios::foundation;

enum DeadbandType
{
    None = 0, Absolute = 1, Percentage = 2
};

struct deadband
{
    int64_t oid;
    iopoint_id_t io_point_id;
    DeadbandType deadband_type;
    std::string delta;
};

using deadband_t = struct deadband;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Deadband
{
public:
    Deadband() = default;
    virtual ~Deadband() = default;

    virtual bool currentValueHasChanged(T currentValue, T newValue) const = 0;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class NoDeadband: public Deadband<T>
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
class AbsoluteDeadband: public Deadband<T>
{
public:
    AbsoluteDeadband(T changeAmount) :
            _changeAmount(changeAmount)
    {
    }

    ~AbsoluteDeadband() = default;

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
class PercentageDeadband: public Deadband<T>
{
public:
    PercentageDeadband(float changePercentage) :
            _changedPercentage(changePercentage)
    {
    }

    ~PercentageDeadband() = default;

    bool currentValueHasChanged(T currentValue, T newValue) const
    {
        T diff;
        if (newValue > currentValue)
            diff = std::fabs(newValue - currentValue);
        else
            diff = std::fabs(currentValue - newValue);
        float percent = ((float) diff / (float) currentValue) * 100.0f;
        return percent >= _changedPercentage;
    }

private:
    float _changedPercentage;
};
}

#endif /* __DEADBAND_H___ */
