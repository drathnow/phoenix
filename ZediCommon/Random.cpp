/** @file Random.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 31, 2014 by daver:
 * @endhistory
 */
#include <cstdlib>
#include <ctime>
#include "Random.h"

namespace zios {
namespace common {

static bool seeded = false;

Random::Random() {
    if (seeded == false) {
        seeded = true;
        srand(::time(NULL));
    }
}

Random::~Random() {
}

uint32_t Random::nextU32() {
    return (uint32_t)rand();
}

uint16_t Random::nextU16() {
    return (uint16_t)(rand() % 65535);
}

uint8_t Random::nextU8() {
    return (uint8_t)(rand() % 255);
}


} /* namespace common */
} /* namespace zios */
