#ifndef __RUNNABLE_H___
#define __RUNNABLE_H___

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

template<typename T>
class Runnable
{
public:
    virtual ~Runnable();

    virtual T run() = 0;
};

#endif /* __RUNNABLE_H___ */
