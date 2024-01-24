/** @file Lpc3250Button.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 5, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250BUTTON_H_
#define LPC3250BUTTON_H_

#include <zhal.h>

namespace zios {
namespace lpc3250 {

class Lpc3250Button : public zios::hal::Button
{
public:
    Lpc3250Button();
    ~Lpc3250Button();

    int waitForButtonPress(uint32_t timeoutMilliseconds);

    int buttonState();
};

}
}
#endif /* LPC3250BUTTON_H_ */
