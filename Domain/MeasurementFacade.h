#pragma once

#ifndef __MEASUREMENTFACADE_H___
#define __MEASUREMENTFACADE_H___

#include "deadband.h"
#include "Measurement.h"
#include "AlarmLimitMeister.h"

namespace dios::domain
{

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class MeasurementFacade
{
public:
    MeasurementFacade(Measurement<T>* measurement, AlarmLimitMeister<T>* alarmLimitMeister, deadband<T>* deadband) :
        _measurement(measurement),
        _alarmLimitMeister(alarmLimitMeister),
        _deadband(deadband)
    {
    }

    ~MeasurementFacade()
    {
        delete _measurement;
        delete _alarmLimitMeister;
        delete _deadband;
    }

private:
    Measurement<T>* _measurement;
    AlarmLimitMeister<T>* _alarmLimitMeister;
    deadband<T>* _deadband;
};

} /* namespace dios */

#endif /* __MEASUREMENTFACADE_H___ */
