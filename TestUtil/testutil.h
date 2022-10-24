#pragma once

#ifndef __TESTUTIL_H___
#define __TESTUTIL_H___

namespace zios::foundation
{
    class Clock;
}
namespace zios::test
{

class MockClock;

bool operator==(const zios::foundation::Clock& lhs, const zios::test::MockClock& rhs);

}

#endif /* __TESTUTIL_H___ */
