#pragma once

#include <sqlite3.h>

#include "DbUpgrader.h"

#define IOPOINT_TABLENAME       "IOPoint"
#define ALARM_LIMITS_TABLENAME  "AlarmLimits"
#define DB_VERSION_TABLENAME    "DbVersion"
#define DEVICE_TABLENAME        "Device"
#define DEADBAND_TABLENAME      "Deadband"

namespace dios::persist
{

class DbUpgradeVersion0 : public DbUpgrade
{
public:
    DbUpgradeVersion0() = default;
    ~DbUpgradeVersion0() = default;

    bool doUpgrade(sqlite3 *context);

    inline int version() const
    {
        return 0;
    }
};

} /* namespace dios */
