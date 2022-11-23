#pragma once

#ifndef __MOCKDEADBAND_H___
#define __MOCKDEADBAND_H___

#include <gmock/gmock.h>
#include <Deadband.h>

namespace dios::domain
{
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    class MockDeadband : public Deadband<T>
    {
    public:
        MockDeadband(bool verifyDestroy = false) : _verifyDestroy(verifyDestroy)
        {
            if (verifyDestroy)
            {
                EXPECT_CALL(*this, destoryMockAlarmRange);
            }
        }

        ~MockDeadband()
        {
            if (_verifyDestroy)
            {
                destoryMockAlarmRange();
            }
        }

        MOCK_METHOD(bool, currentValueHasChanged, (T currentValue, T newValue), (const, override));

        MOCK_METHOD(void, destoryMockAlarmRange, ());

    private:
        bool _verifyDestroy{false};

    };
}



#endif /* __MOCKDEADBAND_H___ */
