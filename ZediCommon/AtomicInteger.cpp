/** @file AtomicInteger.cpp
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

#include "AtomicInteger.h"

namespace zios {
namespace common {

AtomicInteger::AtomicInteger() : _theInt(0) {
}

AtomicInteger::AtomicInteger(int value) : _theInt(value) {
}

AtomicInteger::~AtomicInteger() {
}

int AtomicInteger::get() {
    int tmp = 0;
    _mutex.lock();
    tmp = _theInt;
    _mutex.unlock();
    return tmp;
}

int AtomicInteger::incrementAndGet() {
    int tmp = 0;
    _mutex.lock();
    tmp = ++_theInt;
    _mutex.unlock();
    return tmp;
}

int AtomicInteger::decrementAndGet() {
    int tmp = 0;
    _mutex.lock();
    tmp = --_theInt;
    _mutex.unlock();
    return tmp;
}

void AtomicInteger::increment() {
    _mutex.lock();
    ++_theInt;
    _mutex.unlock();
}

void AtomicInteger::decrement() {
    _mutex.lock();
    --_theInt;
    _mutex.unlock();
}

void AtomicInteger::set(int newValue) {
    _mutex.lock();
    _theInt = newValue;
    _mutex.unlock();
}

}
} /* namespace zios */
