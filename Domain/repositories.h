#pragma once

#ifndef __REPOSITORIES_H___
#define __REPOSITORIES_H___

#include <vector>

#include "Device.h"
#include "Deadband.h"
#include "Measurement.h"
#include "AlarmConfiguration.h"

namespace dios::domain
{
    class IIOPointRepository
    {
    public:
        IIOPointRepository() = default;
        virtual ~IIOPointRepository() = default;

        virtual iopoint_id_t createIOPoint(IOPoint& ioPoint) = 0;
        virtual int updateIOPoint(const IOPoint& ioPoint) = 0;
        virtual int deleteIOPointWithOid(iopoint_id_t oid) = 0;
        virtual IOPoint* ioPointForOid(iopoint_id_t oid) = 0;
        virtual int ioPoints(std::vector<IOPoint*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
    };

    class IDeadbandRepository
    {
    public:
        IDeadbandRepository() = default;
        virtual ~IDeadbandRepository() = default;

        virtual iopoint_id_t createDeadband(deadband_t& deadband) = 0;
        virtual int updateDeadband(const deadband_t& deadband) = 0;
        virtual int deleteDeadbandWithOid(deadband_id_t oid) = 0;
        virtual deadband_t* deadbandForOid(deadband_id_t oid) = 0;
        virtual int deadbands(std::vector<deadband_id_t*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
    };

    class IDeviceRepository
    {
    public:
        IDeviceRepository() = default;
        virtual ~IDeviceRepository() = default;

        virtual iopoint_id_t createDevice(device_t& device) = 0;
        virtual int updateDevice(const device_t& device) = 0;
        virtual int deleteDeviceWithOid(device_id_t oid) = 0;
        virtual device_t* deviceForOid(device_id_t oid) = 0;
        virtual int devices(std::vector<device_t*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
    };

    class IAlarmLimitsRepository
    {
    public:
        IAlarmLimitsRepository() = default;
        virtual ~IAlarmLimitsRepository() = default;

        virtual alarm_limits_id_t createAlarmLimits(alarm_limits_t& alarm_limits) = 0;
        virtual int updateAlarmLimits(const alarm_limits_t& alarm_limits) = 0;
        virtual int deleteAlarmLimitsWithOid(alarm_limits_id_t oid) = 0;
        virtual alarm_limits_t* alarmLimitsForOid(alarm_limits_id_t oid) = 0;
        virtual int alarmLimitses(std::vector<alarm_limits_t*>& entityVector, int count, iopoint_id_t fromOid = 0) = 0;
    };
}

#endif /* __REPOSITORIES_H___ */
