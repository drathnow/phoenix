/** @file ReturnCodes.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 4, 2017 by daver:
 * @endhistory
 */
#include "ReturnCodes.h"

namespace zios {
namespace common {

static const char* SUCESS_STR = "Success";
static const char* UNDEFINED_ERROR_STR = "Undefined error";
static const char* DUPLICATE_NAME_STR = "Duplicate name";
static const char* DUPLICATE_ID_STR = "Duplicate ID";
static const char* NOT_FOUND_STR = "Not found";
static const char* NOT_EMPTY_STR = "Not empty";
static const char* INVALID_PARAMETER_STR = "Invalid parameter";
static const char* TIMEOUT_STR = "Timeout";
static const char* UNSUPPORTED_OPERATION_STR = "Unsupported operation";
static const char* INVALID_NAME_STR = "Invalid name";
static const char* COMM_API_ERROR_STR = "Comm API error";
static const char* MQ_NOT_OPEN_STR = "Queue not open";
static const char* DUPLICATE_PASSTHROUGH_PORT_STR = "Duplicate PassThrough Port";
static const char* MQ_RECEIVER_WAKE_ERROR_STR = "Bad setup of notifying socket(s) for message Q receiver";

const char* ReturnCodes::strerror(ReturnCodes::ReturnCodeID returnCode) {
    // TODO: Finish this list!!!
    switch (returnCode) {
        case SUCCESS:
            return SUCESS_STR;

        case UNDEFINED_ERROR:
            return UNDEFINED_ERROR_STR;

        case DUPLICATE_NAME:
            return DUPLICATE_NAME_STR;

        case DUPLICATE_ID:
            return DUPLICATE_ID_STR;

        case MQ_RECEIVER_WAKE_ERROR :
            return MQ_RECEIVER_WAKE_ERROR_STR;

        case MQ_NOT_OPEN :
            return MQ_NOT_OPEN_STR;

        case NOT_FOUND:
            return NOT_FOUND_STR;

        case NOT_EMPTY:
            return NOT_EMPTY_STR;

        case MQ_INVALID_PARAMETER :
        case INVALID_PARAMETER:
            return INVALID_PARAMETER_STR;

        case TIMEOUT:
            return TIMEOUT_STR;

        case UNSUPPORTED_OPERATION:
            return UNSUPPORTED_OPERATION_STR;

        case INVALID_NAME:
            return INVALID_NAME_STR;

        case DUPLICATE_PASSTHROUGH_PORT:
            return DUPLICATE_PASSTHROUGH_PORT_STR;

        case COMM_API_ERROR:
            return COMM_API_ERROR_STR;

        default:
            return "Unknown/not implemented error code";

    }

    return "Unknown error";
}

}
}

