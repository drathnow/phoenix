/** @file Hasher.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 3, 2013 by daver:
 * @endhistory
 */

#ifndef HASHER_H_
#define HASHER_H_

#include <stddef.h>
#include <stdint.h>
#include <cjson/cJSON.h>

namespace zios {
namespace common {

class Hasher {
public:
	Hasher() {}
	virtual ~Hasher() {};
	virtual void reset() = 0;
	virtual void update(const uint8_t* value, uint32_t length) = 0;
	virtual uint32_t hashedValue(uint8_t* outputBuffer, uint32_t bufferLength) = 0;
};


class Sha1Hasher : public Hasher {
public:

    Sha1Hasher(uint32_t iterations);

    /** Copy Constructor
     *
     * The Copy Constructor will only copy the iteration count for the Sha1Hasher and initialize
     * the internal buffer to its default size.  It will not copy over the contents of the interal buffer.
     * For this reason, you should only use the copy constructor to pass newly create Sha1Hasher
     * objects.
     */
    Sha1Hasher(const Sha1Hasher& otherHasher);
    ~Sha1Hasher();

    static const uint32_t INITIAL_BUFFER_SIZE;
    static const uint32_t INCREMENT_BUFFER_SIZE;
    static const uint32_t MIN_BUFFER_SIZE;

    /** Assignement Operator
     *
     * The Assignement Operator will only copy the iteration count for the Sha1Hasher and initialize
     * the internal buffer to its default size.  It will not copy over the contents of the internal buffer.
     * For this reason, you should only use the Assignement Operator to assign newly create Sha1Hasher
     * objects.
     */
    Sha1Hasher& operator=(const Sha1Hasher& otherHasher);

    void reset();
    void update(const uint8_t* value, uint32_t length);
    uint32_t hashedValue(uint8_t* outputBuffer, uint32_t bufferLength);

    uint32_t iterations() { return _iterations; }
private:
    uint32_t _iterations;
    unsigned char* _internalBuffer;
    uint32_t _currentCount;
    uint32_t _currentBufferSize;
};

class HashFactory  {
public:
    ~HashFactory() {}

    /**
     * Constructs a Hasher object from a JSON configuation.  The keys required in the JSON structure are
     * hash algorithm specific.  Every structure must contain a "Name" key.
     *
     *  SHA1
     *  {
     *      "Name": "SHA1",
     *      "IterationCount": <number>  - Number of hash iterations.
     *  }
     *
     * @param jsonConfig
     * @return
     */
    static Hasher* hasherForJsonConfig(const cJSON* jsonConfig);
private:
    HashFactory() {}
};

}
}
#endif /* HASHER_H_ */
