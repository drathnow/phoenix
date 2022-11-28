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

class AlarmMeisterFactory;
class DeadbandFactory;

class MeasurementFacadeFactory
{
public:
    MeasurementFacadeFactory(AlarmMeisterFactory* alarmMeisterFactory, DeadbandFactory* deadbandFactory);
    ~MeasurementFacadeFactory();

    template <typename T>
    MeasurementFacade<T>* measurementFacadeForIoPoint(const IOPoint& ioPoint, const alarm_limits_t& alarmLimits, const deadband_t& deadban);

private:
    AlarmMeisterFactory* _alarmMeisterFactory;
    DeadbandFactory* _deadbandFactory;
};

} /* namespace domain */
} /* namespace dios */

#endif /* __MEASUREMENTFACADEFACTORY_H___ */
