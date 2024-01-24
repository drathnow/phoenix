/** @file Hasher.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 21, 2014 by daver:
 * @endhistory
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "Hasher.h"
#include "sha1.h"

namespace zios {
namespace common {

using namespace std;

const uint32_t Sha1Hasher::INITIAL_BUFFER_SIZE = 100u;
const uint32_t Sha1Hasher::INCREMENT_BUFFER_SIZE = 100u;
const uint32_t Sha1Hasher::MIN_BUFFER_SIZE = 20u;

static const uint8_t* NAME_KEY = (const uint8_t*)"Name";
static const uint8_t* SHA1_HASH_NAME = (const uint8_t*)"SHA1";
static const uint8_t* ITERATION_COUNT = (const uint8_t*)"IterationCount";

Sha1Hasher::Sha1Hasher(uint32_t iterations) :
        _iterations(iterations),
        _internalBuffer(new unsigned char[INITIAL_BUFFER_SIZE]),
        _currentCount(0),
        _currentBufferSize(INITIAL_BUFFER_SIZE) {
}

Sha1Hasher::~Sha1Hasher() {
    delete [] _internalBuffer;
}

Sha1Hasher::Sha1Hasher(const Sha1Hasher& otherHasher) :
                    _iterations(otherHasher._iterations),
                    _internalBuffer(new unsigned char[INITIAL_BUFFER_SIZE]),
                    _currentCount(0),
                    _currentBufferSize(INITIAL_BUFFER_SIZE) {
}

Sha1Hasher& Sha1Hasher::operator=(const Sha1Hasher& otherHasher) {
    _iterations = otherHasher._iterations;
    _internalBuffer = new unsigned char[INITIAL_BUFFER_SIZE];
    _currentCount = 0;
    _currentBufferSize = INITIAL_BUFFER_SIZE;
    return *this;
}

void Sha1Hasher::reset() {
    _currentCount = 0;
}

void Sha1Hasher::update(const uint8_t* value, uint32_t length) {
    if (_currentCount + length > _currentBufferSize) {
        _currentBufferSize += INCREMENT_BUFFER_SIZE;
        unsigned char* tmp = new unsigned char[_currentBufferSize];
        ::memcpy((void*)tmp, _internalBuffer, _currentCount);
        delete [] _internalBuffer;
        _internalBuffer = tmp;
    }
    ::memcpy(&_internalBuffer[_currentCount], value, length);
    _currentCount += length;
}

uint32_t Sha1Hasher::hashedValue(uint8_t* outputBuffer, uint32_t bufferLength) {
    unsigned char tmpBuffer[MIN_BUFFER_SIZE];
    sha1::calc((void*)_internalBuffer, _currentCount, tmpBuffer);
    for (uint32_t i = 1; i < _iterations; i++)
        sha1::calc((void*)tmpBuffer, MIN_BUFFER_SIZE, tmpBuffer);
    ::memcpy(outputBuffer, tmpBuffer, bufferLength);
    return bufferLength;
}


static Hasher* sha1HashForJsonConfig(const cJSON* jsonConfig) {
    cJSON* iterItem = cJSON_GetObjectItem((cJSON*)jsonConfig, (const char*)ITERATION_COUNT);
    if (iterItem == NULL)
        return NULL;
    return new Sha1Hasher(iterItem->valueint);
}

Hasher* HashFactory::hasherForJsonConfig(const cJSON* jsonConfig) {
    cJSON* nameEl = cJSON_GetObjectItem((cJSON*)jsonConfig, (const char*)NAME_KEY);
    if (nameEl == NULL)
        return NULL;
    if (::strcasecmp((char*)SHA1_HASH_NAME, nameEl->string))
        return sha1HashForJsonConfig(jsonConfig);
    return NULL;
}


}
}
