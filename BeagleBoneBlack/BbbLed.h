#include <cstdint>

#pragma once

namespace dios::foundation
{

class BbbLed
{
public:

    enum TriggerMode
    {
        none,
        nand_disk,
        usb_gadget,
        usb_host,
        mmc0,
        mmc1,
        timer,
        oneshot,
        heartbeat,
        backlight,
        gpio,
        default_on,
    };

    BbbLed(int number, const char* name);
    ~BbbLed();

    BbbLed(const BbbLed&) = delete;
    BbbLed(BbbLed&&) = delete;
    BbbLed& operator=(const BbbLed&) = delete;
    BbbLed& operator==(BbbLed&&) = delete;

    void turnOn();
    void turnOff();
    void setFlash(int freqencyMilliseconds, int durationMilliseconds);

private:
    uint16_t _number;
    char* _name;
    char* _brightnessPath;
};

}
