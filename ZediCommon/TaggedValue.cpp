/** @file TaggedValue.cpp
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 21, 2015 by andrewj:
 * @endhistory
 */

#include "TaggedValue.h"

namespace zios {
namespace common {

TaggedValue::TaggedValue(uint32_t id, const std::string& value) :
        id(id),
        value(value) {
}

}
} // namespace
