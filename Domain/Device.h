#pragma once

#ifndef DEVICE_H_
#define DEVICE_H_

#include <foundation.h>
#include <cstdint>
#include <string>

namespace dios::domain
{
using namespace dios::foundation;

struct device
{
    device_id_t oid;
    DeviceType device_type;
    std::string name;
    uint16_t max_retries;
    uint16_t request_timeout_seconds;
    uint16_t rtu_backoff_timeout_seconds;
    uint16_t rtu_backoff_count;
    std::string address;
    std::string parameters;
    std::string extended_parameters;
};

using device_t = struct device;

enum ConnectionStatus
{
    Busy, Disconnected, Connected
};
class Device
{
public:
    Device();
    ~Device();



private:
    std::string _name;
    DeviceType _deviceType;
    uint16_t _requestTimeoutSeconds;
    uint16_t rtuBackoffTimeoutSeconds;
    uint16_t rtuBackoffCount;
};

} /* namespace dios */

#endif /* DEVICE_H_ */
