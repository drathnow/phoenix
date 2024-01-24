/** @file Queue.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 28, 2013 by daver:
 * @endhistory
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include <map>
#include "mutexbase.h"
#include "ConditionVariable.h"
#include "Mutex.h"

namespace zios {
namespace common {

template<class T>
class QEntry {
public:
    QEntry() : _theObject(NULL), _next(NULL) {}
    QEntry(T theObject) : _theObject(theObject), _next(NULL) {}
    T _theObject;
    QEntry *_next;
};

/**
 * A BlockingQueue provides a generic queue that will block threads that attempt to remove
 * and entry from the queue when no entry is available.
 *
 */
template<class T>
class BlockingQueue {
public:
    BlockingQueue() : _queue(NULL), _size(0), _waitCount(0) {}
    virtual ~BlockingQueue() {
        while (empty() == false)
            pRemoveFront();
    }

    /**
     * Adds an entry to the back of the queue.
     *
     * @param t - Entry to added.
     */
    virtual void addBack(T t) {
        _conditionVariable.lock();
        QEntry<T>* entry = new QEntry<T>(t);
        insertBack(&_queue, entry);
        _conditionVariable.unlock();
        _conditionVariable.signal();
    }

    /**
     * Removes an entry from the front of the queue.  Threads that call this method
     * will block until an entry becomes available, the elapsed wait time expires, or another
     * thread calls wake.
     *
     * @param waitTimeMilliseconds - The amount of time, in milliseconds, to wait for an
     * entry to become availble.  Zero means wait indefinitely.
     *
     * @return T - Entry at the front of the queue. null if no entry found and either
     * the wait time expires or wake() is called by another thread.
     */
    virtual T removeFront(long waitTimeMilliseconds = 0) {
        _conditionVariable.lock();
        T value = pRemoveFront();
        if (value == NULL) {
            _waitCount++;
            _conditionVariable.wait(waitTimeMilliseconds);
            _waitCount--;
            value = pRemoveFront();
        }
        _conditionVariable.unlock();
        return value;
    }

    virtual void wait(long waitTimeMilliseconds = 0) {
        _conditionVariable.lock();
        _conditionVariable.wait(waitTimeMilliseconds);
        _conditionVariable.unlock();
    }

    /**
     * Retrieves the number of threads that are currently waiting on this queue.
     *
     * @return number of threads that are currently waiting on this queue.
     */
    virtual int waitCount() const {
        return _waitCount;
    }

    /**
     * Returns the number of entries in the queue.
     *
     * @return int - the number of entries in the queue.
     */
    virtual int size() const {
        return _size;
    }

    /**
     * Indicates if the queue is empty or not.
     *
     * @return bool - true the queue is empty, false it isn't
     */
    virtual bool empty() const {
        return size() == 0;
    }

    /**
     * Issues a broadcast on the condition variable controlling access to the queue.
     * This will cause alll threads to wake.  Upon waking, each thread will compete
     * for the associated mutex locking the queue.
     */
    virtual void wake() {
        _conditionVariable.broadcast();
    }

private:
    QEntry<T>* _queue;
    int _size;
    int _waitCount;
    ConditionVariable _conditionVariable;

    void insertBack(QEntry<T>** queue, QEntry<T>* entry) {
        if (*queue == NULL) {
            *queue = entry;
            _size++;
        } else
            insertBack(&(*queue)->_next, entry);
    }

    T pRemoveFront() {
        T value = NULL;
        if (_queue != NULL) {
            QEntry<T>* node = _queue;
            _queue = node->_next;
            value = node->_theObject;
            delete node;
            _size--;
        }
        return value;
    }
};

/**
 * Template that provides a generic queue for static typed pointers.  This
 * template will only work with pointers.
 */
template<class T, lock_policy lp = _no_lock_>
class Queue : public MutexBase<lp> {
public:
    Queue() : _queue(NULL), _size(0) {}
    virtual ~Queue() {}

    virtual void addFront(T t) {
        MutexBase<lp>::lock();
        QEntry<T>* entry = new QEntry<T>(t);
        entry->_theObject = t;
        if (_queue != NULL)
            _queue = entry;
        else {
            entry->_next = _queue;
            _queue = entry;
        }
        _size++;
        MutexBase<lp>::unlock();
    }

    virtual void addBack(T t) {
        MutexBase<lp>::lock();
        QEntry<T>* entry = new QEntry<T>(t);
        entry->_theObject = t;
        insertBack(&_queue, entry);
        MutexBase<lp>::unlock();
    }

    virtual T removeFront() {
        T value = NULL;
        MutexBase<lp>::lock();
        if (_queue != NULL) {
            QEntry<T>* node = _queue;
            _queue = node->_next;
            value = node->_theObject;
            delete node;
            _size--;
        }
        MutexBase<lp>::unlock();
        return value;
    }

    virtual int size() {
        int mySize = 0;
        MutexBase<lp>::lock();
        mySize = _size;
        MutexBase<lp>::unlock();
        return mySize;
    }

private:
    QEntry<T>* _queue;
    int _size;

    void insertBack(QEntry<T>** queue, QEntry<T>* entry) {
        if (*queue == NULL) {
            *queue = entry;
            _size++;
        } else
            insertBack(&(*queue)->_next, entry);
    }
};

}
} /* namespace zios */
#endif /* QUEUE_H_ */
