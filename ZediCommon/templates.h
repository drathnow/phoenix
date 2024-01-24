/** @file templates.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created ul 30, 2013 by daver:
 * @endhistory
 */

#ifndef TEMPLATES_H_
#define TEMPLATES_H_

#include "stdio.h"
#include <istream>
#include <sstream>
#include <string>

namespace zios {
namespace common {

/**
 * Will convert a string representation of a numberic value to its numeric value.
 *
 * The conversion is dictated by the rules of C++ stream parsers.  So if you specify a
 * string of "41234fooE-2" you will not recieve and error.  Rather you will get the value
 * 41234 back.
 *
 * @param strval - A string representation of a value
 * @param retValue - The place to return the value
 * @return true if the value was translated to the template type. false otherwise.
 */
template<class T>
bool valueFromString(const std::string& strval, T& retValue) {
    std::istringstream iss(strval);
    return (iss >> retValue).fail() == 0;
}

template<class T>
class ScopedMallocPtr {
public:
    ScopedMallocPtr(T* val) : _val(val) {}
    ~ScopedMallocPtr() { free(_val); }
    T* _val;
};


template<class T>
class ScopedArrayPtr {
public:
    ScopedArrayPtr(T* array) : _array(array) {}
    ~ScopedArrayPtr() { delete [] _array; }
    T* _array;
};

template<class Lockable>
class ScopedLock {
public:
    ScopedLock(Lockable & lock) :
            _lock(lock) {
        _lock.lock();
    }

    ~ScopedLock() {
        _lock.unlock();
    }
private:
    Lockable& _lock;
};

template<class Closeable>
class ScopedCloser {
public:
    ScopedCloser(Closeable* closeable) :
            _closeable(closeable) {
    }

    ~ScopedCloser() {
        if (_closeable != NULL)
            _closeable->close();
    }

    void clear() {
        _closeable = NULL;
    }

private:
    Closeable* _closeable;
};

template<class Something>
class ThreadLocal {
public:
    ThreadLocal(Something someThing) :
            _someThing(someThing) {
    }
    ~ThreadLocal() {
    }

    inline Something get() const {
        return _someThing;
    }
    inline void set(Something someThing) const {
        _someThing = someThing;
    }

private:
    Something _someThing;
};

}
}
#endif /* TEMPLATES_H_ */
