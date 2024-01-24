#ifndef BUTTON_H__
#define BUTTON_H__

#include <stdint.h>

#define WAKE_BUTTON_NUMBER          1

namespace zios {
namespace hal {

class Button {
public:
    enum STATE {
        PRESSED, NOT_PRESSED
    };

    virtual ~Button() {}

    virtual int waitForButtonPress(uint32_t timeoutMilliseconds) = 0;

    virtual int buttonState() = 0;
};

}
}

#endif
