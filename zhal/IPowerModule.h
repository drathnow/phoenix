/** @file IPowerModule.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb. 27, 2019 by daver:
 * @endhistory
 */
#ifndef IPOWERMODULE_H_
#define IPOWERMODULE_H_

#include <stdint.h>
#include <cstddef>
#include <vector>

namespace zios {
namespace hal {


class IPowerModule {
public:

    /**
     * status of power module returned by isEnabled.
     */
    enum POWER_STATE {
        POWER_OFF = 0,    //!< POWER_OFF
        POWER_PENDING = 1,//!< pending, witing for time delay before on...
        POWER_ON = 2,     //!< POWER_ON
    };

    virtual ~IPowerModule() {}

    /**
     * retrieve name from power module
     * @return name of power module
     */
    virtual const char* name() const = 0;

    /**
     * retrieve pin name from power module
     * @return pin name of power module
     */
    virtual const char* label() const = 0;

    /**
     * return status of power supply fault
     * @retval true if power module fault condition
     * @retval false if power module not fault condition
     */
    virtual bool isFaulted() const = 0;

    /**
     *
     * @retval true if internal power supply
     * @retval false if not.
     */
    virtual bool isInternalPowerSupply() const = 0;

    /**
     * @return value of enable delay (msec)
     */
    virtual int32_t enableDelayMilliseconds() const = 0;

    /**
     * Sets the number of milliseconds to wait between turning on a
     * power module and when it is considered ready to use.
     *
     * @param delayMilliseconds - Number of millisceconds
     */
    virtual void setEnableDelayMilliseconds(int32_t delayMilliseconds) = 0;

    /**
     *
     * @return number of ranges defined for this
     */
    virtual size_t rangeCount() const = 0;

    /**
     * Sets the range by selection code
     * if => rangeCount then will default to highest range
     * @param range (1 based)
     * @return selection code selected, 0 means no change, set failed.
     */
    virtual uint32_t setSelectRange(uint32_t range) = 0;

    /**
     * return current range selection code.
     * Note if no selector specified, it will return 0
     * @retval selected range
     * @retval 0 if no selector specified in module.
     */
    virtual uint32_t selectedRange() const = 0;

    /**
     * will return the current voltage range selection
     * eg: 5, 12, 24
     * @return current range value
     */
    virtual uint32_t currentRangeValue() const = 0;

    /**
     * Turns this power module on.
     *
     * @return
     *      POWER_ON        - Power has been turned on, or was already on
     *      POWER_PENDING   - Power has been turned on but we must wait for power to stablize
     *      POWER_OFF       - Power is off (if did not turn on)
     */
    virtual POWER_STATE turnOn() = 0;

    /**
     * Turns this power module on.
     *
     * @return
     *      POWER_ON        - Power has been turned on
     *      POWER_PENDING   - Power has been turned on but we must wait for power to stablize
     *      POWER_OFF       - Power is off (if did not turn on)
     */
    virtual POWER_STATE turnOff() = 0;

    /**
     * Indicates if this PowerModule is currently on.  A power module is on if
     * its current state is POWER_ON state.
     *
     * @return true if in POWER_ON state, false if its in either POWER_PENDING or POWER_OFF
     */
    virtual bool isOn() = 0;


    /**
     * Everything below this line is @deprecated
     */
    virtual POWER_STATE powerState() = 0;
};

} /* namespace hal */
} /* namespace zios */

#endif /* IPOWERMODULE_H_ */
