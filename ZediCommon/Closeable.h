/** @file Closeable.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Nov 21, 2013 by daver:
 * @endhistory
 */

#ifndef CLOSEABLE_H_
#define CLOSEABLE_H_

namespace zios {
namespace common {

class Closeable {
public:
    Closeable();
    virtual ~Closeable();

    virtual void close() = 0;
};

} /* namespace common */
} /* namespace zios */
#endif /* CLOSEABLE_H_ */
