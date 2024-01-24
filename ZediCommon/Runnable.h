/** @file Runnable.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 29, 2013 by daver:
 * @endhistory
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

namespace zios {
namespace common {

/**
 * A Runnable object abstracts the target of a thread's execution.  When a thread starts running,
 * the Runnable objects run method will be invoked.  Derived classes must provide an implemenation
 * of this method.
 */
class Runnable {
public:
	Runnable() {}
	virtual ~Runnable() {}

	/** run
	 *
	 *  Invoked in the context of the new thread.  Implemenations should ensure that they do not throw
	 *  exceptions.
	 */
	virtual void run() = 0;

    bool operator==(const Runnable& otherRunnable) const {
        return this == &otherRunnable;
    }
};

} // namespace common
} // namespace zios

#endif /* RUNNABLE_H_ */
