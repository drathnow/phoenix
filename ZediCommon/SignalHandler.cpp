/** @file SignalHandler.cpp
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

#include "SignalHandler.h"

#ifndef _ONARM
#include <execinfo.h> // for backtrace...
#endif
#include <time.h>

namespace zios {
namespace common {

// Initialize to default settings
ISignalNotification *SignalHandler::_handler = NULL;
const char *SignalHandler::_exitFilename = NULL;

/**
 * Adds a line to the specified file indicating which signal was received
 * @param sig  - signal number
 * @param stream - open FILE stream
 */
void SignalHandler::addSignalInfoToFile(int sig, FILE *stream) {
    ::fprintf(stream, "received %s (%d)\n", ::strsignal(sig), sig);
}

/**
 * Creates and/or opens the exit.log.
 * If the exit.log is too big, it is overwritten with a new file
 * copying the last part of the first file.
 * This creates a "rolling" file with a bound on maximum size
 *
 * After preparing the file, a header section is appended to the file
 * and the open FILE * is returned.
 *
 * @return NULL if unable to open file, FILE * otherwise
 */
FILE* SignalHandler::getSysExitLogFile() {
    if (!_exitFilename) {
        return NULL;
    }

    FILE *errFile = ::fopen(_exitFilename, "a+");
    if (NULL == errFile)
        return NULL;

    // Truncate file when length exceeds 16K
    ::fseek(errFile, 0, SEEK_END);
    long flen = ::ftell(errFile);
    if (flen > MaxLogFileSize) {
        char buffer[(MaxLogFileSize * 3) / 4];        // keep 3/4 of the file, shuffle the rest
        ::fseek(errFile, -sizeof(buffer), SEEK_END);
        flen = ::fread(buffer, sizeof(buffer), 1, errFile);
        ::fclose(errFile);
        // looks for beginning of a new section, indicated with a *
        char *src = (char*)::memchr(buffer, '*', flen);
        if (!src)
            src = buffer;
        if ((errFile = ::fopen(_exitFilename, "w")) != NULL) {
            ::fwrite(src, sizeof(buffer) - (src - buffer), 1, errFile);
        } else {
            return NULL;
        }
    }

    time_t now = ::time(NULL);

    ::fprintf(errFile, "**********************************\n");
    struct tm gmtt;
    ::memset(&gmtt, 0, sizeof(struct tm));
    if (NULL != ::gmtime_r(&now, &gmtt)) {
        char buf[32];
        ::fprintf(errFile, "%s", ::asctime_r(&gmtt, buf));
    } else {
        ::fprintf(errFile, "UNKNOWN TIME\n");
    }
    ::fprintf(errFile, "System exiting. Reason follows\n");
    return errFile;
}

/**
 * Signal handler for a segment fault.
 * Write a stack trace and then exit immediately with a core dump.
 *
 * @param sig   signal number
 * @param info  data structure containing detailed information about the signal
 * @param secret
 */
void SignalHandler::segvSignal(int sig, siginfo_t *info, void *secret)
{
#ifdef _ONARM
    FILE *log = getSysExitLogFile();
    if (log) {
        addSignalInfoToFile(sig, log);
        ::fclose(log);
    }
#else
    void *trace[16];
    int ntrace = ::backtrace(trace, 16) - 2;

    // Output exit info to exit log
    FILE *log = getSysExitLogFile();
    if (log) {
        addSignalInfoToFile(sig, log);
        if (ntrace > 0) {
            ::fprintf(log, "ntrace: %d\n", ntrace);
            ::fflush(log);
            ::backtrace_symbols_fd(&trace[2], ntrace, fileno(log));
        }
        ::fclose(log);
    }
#endif

    // cause application to core
    ::signal(sig, SIG_DFL);
    ::raise(sig);
}

/**
 * Signal handler for an application abort.
 * Log the signal, shut things down as quickly as possible.
 * @param sig
 */
