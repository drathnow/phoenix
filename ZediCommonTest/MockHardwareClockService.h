/** @file MockHardwareClockService.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 31, 2015 by daver:
 * @endhistory
 */
#ifndef MOCKHARDWARECLOCKSERVICE_H_
#define MOCKHARDWARECLOCKSERVICE_H_
#include <gmock/gmock.h>
#include <HardwareClockService.h>

class MockHardwareClockService : public zios::common::HardwareClockService {
public:
    MockHardwareClockService() : HardwareClockService(0) {}
    ~MockHardwareClockService() {}

    MOCK_METHOD0(didUpdateHardwardClockFromSystemClock, bool());
    MOCK_METHOD0(forceSystemTimeToHardwareClock, int());
};



#endif /* MOCKHARDWARECLOCKSERVICE_H_ */
