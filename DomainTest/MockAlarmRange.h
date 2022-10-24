#pragma once

#ifndef __MOCKALARMRANGE_H___
#define __MOCKALARMRANGE_H___

#include <gmock/gmock.h>
#include <AlarmRange.h>
#include <type_traits>
#include <Clock.h>

namespace dios::domain
{

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class MockAlarmRange : public IAlarmRange<T>
{
public:
    MockAlarmRange(bool verifyDestroy = false) : _verifyDestroy(verifyDestroy)
    {
        if (verifyDestroy)
        {
            EXPECT_CALL(*this, destoryMockAlarmRange);
        }
    }

    ~MockAlarmRange()
    {
        if (_verifyDestroy)
        {
            destoryMockAlarmRange();
        }
    }

    MOCK_METHOD(AlarmStatus, alarmStatusForValue, (T value, dios::foundation::Clock& clock, uint16_t setTimeSeconds, uint16_t clearTimeSeconds), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(bool, isValid, (), (const, override));
    MOCK_METHOD(bool, isActive, (), (const, override));
    MOCK_METHOD(bool, isGreaterThan, (const IAlarmRange<T>& otherRange), (const override));
    MOCK_METHOD(bool, isLessThan, (const IAlarmRange<T>& otherRange), (const override));
    MOCK_METHOD(T, highestSetLimit, (), (const, override));
    MOCK_METHOD(T, lowestClearLimit, (), (const, override));
    MOCK_METHOD(T, setLimit, (), (const override));
    MOCK_METHOD(T, clearLimit, (), (const override));
    MOCK_METHOD(void, addValuesToAlarmConfiguration, (AlarmConfiguration<T> &alarmConfiguration), (const override));

    MOCK_METHOD(void, destoryMockAlarmRange, ());

private:
    bool _verifyDestroy{false};
};

}


#endif /* __MOCKALARMRANGE_H___ */
