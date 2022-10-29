/** @file DBirthMessageBuilder.h 
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
#ifndef DBIRTHMESSAGEBUILDER_H_
#define DBIRTHMESSAGEBUILDER_H_

#include "sparkplug_b.h"

namespace zios {
namespace sparkplug {

class DBirthMessageBuilder
{
public:
    DBirthMessageBuilder();
    virtual ~DBirthMessageBuilder();

    com::cirruslink::sparkplug::protobuf::Payload buildMessage();
};

}
}


#endif /* DBIRTHMESSAGEBUILDER_H_ */
