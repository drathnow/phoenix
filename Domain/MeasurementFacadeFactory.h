#pragma once

#ifndef __MEASUREMENTFACADEFACTORY_H___
#define __MEASUREMENTFACADEFACTORY_H___

#include "MeasurementFacade.h"
#include "deadband.h"
#include "Measurement.h"

namespace zios
{
namespace domain
{

class MeasurementFacadeFactory
{
public:
    MeasurementFacadeFactory() = default;
    ~MeasurementFacadeFactory() = default;

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    MeasurementFacade<T> measurementFacadeForIoPoint(const IOPoint& ioPoint, const AlarmConfiguration<T>& alarmConfiguration, const deadband_params_t& deadbandParams)
    {

    }
};

} /* namespace domain */
} /* namespace zios */

#endif /* __MEASUREMENTFACADEFACTORY_H___ */
