#ifndef __BBB_H___
#define __BBB_H___

#define DELIMITER_CHAR          '_'

#define GPIO_DIR_PATH           "/sys/class/gpio"
#define GPIO_EXPORT_PATH        "/sys/class/gpio/export"
#define GPIO_PATH_FMT           "/sys/class/gpio/gpio%d"
#define GPIO_VALUE_FMT          "/sys/class/gpio/gpio%d/value"
#define GPIO_ACTIVE_LOW_FMT     "/sys/class/gpio/gpio%d/active_low"
#define GPIO_DIRECTION_FMT      "/sys/class/gpio/gpio%d/direction"

#define LED_DIR_PATH            "/sys/class/leds"
#define LED_PATH_FMT            "/sys/class/leds/beaglebone:green:usr%d"
#define LED_BRIGHTNESS_FMT      "/sys/class/leds/beaglebone:green:usr%d/brightness"

typedef struct pin
{
    const char* cape;
    int number;
} pin_t;

#endif /* __BBB_H___ */
