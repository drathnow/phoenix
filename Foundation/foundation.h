#pragma once

#include <cstdint>

namespace dios::foundation
{

enum DataType
{
    UNKNOWN = 0,    //!< UNKNOWN type (error)
    DISCRETE = 1,   //!< Boolean (0 FALSE, !0 TRUE (usually 1))
    INT8 = 2,       //!< 1 byte signed
    UINT8 = 3,      //!< 1 byte unsigned
    INT16 = 4,      //!< 2 bytes signed
    UINT16 = 5,     //!< 2 bytes unsigned
    INT32 = 6,      //!< 4 bytes signed
    UINT32 = 7,     //!< 4 bytes unsigned
    INT64 = 8,      //!< 8 bytes signed
    UINT64 = 9,     //!< 8 bytes unsigned
    FLOAT32 = 10,   //!< single precision floating point
    FLOAT64 = 11,   //!< double precision floating point
    STRING = 12,    //!< ascii string - 2 bytes length followed by [n] bytes character array. (serialized)
    BLOB = 13,      //!< binary blob - 4 bytes length followed by [n] bytes (serialized)
};

enum DeviceType
{
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_MODBUS = 1,
    DEVICE_TYPE_MODBUS_TCP = 2,
    DEVICE_TYPE_ROC = 3,
    DEVICE_TYPE_ROC_TCP = 2
};

enum IOPointType
{
    UKNOWN = 0,
    IOPOINT_TYPE_RTU = 2,
    IOPOINT_TYPE_NET = 4,
    IOPOINT_TYPE_APP = 5,
    IOPOINT_TYPE_MEM = 6,
    IOPOINT_TYPE_CFG = 7,
    IOPOINT_TYPE_INT = 3
};

enum DeadbandType
{
    DEADBAND_NONE = 0,
    DEADBAND_ABSOLUTE = 1,
    DEADBAND_PERCENTAGE = 2
};

typedef int16_t pollset_id_t;
typedef int32_t index_id_t;
typedef int64_t device_id_t;
typedef int64_t iopoint_id_t;

}
