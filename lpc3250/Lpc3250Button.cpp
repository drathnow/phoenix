/** @file Lpc3250Button.cpp 
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
#include "Lpc3250Button.h"
#include <unistd.h>

namespace zios {
namespace lpc3250 {

Lpc3250Button::Lpc3250Button()
{
}

Lpc3250Button::~Lpc3250Button()
{
}

int Lpc3250Button::waitForButtonPress(uint32_t timeoutMilliseconds)
{
    ::usleep(timeoutMilliseconds*1000);
    return -1;
}

int Lpc3250Button::buttonState()
{
    return -1;
}

}
}
