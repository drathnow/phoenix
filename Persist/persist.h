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
        uint64_t oid;
        std::string name;
        index_id_t index;
        IOPointType io_point_type;
        DataType data_type;
        device_id_t device_id;
        bool readonly;
        bool system;
        std::string source_address;
        std::string display_hint;

        std::string insert_statement() const
        {
            std::ostringstream oss;
            oss << "insert into IOPoint (name, io_point_type, data_type, device_id, is_readonly, is_system, source_address, display_hint) values ('"
                    << name << "', "
                    << io_point_type << ", "
                    << data_type << ", "
                    << device_id << ", "
                    << readonly << ", "
                    << system << ", '"
                    << source_address << "', '"
                    << (display_hint.empty() ? "NULL" : display_hint) << "')";
            return oss.str();
        }

        std::string update_statement() const
        {
            std::ostringstream oss;
            oss << "update IOPoint set name = '" << name << "'"
                    << " index = " << index
                    << " io_point_type = " << io_point_type
                    << " data_type = " << data_type
                    << " device_id = " << device_id
                    << " is_readonly = " << readonly
                    << " system = " << system
                    << " source_address = '" << source_address << "' "
                    << " display_hint = '" << (display_hint.empty() ? "NULL" : display_hint)
                    << " where oid = " << oid;
            return oss.str();
        }

        std::string delete_statement() const
        {
            std::ostringstream oss;
            oss << "delete from IOPoint where oid = " << oid;
            return oss.str();
        }
    };

    struct alarm_limits
    {
        uint64_t oid;
        uint64_t io_point_id;
        bool no_data_enabled;
        uint16_t set_time_seconds;
        uint16_t clear_time_seconds;
        std::string low_low_set_limit;
        std::string low_low_clear_limit;
        std::string low_set_limit;
        std::string low_clear_limit;
        std::string high_set_limit;
        std::string high_clear_limit;
        std::string high_high_set_limit;
        std::string high_high_clear_limit;

        std::string insert_statement() const
        {
            std::ostringstream oss;
            oss << "insert into AlarmLimits (io_point_id, set_time, clear_time, no_data_enabled, high_high_set_limit, high_high_clear_limit, high_set_limit, high_clear_limit, low_low_set_limit, low_low_clear_limit, low_set_limit, low_clear_limit) values ("
                    << io_point_id <<  ", "
                    << set_time_seconds <<  ", "
                    << clear_time_seconds <<  ", "
                    << no_data_enabled <<  ", '"
                    << high_high_set_limit <<  "', '"
                    << high_high_clear_limit <<  "', '"
                    << high_set_limit <<  "', '"
                    << high_clear_limit <<  "', '"
                    << low_low_set_limit <<  "', '"
                    << low_low_clear_limit <<  "', '"
                    << low_set_limit <<  "', '"
                    << low_clear_limit << "')";
            return oss.str();
        }

        std::string update_statement() const
        {
            std::ostringstream oss;
            oss << "update AlarmLimits "
                    << " set io_point_id = " << io_point_id
                    << " set set_time = " << set_time_seconds
                    << " set clear_time = " << clear_time_seconds
                    << " set no_data_enabled = " << no_data_enabled
                    << " set high_high_set_limit = '" << high_high_set_limit << "'"
                    << " set high_high_clear_limit = '" << high_high_clear_limit << "'"
                    << " set high_set_limit = '" << high_set_limit << "'"
                    << " set high_clear_limit = '" << high_clear_limit << "'"
                    << " set low_low_set_limit = '" << low_low_set_limit << "'"
                    << " set low_low_clear_limit = '" << low_low_clear_limit << "'"
                    << " set low_set_limit = '" << low_set_limit << "'"
                    << " set low_clear_limit = '" << low_clear_limit << "'"
                    << " where oid = " << oid;
            return oss.str();
        }

        std::string delete_statement() const
        {
            std::ostringstream oss;
            oss << "delete from AlarmLimits where oid = " << oid;
            return oss.str();
        }
    };

    struct device
    {
        uint64_t oid;
        DeviceType device_type;
        std::string name;
        uint16_t max_retries;
        uint16_t request_timeout_seconds;
        uint16_t rtu_backoff_timeout_seconds;
        uint16_t rtu_backoff_count;
        std::string address;
        std::string parameters;
        std::string extended_parameters;

        std::string insert_statement() const
        {
            std::ostringstream oss;
            oss << "insert into device (device_type, name, max_retries, request_timeout_sec, rtu_backoff_timeout_sec, rtu_backoff_count, address, parameters, extended_parameters) values ("
                    << device_type << ", '"
                    << name << "', "
                    << max_retries << ", "
                    << request_timeout_seconds << ", "
                    << rtu_backoff_timeout_seconds << ", "
                    << rtu_backoff_count << ", '"
                    << address << "', "
                    << (parameters.empty() ? "NULL" : ("'" + parameters + "'")) << ", "
                    << (extended_parameters.empty() ? "NULL" : ("'" + extended_parameters + "'"))
                    << ")";
            return oss.str();
        }

        std::string update_statement() const
        {
            std::ostringstream oss;
            oss << "update device "
                    << " set device_type = " << device_type
                    << " set name = '" << name << "'"
                    << " set max_retries = " << max_retries
                    << " set request_timeout_seconds = " << request_timeout_seconds
                    << " set rtu_backoff_timeout_seconds = " << rtu_backoff_timeout_seconds
                    << " set rtu_backoff_count = " << rtu_backoff_count
                    << " set address = " << address
                    << " set parameters = " << (parameters.empty() ? "NULL" : ("'" + parameters + "'"))
                    << " set extended_parameters = " << (extended_parameters.empty() ? "NULL" : ("'" + extended_parameters + "'"))
                    << " where oid = " << oid;
            return oss.str();
        }

        std::string delete_statement() const
        {
            std::ostringstream oss;
            oss << "delete from device where oid = " << oid;
            return oss.str();
        }
    };

    struct deadband
    {
        uint64_t oid;
        uint64_t io_point_id;
        DeadbandType deadband_type;
        std::string delta;


        std::string insert_statement() const
        {
            std::ostringstream oss;
            oss << "insert into deadband (io_point_id, deadband_type, delta) values ("
                    << io_point_id << ", '"
                    << deadband_type << "', "
                    << delta
                    << ")";
            return oss.str();
        }

        std::string update_statement() const
        {
            std::ostringstream oss;
            oss << "update deadband "
                    << " set io_point_id = " << io_point_id
                    << " set deadband_type = " << deadband_type
                    << " set delta = '" << delta << "'"
                    << " where oid = " << oid;
            return oss.str();
        }

        std::string delete_statement() const
        {
            std::ostringstream oss;
            oss << "delete from deadband where oid = " << oid;
            return oss.str();
        }
    };

using alarm_limits_t = struct alarm_limits;

template<class E>
bool insert_entity(E& entity, sqlite3* context)
{
    std::string insertSql = entity.insert_statement();
    bool foo = SQLITE_OK == ::sqlite3_exec(context, insertSql.c_str(), NULL, 0, NULL);
    if (!foo)
        LOG4CPLUS_ERROR(Logger::getInstance("test"), "SQL Error: " << insertSql << " Error : " << ::sqlite3_errmsg(context));

    sqlite3_int64 rowId = ::sqlite3_last_insert_rowid(context);
    entity.oid = rowId;
    return true;
}

}

#endif /* __PERSIST_H___ */
