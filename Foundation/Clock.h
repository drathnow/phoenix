#ifndef CLOCK_H_
#define CLOCK_H_

#include <ctime>
#include <cstdint>

namespace zios::foundation
{

class Clock
{
public:
    virtual ~Clock() {}

    virtual time_t currentTime() const = 0;
    virtual bool isDst() const = 0;
	virtual clock_t currentTimeMS(void) const = 0;
};

class SystemClock : public Clock
{
public:
    static SystemClock globalSystemTime;

	SystemClock();
	~SystemClock();

    time_t currentTime() const;
    bool isDst() const;
    clock_t currentTimeMS(void) const;
};

class MonotonicClock : public Clock
{
public:
    static MonotonicClock globalMonotonicClock;

    MonotonicClock();
	~MonotonicClock();

    time_t currentTime() const;
    bool isDst() const;
    clock_t currentTimeMS(void) const;
};

}

#endif /* CLOCK_H_ */
