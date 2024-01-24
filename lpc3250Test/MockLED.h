/** @file MockLED.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 22, 2019 by daver:
 * @endhistory
 */
#ifndef MOCKLED_H_
#define MOCKLED_H_

#include <gmock/gmock.h>
#include <zhal.h>

namespace zios {
namespace lpc3250 {

class MockLED: public zios::hal::LED
{
public:
    MockLED(bool verifyDestroy = true) :
            _verifyDestroy(verifyDestroy)
    {
    }

    ~MockLED()
    {
        if (_verifyDestroy)
            destroyMockLED();
    }

    MOCK_METHOD0(turnOn, int());
    MOCK_METHOD0(turnOff, int());
    MOCK_METHOD1(setFlashSpeed, int(int flashSpeed));
    MOCK_METHOD0(destroyMockLED, void());

    bool _verifyDestroy;
};

}
}

#endif /* MOCKLED_H_ */
