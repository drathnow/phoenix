#ifndef MOCKCLOCK_H_
#define MOCKCLOCK_H_

#include <Clock.h>
#include <gmock/gmock.h>

namespace zios::test
{

class MockClock : public zios::foundation::Clock {
public:
    MockClock() {}
    ~MockClock() {}

    MOCK_METHOD(time_t, currentTime, (), (const, override));
    MOCK_METHOD(bool, isDst, (), (const, override));
    MOCK_METHOD(int64_t, currentTimeMS, (), (const, override));

    bool operator==(const zios::foundation::Clock& rhs)
    {
        return this == &rhs;
    }
};

}

#endif /* MOCKCLOCK_H_ */
