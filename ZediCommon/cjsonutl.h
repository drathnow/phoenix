/** @file cjsonutl.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 20, 2014 by daver:
 * @endhistory
 */
#ifndef CJSONUTL_H_
#define CJSONUTL_H_

#include <stdint.h>

#include <cjson/cJSON.h>

namespace zios {
namespace common {

bool JSONUTL_didExtractU32ValueFromItem(cJSON& json, uint32_t& returnValue);
bool JSONUTL_didExtractS16ValueFromItem(cJSON& json, uint16_t& returnValue);

class cJSONPtr {
public:
    cJSONPtr(cJSON* ptr);
    ~cJSONPtr();

    cJSON* get() const;
    cJSON* operator*() const;

private:
    cJSON* _ptr;
};

}
}

#endif /* CJSONUTL_H_ */
