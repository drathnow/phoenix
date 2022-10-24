#ifndef __LOCK_H_
#define __LOCK_H_

namespace zios::foundation
{

class Lock
{
public:
    virtual ~Lock() {}

    virtual void lock() = 0;
    virtual void unlock() = 0;
};

}
#endif // __LOCK_H_
