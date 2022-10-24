#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Clock.h>

namespace dios::test
{

class MockClock;

bool operator==(const dios::foundation::Clock& lhs, const dios::test::MockClock& rhs)
{
    return (void*)&lhs == (void*)&rhs;
}

}
