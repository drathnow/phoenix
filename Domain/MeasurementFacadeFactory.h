#pragma once

#ifndef __MEASUREMENTFACADEFACTORY_H___
#define __MEASUREMENTFACADEFACTORY_H___

#include "Deadband.h"
#include "AlarmConfiguration.h"
#include "MeasurementFacade.h"
#include "Measurement.h"

namespace dios
{
namespace domain
{

class MeasurementFacadeFactory
{
public:
    MeasurementFacadeFactory() = default;
    ~MeasurementFacadeFactory() = default;

    auto* measurementFacadeForIoPoint(const IOPoint& ioPoint, const alarm_limits_t& alarmLimits, const deadband_t& deadban);
};

} /* namespace domain */
} /* namespace dios */

#endif /* __MEASUREMENTFACADEFACTORY_H___ */
