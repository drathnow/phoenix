/** @file MockConditionVariable.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 17, 2018 by daver:
 * @endhistory
 */
#ifndef MOCKCONDITIONVARIABLE_H_
#define MOCKCONDITIONVARIABLE_H_

#include <gmock/gmock.h>
#include <ConditionVariable.h>

namespace zios {
namespace common {

class MockConditionVariable : public ConditionVariable {
public:
    ~MockConditionVariable() { destroy(); }

    MOCK_METHOD0(destroy, void());
    MOCK_METHOD0(lock, void());
    MOCK_METHOD0(unlock, void());
    MOCK_METHOD1(wait, Status(long waitTimeMilliseconds));
    MOCK_METHOD0(signal, void());

};

}
}


#endif /* MOCKCONDITIONVARIABLE_H_ */
