#pragma once

#ifndef __WATCHDOG_H___
#define __WATCHDOG_H___

#include <cstdint>
#include <string>

namespace zios::foundation
{

class WatchdogClient
{
public:
    virtual ~WatchdogClient() = default;

    virtual void registerThread(const std::string& name, uint32_t timeoutSeconds) = 0;
    virtual void start() = 0;
    virtual void kick() = 0;
    virtual void stop() = 0;
    virtual void close() = 0;
};

}

#endif /* __WATCHDOG_H___ */
