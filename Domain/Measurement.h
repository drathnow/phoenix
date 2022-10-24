#ifndef __IOPOINT_H___
#define __IOPOINT_H___

#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <ctime>
#include <cassert>

#include "deadband.h"
#include "domain.h"

namespace zios::domain
{

class IMeasurement
{
public:
    IMeasurement() = default;
    virtual ~IMeasurement() = default;

    virtual time_t lastUpdateTime() const = 0;
    virtual index_id_t index() const = 0;
    virtual DataType dataType() const = 0;
};

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Measurement : public IMeasurement
{
public:
    Measurement() = default;

    Measurement(DataType dataType, uint32_t index, std::string&& sourceAddress) noexcept :
          _dataType{ dataType },
          _currentValue{ 0 },
          _index{ index },
          _lastUpdateTime{ 0 }
    {
    }

    Measurement(Measurement<T>&& rhs) noexcept :
          _dataType{ std::move(rhs._dataType) },
          _currentValue{ std::move(rhs._currentValue) },
          _index{ std::move(rhs._index) },
          _lastUpdateTime{ std::move(rhs._lastUpdateTime) }
    {
        rhs._dataType = DataType::UNKNOWN;
        rhs._currentValue = T();
        rhs._index = 0;
    }

    Measurement<T>& operator=(Measurement<T>&& rhs) noexcept
    {
        if (this != &rhs)
        {
            _dataType = std::move(rhs._dataType);
            _currentValue = std::move(rhs._currentValue);
            _index = std::move(rhs._index);
            _lastUpdateTime = std::move(rhs._lastUpdateTime);
        }
        return *this;
    }

    ~Measurement() = default;

    inline void updateCurrentValue(const T& value)
    {
        _currentValue = value;
        _lastUpdateTime = ::time(nullptr);
    }

    inline time_t lastUpdateTime() const
    {
        return _lastUpdateTime;
    }

    inline index_id_t index() const
    {
        return _index;
    }

    inline DataType dataType() const
    {
        return _dataType;
    }

    const T& currentValue() const
    {
        return _currentValue;
    }

    friend class MeasurementTest;

private:
    DataType _dataType{DataType::UNKNOWN};
    T _currentValue{0};
    uint32_t _index{0};
    time_t _lastUpdateTime{0};
};


class IOPoint
{
public:
    IOPoint(index_id_t index, pollset_id_t pollsetId) noexcept :
        _index(index),
        _pollsetId(pollsetId)
    {
    }

    index_id_t index() const
    {
        return _index;
    }

private:
    index_id_t _index;
    pollset_id_t _pollsetId;
    std::string _sourceAddress;
};

}

#endif /* __IOPOINT_H___ */
