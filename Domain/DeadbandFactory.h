#pragma once

#ifndef __DEADBANDFACTORY_H___
#define __DEADBANDFACTORY_H___

#include "Deadband.h"

namespace dios
{
namespace domain
{

class DeadbandFactory
{
public:
    DeadbandFactory() = default;
    ~DeadbandFactory() = default;

    template<typename T>
    Deadband<T>* deadbandForDeadbandParams(const deadband_t &deadbandParams) const;
};

} /* namespace domain */
} /* namespace dios */

#endif /* __DEADBANDFACTORY_H___ */
