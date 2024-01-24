/** @file LPC3250LED.cpp 
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
#include "Lpc3250LED.h"

#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <gpiod.h>
#include <cstring>
#include <Exception.h>
#include <cstdio>

namespace zios {
namespace lpc3250 {

using namespace zios::common;

struct led_paths
{
    const char *exportPath;
    const char *periodPath;
    const char *dutyCyclePath;
    const char *enablePath;
};

struct led_chip
{
    const char *chipname;
    uint8_t lineNumber;
};

const struct led_paths Lpc3250LED::SIG_LED_PATHS = {
        "/sys/class/pwm/pwmchip0/export",
        "/sys/class/pwm/pwmchip0/pwm0/period",
        "/sys/class/pwm/pwmchip0/pwm0/duty_cycle",
        "/sys/class/pwm/pwmchip0/pwm0/enable"
};

const struct led_paths Lpc3250LED::NET_LED_PATHS = {
        "/sys/class/pwm/pwmchip1/export",
        "/sys/class/pwm/pwmchip1/pwm0/period",
        "/sys/class/pwm/pwmchip1/pwm0/duty_cycle",
        "/sys/class/pwm/pwmchip1/pwm0/enable"
};


static int write_value_to_file(uint32_t value, const char *path)
{
    FILE *file = NULL;
    int len = 0;

    if (NULL == (file = ::fopen(path, "w")))
        return -1;

    if (0 == (len = ::fprintf(file, "%d", value)))
        return -1;

    ::fclose(file);

    return 0;
}


Lpc3250LED::Lpc3250LED(const struct led_paths &ledPaths) :
        _ledPaths(&ledPaths),
                _gpioLine(NULL)
{
    FILE *file;
    file = ::fopen(_ledPaths->exportPath, "w");
    assert(file != NULL);
    int len = ::fprintf(file, "0");
    assert(len > 0);
    ::fclose(file);
}

Lpc3250LED::Lpc3250LED(gpiod_line *gpioLine) :
        _ledPaths(NULL),
                _gpioLine(gpioLine)
{

}

Lpc3250LED::~Lpc3250LED()
{
    if (_gpioLine != NULL)
    {
        gpiod_chip *chip = ::gpiod_line_get_chip(_gpioLine);
        if (chip != NULL)
            ::gpiod_chip_close(chip);
    }
}

int Lpc3250LED::turnOn()
{
    return setFlashSpeed(LED_ON);
}

int Lpc3250LED::turnOff()
{
    if (_ledPaths != NULL)
        write_value_to_file(0, _ledPaths->enablePath);
    else
        return ::gpiod_line_set_value(_gpioLine, 0);
    return 0;
}

int Lpc3250LED::setFlashSpeed(int flashSpeed)
{
    if (_ledPaths != NULL)
    {
        switch (flashSpeed)
        {
            case LED_OFF:
                return turnOff();

            case LED_ON:
                return setFlashSpeed(1000000000, 1000000000);
                break;

            case LED_SLOW_20:   // 1/32 s x 64 = 2 sec period, 20% duty (0x33 = 51d = 2/10 of 256)
                return setFlashSpeed(2000000000, 400000000);

            case LED_SLOW_10:   // 1/32 s x 64 = 2 sec period, 10% duty (0x1a = 26d = 1/10 of 256)
                return setFlashSpeed(200000000, 20000000);

            case LED_SLOW_2:    // 1/32 s x 64 = 2 sec period, 2% duty
                return setFlashSpeed(2000000000, 40000000);

            case LED_MED_75:    // 1/32 s x 16 = 0.5 sec, 75% duty
                return setFlashSpeed(500000000, 375000000);

            case LED_MED_50:    // 1/32 s x 16 = 0.5 sec, 50% duty
                return setFlashSpeed(500000000, 250000000);

            default:
                errno = EINVAL;
                break;
        }

    }
    else
    {
        if (flashSpeed == LED_ON)
        {
            return ::gpiod_line_set_value(_gpioLine, 1);
        }
        else if (flashSpeed == LED_OFF)
            turnOff();
        errno = ENOTSUP;
    }
    return -1;

}

int Lpc3250LED::setFlashSpeed(uint32_t period, uint32_t dutyCycle)
{
    write_value_to_file(0, _ledPaths->enablePath);
    write_value_to_file(period, _ledPaths->periodPath);
    write_value_to_file(dutyCycle, _ledPaths->dutyCyclePath);
    write_value_to_file(1, _ledPaths->enablePath);

    return 0;
}

} /* namespace hal */
} /* namespace zios */
