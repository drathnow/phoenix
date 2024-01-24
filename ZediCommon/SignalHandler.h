/** @file SignalHandler.h
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
 * @endhistory
 */
#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <stdio.h>
#include <string.h>
#include <signal.h>   // for signal handling.
#include "SignalNotification.h"

namespace zios {
namespace common {

/**
 * Static class to abstract handling operating system signals.
 *
 * This class uses all static methods and variables, as it is naturally a singleton.
 *
 * To use, call init() with the filename to log exit information, and the handler
 * to receive notifications.
 *
 * Use registerNotificationHandler() to change which object will receive notifications.
 *
 * Be sure to set the notificationHandler to NULL before destroying the object handling
 * the signals.
 */
class SignalHandler {
public:
    static void registerNotificationHandler(ISignalNotification *notificationHandler);
    static void init(const char *exitFilename, ISignalNotification *handler);

private:
    static void addSignalInfoToFile( int sig, FILE *stream );
    static FILE* getSysExitLogFile();
    static void segvSignal(int sig, siginfo_t *info, void *secret);
    static void abortSignal(int sig);
    static void userSignal(int sig);
    static void hangupSignal(int sig);

    static ISignalNotification * _handler;
    static const char *_exitFilename;

    static const int MaxLogFileSize=1024*64;        // max size of exit-log
};

} } // namespace

#endif /* SIGNALHANDLER_H_ */
