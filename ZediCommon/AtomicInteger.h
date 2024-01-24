/** @file AtomicInteger.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 22, 2013 by daver:
 * @endhistory
 */

#ifndef ATOMICINTEGER_H_
#define ATOMICINTEGER_H_

#include "templates.h"
#include "Mutex.h"

namespace zios {
namespace common {

/** AtomicInteger
 *
 * The AtomicInteger class provide a thread-safe integer that can be set, incremented, and decremented.
 *
 */
class AtomicInteger {
public:
    AtomicInteger();
    AtomicInteger(int value);
    ~AtomicInteger();

    /** get
    *  Returns the current value
    *
    *  @returns int - the current value
    */
    int get();

    /** get
    *  Adds 1 from the current value and returns the value.
    *
    *  @returns int - the current value after incrementing
    */
    int incrementAndGet();

    /** get
    *  Subtracts 1 from the current value and returns the value.
    *
    *  @returns int - the current value after decrementing
    */
    int decrementAndGet();

    /** get
    *  Increments the current value
    */
    void increment();

    /** get
    *  Decrements the current value
    */
    void decrement();

    /** get
    *  Sets the current value
    *
    */
    void set(int newValue);

private:
    Mutex _mutex;
    int _theInt;

    AtomicInteger(const AtomicInteger& otherAtomicInteger);
    AtomicInteger& operator=(const AtomicInteger& otherAtomicInteger);

};

}
} /* namespace zios */
#endif /* ATOMICINTEGER_H_ */
