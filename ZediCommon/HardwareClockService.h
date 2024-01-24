/** @file HardwareClockService.h
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
#ifndef HARDWARECLOCKSERVICE_H_
#define HARDWARECLOCKSERVICE_H_
#include <ctime>
#include <stdint.h>

namespace zios {
namespace common {

/**
 * The HardwareClockService is intended to be used as a singleton service to track the setting
 * of the system's hardward clock. Each time the HW clock is updated, the time is noted.  Subsequent
 * attempts to update the HW clock will only be done if a specific period of time has elapsed.
 */
class HardwareClockService {
public:
    /**
     * Set the threshold for the time that must expire before updateHardwardClockFromSystemClock
     * will force a change to the HW clock.
     */
    static const int32_t DefaultTimeSetThreadholdSeconds = 7200; // 2 hours.

    /**
     * Constructs a HardwareClockService object.
     *
     * @param timeSetThresholdSeconds - Number of second that must expire before a call to
     * didUpdateHardwardClockFromSystemClock() will result in the system time being pushed to
     * the HW clock.  The default value is DefaultTimeSetThreadholdSeconds.
     */
    HardwareClockService(int32_t timeSetThresholdSeconds = DefaultTimeSetThreadholdSeconds);
    virtual ~HardwareClockService();

    /**
     * Evaluates whether the current system time should be pushed (written) to the Hardware
     * clock. The method compares the current time to the last time the HW clock was set and
     * if the difference exceeds timeSetThresholdSeconds, the current system time will be pushed
     * to the HW clock.  If the time was never set, the current time will be pushed.
     *
     * @return 0 - if the time was adjusted. 1 - If the difference between the current time
     * and the last time the clock was adjusted has not exceeded timeSetThresholdSeconds. -1
     * if and error occurred trying to set the clock. errno contains the reason.
     */
    virtual int updateSystemTimeToHardwareClock();

    /**
     * Forces the current system time to the HW clock.
     *
     * @return 0 if the time was pushed. -1 if not. errno contains the cause.
     */
    virtual int forceSystemTimeToHardwareClock();

protected:
    time_t _lastSetTime;

private:
    int32_t _timeSetThresholdSeconds;
};

class FakeHardwareClockService : public HardwareClockService {
public:
    FakeHardwareClockService(int32_t timeSetThresholdSeconds);
    virtual ~FakeHardwareClockService();

    virtual int forceSystemTimeToHardwareClock();
};

} /* namespace zap */
} /* namespace zios */

#endif /* HARDWARECLOCKSERVICE_H_ */
