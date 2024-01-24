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
#ifndef LED_H_
#define LED_H_

#define SIG_LED             0
#define NET_LED             1
#define SRV_LED             2
#define POLL_LED            3

#define LED_OFF         0x0000
#define LED_ON          0xFFFF

#define LED_SLOW_20     1  // 1/32 s x 64 = 2 sec period, 20% duty (0x33 = 51d = 2/10 of 256)
#define LED_SLOW_10     2  // 1/32 s x 64 = 2 sec period, 10% duty (0x1a = 26d = 1/10 of 256)
#define LED_SLOW_2      3  // 1/32 s x 64 = 2 sec period, 2% duty
#define LED_MED_75      4  // 1/32 s x 16 = 0.5 sec, 75% duty
#define LED_MED_50      5  // 1/32 s x 16 = 0.5 sec, 50% duty

namespace zios {
namespace hal {

class LED
{
public:
    virtual ~LED() {}

    /**
     * Turns the LED on.
     *
     * @return 0 on success. -1 on failure. errno contains reason
     */
    virtual int turnOn() = 0;

    /**
     * Turns the LED off.
     *
     * @return 0 on success. -1 on failure. errno contains reason
     */
    virtual int turnOff() = 0;

    /**
     * Sets the flash speed for the LED. Implemenation are free to implement there own flashing speeds for
     * any LED, if it supports flashing.
     *
     * @param flashSpeed - Implementation dependant flash speed number
     *
     * @return 0 if the flash speed was set.  -1 if not. errno must be set to the cause.
     */
    virtual int setFlashSpeed(int flashSpeed) = 0;
};

} /* namespace hal */
} /* namespace zios */

#endif /* LED_H_ */
