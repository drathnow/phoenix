#pragma once

#ifndef __TESTUTIL_H___
#define __TESTUTIL_H___

namespace dios::foundation
{
    class Clock;
}
namespace dios::test
{

class MockClock;

bool operator==(const dios::foundation::Clock& lhs, const dios::test::MockClock& rhs);

}

#endif /* __TESTUTIL_H___ */
