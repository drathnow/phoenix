#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Clock.h>

namespace zios::test
{

class MockClock;

bool operator==(const zios::foundation::Clock& lhs, const zios::test::MockClock& rhs)
{
    return (void*)&lhs == (void*)&rhs;
}

}
