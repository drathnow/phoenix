#pragma once

#ifndef __MOCKALARMMEISTER_H___
#define __MOCKALARMMEISTER_H___
#include <gmock/gmock.h>
#include <AlarmMeister.h>

namespace dios::domain
{
    template<typename T>
    class MockAlarmMeister : public AlarmMeister<T>
    {
    public:
        MockAlarmMeister(bool verifyDestroy = false)
        {
            if (verifyDestroy)
            {
                EXPECT_CALL(*this, destoryMockAlarmMeister);
            }
        }
        ~MockAlarmMeister()
        {
            if (_verifyDestroy)
            {
                destoryMockAlarmMeister();
            }
        }

        MOCK_METHOD(AlarmStatus, alarmStatusForValue, (T value), (override));
        MOCK_METHOD(void, destoryMockAlarmMeister, ());

    private:
        bool _verifyDestroy{false};

    };
}




#endif /* __MOCKALARMMEISTER_H___ */
