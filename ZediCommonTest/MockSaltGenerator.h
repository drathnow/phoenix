/** @file MockSaltGenerator.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 9, 2013 by daver:
 * @endhistory
 */

#ifndef MOCKSALTGENERATOR_H_
#define MOCKSALTGENERATOR_H_
#include <gmock/gmock.h>
#include <SaltGenerator.h>

namespace zios {
namespace common {

class MockSaltGenerator : public SaltGenerator {
public:
    MockSaltGenerator() : _salt(NULL), _length(0) {}
    ~MockSaltGenerator() {
        delete [] _salt;
    }

    void generateSalt(uint8_t* buffer, uint32_t bufferLength) {
        SaltGenerator::generateSalt(buffer, bufferLength);
           _length = bufferLength;
           if (_salt != NULL)
               delete [] _salt;
           _salt = new uint8_t[_length];
           ::memcpy((void*)_salt, buffer, _length);
    }

    const uint8_t* _salt;
    uint32_t _length;
};
}
}
#endif /* MOCKSALTGENERATOR_H_ */
