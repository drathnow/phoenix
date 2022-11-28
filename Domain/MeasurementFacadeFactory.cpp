#include <except.h>

#include "MeasurementFacadeFactory.h"
#include "AlarmMeisterFactory.h"
#include "DeadbandFactory.h"

namespace dios::domain
{

MeasurementFacadeFactory::MeasurementFacadeFactory(AlarmMeisterFactory *alarmMeisterFactory, DeadbandFactory *deadbandFactory) :
        _alarmMeisterFactory(alarmMeisterFactory), _deadbandFactory(deadbandFactory)
{
}

MeasurementFacadeFactory::~MeasurementFacadeFactory()
{
    delete _alarmMeisterFactory;
    delete _deadbandFactory;
}

template<typename T>
MeasurementFacade<T>* MeasurementFacadeFactory::measurementFacadeForIoPoint(const IOPoint &ioPoint, const alarm_limits_t &alarmLimits, const deadband_t &deadbandParams)
{
    AlarmMeister<T> *alarmMeister = nullptr;
    Deadband<T> *deadband = nullptr;
    Measurement<T> *measurement = nullptr;

    measurement = new Measurement<T>(ioPoint.dataType(), ioPoint.oid());

    switch (ioPoint.dataType())
    {
        case DataType::DISCRETE:
        case DataType::INT8:
        case DataType::UINT8:
        {
            alarmMeister = new NoAlarmMeister<T>();
            deadband = new NoDeadband<T>();
            break;
        }

        case DataType::INT16:
        case DataType::UINT16:
        case DataType::INT32:
        case DataType::UINT32:
        case DataType::INT64:
        case DataType::UINT64:
        case DataType::FLOAT32:
        case DataType::FLOAT64:
        {
            alarmMeister = _alarmMeisterFactory->alarmMeisterForAlarmLimits<T>(alarmLimits);
            deadband = _deadbandFactory->deadbandForDeadbandParams<T>(deadbandParams);
            break;
        }

        case DataType::BLOB:
        case DataType::STRING:
        {
            delete measurement;
            THROW_EXCEPTION("Cannot build measurement facade for type: " + ioPoint.dataType());
            break;
        }

        default:
        case DataType::UNKNOWN:
        {
            delete measurement;
            THROW_EXCEPTION("Unknown data type: " + ioPoint.dataType());
        }

        return new MeasurementFacade<T>(measurement, alarmMeister, deadband);
    }
}

} /* namespace dios */
