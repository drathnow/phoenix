#pragma once

#ifndef __PERSIST_H___
#define __PERSIST_H___

#include <string>
#include <sstream>
#include <sqlite3.h>
#include <foundation.h>
#include <log4cplus/log4cplus.h>

namespace dios::persist
{
    using namespace log4cplus;
    using namespace dios::foundation;

    struct io_point
    {
        int64_t oid;
        std::string name;
        IOPointType io_point_type;
        DataType data_type;
        device_id_t device_id;
        bool readonly;
        bool system;
        std::string source_address;
        std::string display_hint;
    };



using io_point_t = struct io_point;

}

#endif /* __PERSIST_H___ */
