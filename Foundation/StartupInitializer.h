#pragma once

#ifndef STARTUPINITIALIZER_H_
#define STARTUPINITIALIZER_H_

namespace dios::foundation
{

class AppManager;

class StartupInitializer
{
public:
    virtual ~StartupInitializer() = default;

    virtual void registerWithAppManager(AppManager &appManager) = 0;

    virtual void initialize() = 0;
    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual const char* name() = 0;
};

}

#endif /* STARTUPINITIALIZER_H_ */
