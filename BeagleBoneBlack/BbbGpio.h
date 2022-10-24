#ifndef __BBBGPIO_H___
#define __BBBGPIO_H___

#include <gpio.h>
#include "bbb.h"

namespace zios::bbb
{

class BbbGpio: public zios::hal::gpio
{
public:
    ~BbbGpio();

    BbbGpio(const BbbGpio&) = delete;
    BbbGpio(BbbGpio&&) = delete;
    BbbGpio& operator=(const BbbGpio&) = delete;
    BbbGpio& operator==(BbbGpio&&) = delete;

    const char* label();

    const char* name();

    int value();

    void setValue(int value);

    bool isActiveLow();

    void setActiveLow(bool isActiveLow);

    bool isReadOnly();

    void setDirection(zios::hal::gpio::Direction direction);

    zios::hal::gpio::Direction direction() const;

    static BbbGpio* createGpio(pin_t pin, const char* name, const char* label);

    friend class BbbGpioTest;

private:
    BbbGpio(pin_t pin, const char* name, const char* label) noexcept;

    pin _pin;
    char *_name;
    char *_label;
    bool _activeLow;
    char* _valuePath;

};

} /* namespace zios */

#endif /* __BBBGPIO_H___ */
