#ifndef MOCKCLOCK_H_
#define MOCKCLOCK_H_

#include <Clock.h>
#include <gmock/gmock.h>

namespace dios::test
{

class MockClock : public dios::foundation::Clock {
public:
    MockClock() {}
    ~MockClock() {}

    MOCK_METHOD(time_t, currentTime, (), (const, override));
    MOCK_METHOD(bool, isDst, (), (const, override));
    MOCK_METHOD(int64_t, currentTimeMS, (), (const, override));

    bool operator==(const dios::foundation::Clock& rhs) const
    {
        return this == &rhs;
    }

    bool operator==(const MockClock& rhs) const
    {
        return this == &rhs;
    }
};

}

#endif /* MOCKCLOCK_H_ */
