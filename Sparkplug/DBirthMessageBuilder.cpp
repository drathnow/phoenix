/** @file DBirthMessageBuilder.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 3, 2022 by dave:
 * @endhistory
 */
#include "DBirthMessageBuilder.h"
#include "sparkplug_b.h"

namespace zios {
namespace sparkplug {

using namespace com::cirruslink::sparkplug::protobuf;

DBirthMessageBuilder::DBirthMessageBuilder()
{
}

DBirthMessageBuilder::~DBirthMessageBuilder()
{
}

Payload DBirthMessageBuilder::buildMessage()
{
    Payload payload;

    Payload_Metric metric;
    metric.set_name("foo");
    metric.set_datatype(data)
    return payload;
}

}
}
