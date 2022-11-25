#ifndef __IOPOINT_H___
#define __IOPOINT_H___

#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <ctime>
#include <cassert>
#include <foundation.h>

#include "domain.h"
#include "Deadband.h"
#include "AlarmRange.h"

namespace dios::domain
{

using namespace dios::foundation;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class IMeasurement
{
public:
    IMeasurement() = default;
    virtual ~IMeasurement() = default;

    virtual time_t lastUpdateTime() const = 0;
    virtual void updateCurrentValue(T value, AlarmStatus alarmStatus = AlarmStatus::ALARM_STATUS_OK) = 0;
    virtual iopoint_id_t ioPointId() const = 0;
    virtual DataType dataType() const = 0;
    virtual T currentValue() const = 0;
    virtual AlarmStatus alarmStatus() const = 0;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Measurement : public IMeasurement<T>
{
public:
    Measurement() = default;

    Measurement(DataType dataType, uint32_t ioPointId) noexcept :
          _dataType{ dataType },
          _currentValue{ 0 },
          _ioPointId{ ioPointId },
          _lastUpdateTime{ 0 }
    {
    }

    Measurement(Measurement<T>&& rhs) noexcept :
          _dataType{ std::move(rhs._dataType) },
          _currentValue{ std::move(rhs._currentValue) },
          _ioPointId{ std::move(rhs._ioPointId) },
          _lastUpdateTime{ std::move(rhs._lastUpdateTime) }
    {
        rhs._dataType = DataType::UNKNOWN;
        rhs._currentValue = T();
        rhs._ioPointId = 0;
    }

    Measurement<T>& operator=(Measurement<T>&& rhs) noexcept
    {
        if (this != &rhs)
        {
            _dataType = std::move(rhs._dataType);
            _currentValue = std::move(rhs._currentValue);
            _ioPointId = std::move(rhs._ioPointId);
            _lastUpdateTime = std::move(rhs._lastUpdateTime);
        }
        return *this;
    }

    ~Measurement() = default;

    inline void updateCurrentValue(T value, AlarmStatus alarmStatus = AlarmStatus::ALARM_STATUS_OK)
    {
        _currentValue = value;
        _lastUpdateTime = ::time(nullptr);
        _alarmStatus = alarmStatus;
    }

    inline time_t lastUpdateTime() const
    {
        return _lastUpdateTime;
    }

    inline iopoint_id_t ioPointId() const
    {
        return _ioPointId;
    }

    inline DataType dataType() const
    {
        return _dataType;
    }

    AlarmStatus alarmStatus() const
    {
        return _alarmStatus;
    }

    T currentValue() const
    {
        return _currentValue;
    }

    friend class MeasurementTest;

private:
    DataType _dataType{DataType::UNKNOWN};
    T _currentValue{0};
    uint32_t _ioPointId{0};
    time_t _lastUpdateTime{0};
    AlarmStatus _alarmStatus{AlarmStatus::ALARM_STATUS_OK};
};


class IOPoint
{
public:
    IOPoint() = default;
    IOPoint(const std::string& name, IOPointType pointType, DataType dataType) noexcept :
        _name(name),
        _pointType(pointType),
        _dataType(dataType)
    {
    }

    DataType dataType() const
    {
        return _dataType;
    }

    void setDataType(DataType dataType)
    {
        _dataType = dataType;
    }

    device_id_t deviceId() const
    {
        return _deviceId;
    }

    void setDeviceId(device_id_t deviceId)
    {
        _deviceId = deviceId;
    }

    const std::string& displayHint() const
    {
        return _displayHint;
    }

    void setDisplayHint(const std::string &displayHint)
    {
        _displayHint = displayHint;
    }

    const std::string& name() const
    {
        return _name;
    }

    void setName(const std::string &name)
    {
        _name = name;
    }

    iopoint_id_t oid() const
    {
        return _oid;
    }

    void setOid(iopoint_id_t oid)
    {
        _oid = oid;
    }

    IOPointType pointType() const
    {
        return _pointType;
    }

    void setPointType(IOPointType pointType)
    {
        _pointType = pointType;
    }

    bool isReadonly() const
    {
        return _readonly;
    }

    void setReadonly(bool readonly)
    {
        _readonly = readonly;
    }

    const std::string& sourceAddress() const
    {
        return _sourceAddress;
    }

    void setSourceAddress(const std::string &sourceAddress)
    {
        _sourceAddress = sourceAddress;
    }

    bool isSystem() const
    {
        return _system;
    }

    void setSystem(bool system)
    {
        _system = system;
    }



private:
    iopoint_id_t _oid{0};
    std::string _name;
    IOPointType _pointType{IOPointType::UKNOWN};
    DataType _dataType{DataType::UNKNOWN};
    device_id_t _deviceId{0};
    bool _readonly{false};
    bool _system{false};
    std::string _sourceAddress;
    std::string _displayHint;
};

}

#endif /* __IOPOINT_H___ */
