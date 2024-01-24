/** @file ReturnCodes.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Sep 22, 2014 by eganb:
 * IncrDev Sep 30, 2014 by eb: add SYSTEM PORT code
 * IncrDev Jan 13, 2015 by eb: add some IOPoint codes
 * IncrDev Feb 10, 2015 by eb: change/add codes (General)
 * IncrDev Feb 10, 2015 by eb: add code for iopoint update when create is requested
 * IncrDev Jun 16, 2016 by eb: add codes for OTAD
 * @endhistory
 */
#ifndef RETURNCODES_H_
#define RETURNCODES_H_

#include <stdint.h>
#include <string>

namespace zios {
namespace common {

class ReturnCodes {

public:
    enum ReturnCodeID{

        // General Errors
        SUCCESS           = 0,     ///<    SUCCESS
        UNDEFINED_ERROR   = 1,     ///<    general or undefined error
        DUPLICATE_NAME    = 2,     ///<    Duplicate name found on primary name key
        DUPLICATE_ID      = 3,     ///<    Duplicate id found on primary ID key
        NOT_FOUND         = 4,     ///<    Search did not return hit on primary search term.
        NOT_EMPTY         = 5,     ///<    Object has references still
        INVALID_PARAMETER = 6,     ///<    Parameter invalid or not supported
        TIMEOUT           = 7,     ///<    Action Timed out.
        UNSUPPORTED_OPERATION = 8, ///<    Operation not supported in current configuration
        INVALID_NAME      = 9,     ///<    Invalid character in name field -> ':', '[', ']', '*', '?', '<' ,'>', ',' , '|', '/', '\', '%'


        // map type errors
        MISSING_KEY       = 46,
        DUPLICATE_KEY     = 47,
        INVALID_KEY       = 48,
        INVALID_VALUE     = 49,

        // Site errors
        DUPLICATE_SITE    = 50,
        SITE_NOT_FOUND    = 51,
        MISSMATCHED_SITE  = 52,

        // External Device errors
        DUPLICATE_EXT_DEV = 100,
        EXT_DEV_NOT_FOUND = 101,

        //IOPOINT ERRORS
        DUPLICATE_IOPOINT = 200,
        IOPOINT_NOT_FOUND = 201,
        INVALID_POLLSET   = 202,
        SYSTEM_POINT      = 203,
        NOT_TIME_YET      = 204,
        INVALID_ALARM_SETTING = 205,
        INVALID_SOURCE_ADDRESS = 206,
        INVALID_EXT_ID  = 207,
        IOPOINT_UPDATED = 208,				// internal code for when "recreating" existing point
        NO_ACTIVE_ALARM_SETTINGS = 209,       // internal 'error' for when alarmMeister does not create because there are NO active alarm settings.

        // protocol errors
        PROTOCOL_NOT_FOUND = 300,
        PROTOCOL_PARSE_ERROR = 301,
        PROTOCOL_EXCEPTION_ERROR = 303,
        PORT_API_ERROR    = 304,


        //port errors
        // ERR_CLASS = 0x1100,///< Serial error class
        COMM_API_ERROR    = 401,         ///< Underlaying API returns an error
        COMM_NOT_FOUND 	  = 402,    ///< Serial port not found
        DUPLICATE_PORT    = 403,
        PORT_NOT_FOUND    = 404,
        PORT_ALREADY_OPEN = 405,      ///< Serial port already open
        PORT_NOT_OPEN     = 406,          ///< Port not opened
        PORT_LINE_ERROR   = 407,        ///< RS232 line error
        PORT_NO_ACCESS    = 409,     ///< No access rights for port (Try as root)
        PORT_LOCK_NOT_ACQUIRED = 410,  ///< Loc
        SYSTEM_PORT       = 411,
        DUPLICATE_PASSTHROUGH_PORT       = 412,

        //Hardware API errors
        DPORT_ACCESS_FAIL   = 450,
        DPORT_BIT_READ_ONLY = 451,
        DPORT_NOT_INITIALIZED = 452,

        // persistence Engine errors  (internal errors)
        PERSISTENCE_FAIL            = 500,     ///<    General persistence failure
        PERSIST_API_ERROR           = 501,     ///<    underlying persistence API failure (eg sql3)
        PERSIST_TRANSACTION_ERROR   = 502,     ///<    transaction failure
        PERSIST_SAVE_ERROR          = 503,     ///<    could not persist object to store
        PERSIST_DELETE_ERROR        = 504,     ///<    could not persist "removal" of object to store
        PERSIST_NOT_OPEN            = 505,     ///<    operation failed, persistence NOT open

        // Message Queue errors (internal errors)
        MQ_INVALID_PARAMETER        = 600,     ///<   bad parameter to MQ (message size, or Max Message count)
        MQ_RECEIVER_WAKE_ERROR      = 601,     ///<   bad setup of notifying socket(s) for message Q receiver
        MQ_NOT_OPEN                 = 602,     ///<   mq not open, operation not allowed.
        MQ_CREATION_FAIL            = 603,
        MQ_WRITE_FAIL               = 604,
        MQ_INVALID_MESSAGE          = 605,


        //OTAD errors
        OTAD_FAILURE                = 700,      ///< general otad failure
        OTAD_ALREADY_IN_PROGRESS    = 701,
        OTAD_VERSION_EXISTS         = 702,
        OTAD_SERVER_MISSING         = 703,
        OTAD_COULDNT_GET_FILE       = 704,
        OTAD_NO_OTOAD_SCRIPT        = 707,
        OTAD_SCRIPT_FAILURE         = 708,

        // OTAD STATUS
        OTAD_GETTING_FILE           = 750,
        OTAD_UPDATING               = 755,
        OTAD_COMPLETE               = 760,

        // Network Config Errors
        NET_CONFIG_NOT_FOUND        = 800,
        NET_CONFIG_BACKUP_NOT_FOUND = 801,
        NET_CONFIG_UNREADABLE       = 802
    };

    ~ReturnCodes() {}

    static const char* strerror(ReturnCodes::ReturnCodeID returnCode);

private:
    ReturnCodes() {}
};

typedef struct {
    uint32_t id;
    ReturnCodes::ReturnCodeID rc;
} new_result_t;


}} // namespace

#endif /* RESULTCODES_H_ */
