/** @file cjsonutl.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 22, 2016 by daver:
 * @endhistory
 */

#include "StringUtl.h"
#include <cjson/cJSON.h>
#include "cjsonutl.h"
#include <string>
#include <cstdlib>
#include <stdint.h>

namespace zios {
namespace common {

using namespace std;

bool JSONUTL_didExtractU32ValueFromItem(cJSON& json, uint32_t& returnValue) {
    if (NULL != json.valuestring) {
        string value(json.valuestring);
        if (false == StringUtl::stringToInt(value, (int32_t&)returnValue))
            return false;
    } else
        returnValue = json.valueint;
    return true;
}

bool JSONUTL_didExtractS16ValueFromItem(cJSON& json, uint16_t& returnValue) {
    if (NULL != json.valuestring) {
        string value(json.valuestring);
        int32_t numValue;
        if (false == StringUtl::stringToInt(value, numValue))
            return false;
        returnValue = (uint16_t)numValue;
    } else
        returnValue = json.valueint;
    return true;
}

cJSONPtr::cJSONPtr(cJSON* ptr) : _ptr(ptr) {};
cJSONPtr::~cJSONPtr() {
    cJSON_Delete(_ptr);
}

cJSON* cJSONPtr::get() const { return _ptr; }
cJSON* cJSONPtr::operator*() const { return _ptr; }

}
}


