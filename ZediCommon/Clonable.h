/** @file Clonable.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Aug 21, 2013 by daver:
 * @endhistory
 */

#ifndef CLONABLE_H_
#define CLONABLE_H_

namespace zios {
namespace common {

class Clonable {
public:
    Clonable();
    virtual ~Clonable();
    virtual Clonable* clone() const = 0;
};

}
} /* namespace zios */
#endif /* CLONABLE_H_ */
