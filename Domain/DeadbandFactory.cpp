#include <StringHelper.h>
#include <except.h>

#include "DeadbandFactory.h"

namespace dios::domain
{

using namespace dios::foundation;

template<typename T>
Deadband<T>* DeadbandFactory::deadbandForDeadbandParams(const deadband_t &deadbandParams) const
{
    switch (deadbandParams.deadband_type)
    {
        case DeadbandType::None:
            return new NoDeadband<T>();

        case DeadbandType::Absolute:
            return new AbsoluteDeadband<T>(deadbandParams.delta);

        case DeadbandType::Percentage:
            return new PercentageDeadband<T>(deadbandParams.delta);

        default:
            THROW_EXCEPTION("Invalid deadband type: " << deadbandParams.deadband_type);
    }
}


} /* namespace dios */
