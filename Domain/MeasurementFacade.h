#pragma once

#ifndef __MEASUREMENTFACADE_H___
#define __MEASUREMENTFACADE_H___
#include <ctime>

#include "Measurement.h"
#include "AlarmMeister.h"
#include "Deadband.h"
#include "ReportItem.h"

namespace dios::domain
{
enum UpdateAction
{
    NONE, VALUE_UPDATE, ALARM_CHANGE
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class MeasurementFacade
{
public:
    struct UpdateStatus
    {
        UpdateAction updateAction;
        T currentValue;
        AlarmStatus alarmStatus;
        time_t lastUpdateTime;
    };

    MeasurementFacade(IMeasurement<T> *measurement, AlarmMeister<T> *alarmMeister, Deadband<T> *deadband) :
            _measurement(measurement), _alarmMeister(alarmMeister), _deadband(deadband)
    {
    }

    ~MeasurementFacade()
    {
        delete _measurement;
        delete _alarmMeister;
        delete _deadband;
    }

    ReportItem<T> reportItem() const
    {
        return _measurement->reportItem();
    }

    UpdateAction updateValue(T newValue)
    {
        UpdateAction result = NONE;
        bool valueChanged = false;

        //
        // Check deadband to see if the value is worthy of an update
        //
        valueChanged = _deadband->currentValueHasChanged(_measurement->currentValue(), newValue);

        //
        // Has an alarm been exceeded?
        //
        AlarmStatus alarmStatus = _alarmMeister->alarmStatusForValue(newValue);

        //
        // If either deadband has changed or alarm status has changed, then we update the
        //
        if (alarmStatus != _measurement->alarmStatus())
        {
            result = UpdateAction::ALARM_CHANGE;
            _measurement->updateCurrentValue(newValue, alarmStatus);
        } else if (valueChanged)
        {
            result = UpdateAction::VALUE_UPDATE;
            _measurement->updateCurrentValue(newValue, alarmStatus);
        }

        return result;
    }

private:
    IMeasurement<T> *_measurement;
    AlarmMeister<T> *_alarmMeister;
    Deadband<T> *_deadband;
};

} /* namespace dios */

#endif /* __MEASUREMENTFACADE_H___ */
