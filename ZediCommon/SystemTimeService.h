/** @file SystemTimeService.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 30, 2015 by daver:
 * @endhistory
 */
#ifndef SYSTEMTIMESERVICE_H_
#define SYSTEMTIMESERVICE_H_
#include <stdint.h>
#include <ctime>
#include <log4cplus/logger.h>

namespace zios {
namespace common {

class HardwareClockService;

class SystemTimeService {
public:
    /**
     *
     * @param hardwareClockService - Pointer to HardwareClockService.  This is NOT a transfer of ownership.
     *
     */
    SystemTimeService(HardwareClockService* hardwareClockService);
    virtual ~SystemTimeService();

    /**
     * When called, this method will use the linxu \::adjtime() system function to adjust
     * the time gradually.
     *
     * @param deltaSeconds - The delta time to adjust the clock by.  Could be positive or negative.
     *
     * @return 0 for success, -1 on failure. errno contains the reason.
     */
    virtual int adjustTime(time_t deltaSeconds);

    /**
     * Sets the current system time of day.  This method will adjust both the System Time
     * and the HW clock.
     *
     * @param toTime - To to update the system clock to.
     *
     * @return 0 for success, -1 on failure. errno contains the reason.
     */
    virtual int updateTime(time_t toTime);

    /**
     * Pushes the curren time to the hardware clock without effecting the currrent
     * system time;
     *
     * @return 0 for success, -1 on failure. errno contains the reason.
     */
    virtual int updateSystemTimeToHardwareClock();

protected:
    HardwareClockService* _hardwareClockService;

    static log4cplus::Logger _logger;
};

/**
 * Since developer probably wouldn't want to actually adjust the time, this
 * fake object is provided.
 *
 */
class FakeSystemTimeService : public SystemTimeService {
public:
    FakeSystemTimeService(HardwareClockService* hardwareClockService);
    virtual ~FakeSystemTimeService();

    virtual int adjustTime(int32_t deltaSeconds);
    virtual int updateTime(time_t toTime);

    static log4cplus::Logger _logger;
};

} /* namespace zap */
} /* namespace zios */

#endif /* SYSTEMTIMESERVICE_H_ */
