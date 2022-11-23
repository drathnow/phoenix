#pragma once

#ifndef __MOCKMEASUREMENT_H___
#define __MOCKMEASUREMENT_H___
#include <gmock/gmock.h>
#include <Measurement.h>

namespace dios::domain
{
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    class MockMeasurement : public IMeasurement<T>
    {
    public:
        MockMeasurement(bool verifyDestroy = false) : _verifyDestroy(verifyDestroy)
        {
            if (verifyDestroy)
            {
                EXPECT_CALL(*this, destoryMockAlarmRange);
            }
        }

        ~MockMeasurement()
        {
            if (_verifyDestroy)
            {
                destoryMockAlarmRange();
            }
        }

        MOCK_METHOD(time_t, lastUpdateTime, (), (const, override));
        MOCK_METHOD(index_id_t, index, (), (const, override));
        MOCK_METHOD(DataType, dataType, (), (const, override));
        MOCK_METHOD(T, currentValue, (), (const, override));
        MOCK_METHOD(void, updateCurrentValue, (T value), (override));

        MOCK_METHOD(void, destoryMockAlarmRange, ());

    private:
        bool _verifyDestroy{false};
    };
}


#endif /* __MOCKMEASUREMENT_H___ */
