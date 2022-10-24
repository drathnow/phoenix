#pragma once

#ifndef __ALARMLIMIT_H___
#define __ALARMLIMIT_H___

#include <type_traits>
#include <functional>
#include <Clock.h>
#include <AlarmRange.h>

using namespace std;

template<typename T>
struct compare_function
{
    virtual ~compare_function() = default;
    virtual constexpr bool operator()(const T &lhs, const T &rhs) const = 0;
};

template<typename T>
struct greater_equal : public compare_function<T>
{
    constexpr
    bool operator()(const T &lhs, const T &rhs) const
    {
        return lhs >= rhs;
    }
};

template<typename T>
struct less_equal : public compare_function<T>
{
    constexpr
    bool operator()(const T &lhs, const T &rhs) const
    {
        return lhs <= rhs;
    }
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class ThresholdEx
{
public:
    template<typename Compare_func = ::greater_equal<T>>
    ThresholdEx(T threasholdValue, Compare_func* compare_func) : _thresholdValue(threasholdValue), _compare_func(compare_func) {}
    ~ThresholdEx() = default;

    bool isExceeded(T value) const
    {
        return (*_compare_func)(value, _thresholdValue);
    }

    T thresholdValue() const
    {
        return _thresholdValue;
    }

private:
    T _thresholdValue;
    compare_function<T>* _compare_func;
};

template<typename T, typename Compare_func = std::greater_equal<T>>
class ThresholdExx
{
public:
    ThresholdExx(T threasholdValue) : _thresholdValue(threasholdValue) {}
    ~ThresholdExx() = default;

    bool isExceeded(T value) const
    {
        return _compare_func(value, _thresholdValue);
    }

    T thresholdValue() const
    {
        return _thresholdValue;
    }

private:
    T _thresholdValue;
    Compare_func _compare_func;
};



template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
bool compare_less_equal(T lhs, T rhs)
{
    return lhs <= rhs;
}

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
bool compare_greater_equal(T lhs, T rhs)
{
    return lhs >= rhs;
}

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Threshold
{
public:
    Threshold(T threasholdValue, bool (*comp_func)(T lhs, T rhs)) : _thresholdValue(threasholdValue), _comp_func(comp_func) {}
    ~Threshold() = default;

    bool isExceeded(T value) const
    {
        return _comp_func(value, _thresholdValue);
    }

    T thresholdValue() const
    {
        return _thresholdValue;
    }

private:
    T _thresholdValue;
    bool (*_comp_func)(T lhs, T rhs);
};

#endif /* __ALARMLIMIT_H___ */
