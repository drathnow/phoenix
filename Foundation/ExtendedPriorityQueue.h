#ifndef __EXTENDEDPRIORITYQUEUE_H_
#define __EXTENDEDPRIORITYQUEUE_H_

#pragma once

#include <queue>
#include <vector>

template<typename T>
class ExtendedPriorityQueue : public std::priority_queue<T, std::vector<T>, std::greater<T>>
{
public:
    ExtendedPriorityQueue() {}
    ~ExtendedPriorityQueue() {}

    typename std::vector<T>::iterator begin() 
    {
        return this->c.begin();
    }

    typename std::vector<T>::iterator end() 
    {
        return this->c.end();   
    }

    void erase(typename std::vector<T>::iterator& iter) 
    {
        this->c.erase(iter);
    }
};

#endif  //  __EXTENDEDPRIORITYQUEUE_H_