void SignalHandler::abortSignal(int sig) {
    const char *exceptionMsg = NULL;
    FILE *errFile = getSysExitLogFile();
    if (errFile) {
        addSignalInfoToFile(sig, errFile);
    }
    if (_handler) {
        try {
            _handler->handleAbortSignal(sig);
        } catch (...) {
            exceptionMsg = "Caught exception in handleAbortSignal().\n";
        }

    }
    if (errFile) {
        if (exceptionMsg) {
            ::fputs(exceptionMsg, errFile);
        }
        ::fclose(errFile);
    }
}

/**
 * Signal handler to recycle network layer
 * @param sig
 */
void SignalHandler::hangupSignal(int sig) {
    bool log = false;
    const char *exceptionMsg = NULL;
    if (_handler) {
        try {
            log = _handler->handleHangupSignal(sig);
        } catch (...) {
            exceptionMsg = "Caught exception in handleHangupSignal().\n";
        }
    }

    if (log) {
        FILE *errFile = getSysExitLogFile();
        if (errFile) {
            addSignalInfoToFile(sig, errFile);
            if (exceptionMsg) {
                ::fputs(exceptionMsg, errFile);
            }
            ::fclose(errFile);
        }
    }
}

/**
 * Signal handler for user messages
 * @param sig
 */
void SignalHandler::userSignal(int sig) {
    bool log = false;
    const char *exceptionMsg = NULL;
    if (_handler) {
        try {
            log = _handler->handleUserSignal(sig);
        } catch (...) {
            exceptionMsg = "Caught exception in handlerUserSignal().\n";
        }
    }

    if (log) {
        FILE *errFile = getSysExitLogFile();
        if (errFile) {
            addSignalInfoToFile(sig, errFile);
            if (exceptionMsg) {
                ::fputs(exceptionMsg, errFile);
            }
            ::fclose(errFile);
        }
    }
}

void SignalHandler::registerNotificationHandler(ISignalNotification *handler) {
    SignalHandler::_handler = handler;
}

void SignalHandler::init(const char *exitFilename, ISignalNotification *handler) {
    SignalHandler::_handler = handler;
    _exitFilename = exitFilename;

    // Setup control c handler....
    // initialize signal handler struct
    struct sigaction sa;
    ::memset(&sa, 0, sizeof(struct sigaction));
    ::sigemptyset(&sa.sa_mask);

    // ignore unused signals that normally cause process termination
    sa.sa_handler = SIG_IGN;
    ::sigaction(SIGPIPE, &sa, NULL); // ignore so that write(2) may return EPIPE
    ::sigaction(SIGALRM, &sa, NULL);
    ::sigaction(SIGIO, &sa, NULL);    // for async IO - we don't use this

    // Shutdown network as quickly as possible
    sa.sa_handler = abortSignal;
    ::sigaction(SIGQUIT, &sa, NULL);
    ::sigaction(SIGABRT, &sa, NULL);
    ::sigaction(SIGINT, &sa, NULL);       // CTRL-C
    ::sigaction(SIGTERM, &sa, NULL);      // regular "kill" default

    // Handle "user" signals
    sa.sa_handler = userSignal;
    ::sigaction(SIGUSR1, &sa, NULL);
    ::sigaction(SIGUSR2, &sa, NULL);

    // Hangup signals
    sa.sa_handler = hangupSignal;
    ::sigaction(SIGHUP, &sa, NULL);

    // setup "were in trouble" handler for signals that normally cause a core dump
    sa.sa_flags = SA_RESTART | SA_SIGINFO; // pass extended into to handler
    sa.sa_sigaction = segvSignal;
    ::sigaction(SIGILL, &sa, NULL);
    ::sigaction(SIGBUS, &sa, NULL);
    ::sigaction(SIGFPE, &sa, NULL);
    ::sigaction(SIGSEGV, &sa, NULL);
}

}
} // namespace
