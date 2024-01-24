/** @file LED.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 26, 2019 by daver:
 * @endhistory
 */
#ifndef MOCKLED_H_
#define MOOCKLED_H_

#include <gmock/gmock.h>
#include <zhal.h>

namespace zios {
namespace hal {

class MockLED : public LED
{
public:
    MOCK_METHOD0(turnOn, int());
    MOCK_METHOD0(turnOff, int());
    MOCK_METHOD1(setFlashSpeed, int(int flashSpeed));
};

} /* namespace hal */
} /* namespace zios */

#endif /* MOCKLED_H_ */
