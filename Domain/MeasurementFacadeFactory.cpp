#include <except.h>

#include "MeasurementFacadeFactory.h"

namespace dios::domain
{
auto* MeasurementFacadeFactory::measurementFacadeForIoPoint(const IOPoint &ioPoint, const alarm_limits_t &alarmLimits, const deadband_t &deadband)
{
    switch (ioPoint.dataType())
    {
        case DataType::DISCRETE:
        {
            auto measurement = new Measurement<bool>(ioPoint.dataType(), ioPoint.oid());
            auto noAlarmMeister = new NoAlarmMeister<bool>();
            auto noDeadband = new NoDeadband<bool>();
            return new MeasurementFacade<bool>(measurement, noAlarmMeister, noDeadband);
        }

        case DataType::INT8:
        {
            auto measurement = new Measurement<bool>(ioPoint.dataType(), ioPoint.oid());
            auto noAlarmMeister = new NoAlarmMeister<bool>();
            auto noDeadband = new NoDeadband<bool>();
            return new MeasurementFacade<bool>(measurement, noAlarmMeister, noDeadband);
            break;
        }

        case DataType::UINT8:
        {
            break;
        }

        case DataType::INT16:
        {
            break;
        }

        case DataType::UINT16:
        {
            break;
        }

        case DataType::INT32:
        {
            break;
        }

        case DataType::UINT32:
        {
            break;
        }

        case DataType::INT64:
        {
            break;
        }

        case DataType::UINT64:
        {
            break;
        }

        case DataType::FLOAT32:
        {
            break;
        }

        case DataType::FLOAT64:
        {
            break;
        }

        case DataType::STRING:
        {
            break;
        }

        case DataType::BLOB:
        {
            break;
        }

        default:
        case DataType::UNKNOWN:
        {
            THROW_EXCEPTION("Unknown data type: " + ioPoint.dataType());
        }
    }
}

} /* namespace dios */
