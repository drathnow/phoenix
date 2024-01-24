/** @file SignalNotification.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun 10, 2015 by andrewj:
 * BugFix_ Jun 18, 2015 by eb: added virtual destructor 
 * @endhistory
 */
#ifndef SIGNALNOTIFICATION_H_
#define SIGNALNOTIFICATION_H_

namespace zios {
namespace common {


/**
 * Signal handling callbacks.
 * Inherit from this interface and override the necessary functions.
 * Each function has a default implementation so only the desired
 * functions need to be implemented.
 */
class ISignalNotification {
public:
    virtual ~ISignalNotification(){}

    /**
     * shutdown "now"
     * @param sig
     */
    virtual void handleAbortSignal(int sig) {}

    /**
     * handle SIGHUP signals
     * @param sig - signal causing the callback
     * @retval true - log the signal in the exit log
     * @retval false - don't log anything
     * @return logging flag
     */
    virtual bool handleHangupSignal(int sig) { return false; }


    /**
     * handle SIGUSR1 or SIGUSR2 signals
     * @param sig - signal causing the callback
     * @retval true - log the signal in the exit log
     * @retval false - don't log anything
     * @return logging flag
     */
    virtual bool handleUserSignal(int sig) { return false; }
};

} } // namespace

#endif /* SIGNALNOTIFICATION_H_ */
