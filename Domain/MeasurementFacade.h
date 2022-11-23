#pragma once

#ifndef __MEASUREMENTFACADE_H___
#define __MEASUREMENTFACADE_H___
#include <ctime>

#include "Measurement.h"
#include "AlarmMeister.h"
#include "Deadband.h"

namespace dios::domain
{

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class MeasurementFacade
{
public:
    enum UpdateAction
    {
        NONE, VALUE_UPDATE, ALARM_CHANGE
    };

    struct UpdateStatus
    {
        UpdateAction updateAction;
        T currentValue;
        AlarmStatus alarmStatus;
        time_t lastUpdateTime;
    };


    MeasurementFacade(IMeasurement<T>* measurement, AlarmMeister<T>* alarmMeister, Deadband<T>* deadband) :
        _measurement(measurement),
        _alarmMeister(alarmMeister),
        _deadband(deadband)
    {
    }

    ~MeasurementFacade()
    {
        delete _measurement;
        delete _alarmMeister;
        delete _deadband;
    }


    UpdateStatus updateValue(T newValue)
    {
        bool valueChanged = false;
        UpdateStatus updateStatus{NONE, 0, AlarmStatus::ALARM_STATUS_OK, 0};

        //
        // Check deadband to see if the value is worthy of an update
        //
        valueChanged = _deadband->currentValueHasChanged(_measurement->currentValue(), newValue);

        //
        // Has an alarm been exceeded?
        //
        updateStatus.alarmStatus = _alarmMeister->alarmStatusForValue(newValue);

        //
        // If either deadband has changed or alarm status has changed, then we update the
        //
        if (updateStatus.alarmStatus != _measurement->alarmStatus())
        {
            _measurement->updateCurrentValue(newValue, updateStatus.alarmStatus);
            updateStatus.updateAction = UpdateAction::ALARM_CHANGE;
        }
        else if (valueChanged)
        {
            _measurement->updateCurrentValue(newValue, updateStatus.alarmStatus);
            updateStatus.updateAction = UpdateAction::VALUE_UPDATE;
        }

        return updateStatus;
    }

private:
    IMeasurement<T>* _measurement;
    AlarmMeister<T>* _alarmMeister;
    Deadband<T>* _deadband;
};

} /* namespace dios */

#endif /* __MEASUREMENTFACADE_H___ */
