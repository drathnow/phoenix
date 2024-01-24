/** @file SaltGenerator.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 3, 2013 by daver:
 * @endhistory
 */

#include <time.h>
#include "SaltGenerator.h"

namespace zios {
namespace common {


SaltGenerator::SaltGenerator() {
    ::srand(::time(NULL));
}

SaltGenerator::~SaltGenerator() {
}

void SaltGenerator::generateSalt(uint8_t* buffer, uint32_t bufferLength) {
    for (uint32_t i = 0; i < bufferLength; i++)
        buffer[i] = rand() % 255;
}

}
} /* namespace zios */
