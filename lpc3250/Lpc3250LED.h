/** @file LPC3250LED.h 
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
#ifndef LPC3250LED_H_
#define LPC3250LED_H_

#include "lpc3250.h"

#include <LED.h>
#include <stdint.h>


namespace zios {
namespace lpc3250 {

class Lpc3250Chip;

struct led_paths;

class Lpc3250LED : public zios::hal::LED
{
public:
    static const struct led_paths SIG_LED_PATHS;
    static const struct led_paths NET_LED_PATHS;


    /**
     * Used to construct an LED object that is controlled via a single chip.  These LEDs do not typically support
     * flashing.
     *
     * @param gpioLine - The gpio line number of the chip.
     */
    Lpc3250LED(gpiod_line* gpioLine);
    Lpc3250LED(const struct led_paths& ledPath);
    ~Lpc3250LED();

    int turnOn();

    int turnOff();

    /**
     * Sets the flash speed of the LED, if it supports flashing.
     *
     * @param flashSpeed - Number to indicate flash speed.  See defines for LED_SLOW_*, LED_MED_*, and LED_FAST_*
     *
     * @return 0 if the flash speed was set.  -1 if an error occurred.  errno contains the reason
     */
    int setFlashSpeed(int flashSpeed);

private:
    const struct led_paths* _ledPaths;
    gpiod_line* _gpioLine;

    int setFlashSpeed(uint32_t period, uint32_t dutyCycle);
};

} /* namespace hal */
} /* namespace zios */

#endif /* LPC3250LED_H_ */
