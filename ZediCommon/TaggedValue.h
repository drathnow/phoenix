/** @file TaggedValue.h
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
#ifndef TAGGEDVALUE_H_
#define TAGGEDVALUE_H_

#include <stdint.h>
#include <string>
#include <list>


namespace zios {
namespace common {


/**
 * A TaggedValue is a simple container class to represent a string value that has an id.
 */
class TaggedValue {
public:
    TaggedValue(uint32_t id, const std::string& value);

    uint32_t    id;
    std::string value;
};

typedef std::list<zios::common::TaggedValue> tagged_value_list_t;

}
} // namespace


#endif /* TAGGEDVALUE_H_ */
