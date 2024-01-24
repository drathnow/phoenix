/** @file Process.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 4, 2015 by andrewj:
 * @endhistory
 */
#ifndef PROCESS_H_
#define PROCESS_H_

#include <unistd.h>
#include <string>
#include <vector>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "Runnable.h"
#include "Exception.h"

namespace zios {
namespace common {

class Process;
class Thread;

//class ProcessOutputReader : public Runnable {
//public:
//    ~ProcessOutputReader();
//
//    void run();
//    void start();
//    void stop();
//
//    friend class Process;
//
//private:
//    Process* _process;
//    int _logFileFd;
//    char* _logfileName;
//    bool _stop;
//    Thread* _thread;
//
//    ProcessOutputReader(Process& process, const char* logfile);
//
//    ProcessOutputReader(const ProcessOutputReader& otherReader);
//    ProcessOutputReader& operator=(const ProcessOutputReader& otherReader);
//};

/**
 * Process encapsulates and simplifies running child processes.
 */
class Process {
public:

    static const int ExitCodeFailedLaunch;

    // list enums explicitly so order doesn't change across builds (i.e. DLL dependency)
    enum ProcessState {
        Unknown = 0,        // can't match or process doesn't exist
        Dead = 1,        // should never be seen
        Defunct = 2,        // terminated but not reaped (zombie)
        Stopped = 3,        // by job control or being traced
        InterruptibleSleep = 4,        // waiting for event
        UninterruptibleSleep = 5,        // usually IO
        Running = 6,        // on run queue
    };

    Process();
    Process(std::string path);
    virtual ~Process();


//    /**
//     * Returns a ProcessOutputReader attached to the output stream for this process.  This method must be
//     * called before the process is started (i.e. before run() is called).  If called after, this method
//     * and the object returned, have no effect.
//     *
//     * @param outputFile
//     *
//     * @return ProcessOutputReader* - This is a transfer of ownership.
//     */
//    ProcessOutputReader* processOutputReaderForOutputFile(const char* outputFile);

    /**
     * Returns the process ID of the child.
     * Note that the pid remains even once the child has exited.  If run() is called again,
     * a new pid will be created.
     * @return 0 of the child hasn't started, or the pid of the child
     */
    int getPid();

    /**
     * Returns the exit code returned from the child if the child has exited.
     * If the child stopped due to a signal, the value is 0.
     * @return
     */
    int getExitCode();

    /**
     * Returns the value of the signal that stopped the child.
     * @return signal value that stopped the child, or 0 if not stopped
     */
    int getExitSignal();

    /**
     *
     * @return true if child is still running, false if stopped
     */
    bool isRunning() const;

    /**
     * Returns the file descriptor from the pipe to the child process.
     * This represents STDOUT from the child.
     *
     * This file descriptor is provided so the caller can read or select on the pipe
     * Do not close the descriptor as it will be closed when the process stops.
     *
     * @return >= 0 if descriptor is valid, -1 if invalid (i.e. not open)
     */
    int getOutputFileDescriptor();

    /**
     * If true, stdout from the child will be redirected and made available
     * by getOutputFileDescriptor.
     * If false, stdout from the child will be ignored.
     */
    bool isRedirectStdout;

    /**
     * Runs the process.  If unable to start, an exception will be thrown.
     * If successful, the pid property will reflect the child process id.
     *
     * @throw Exception
     */
    virtual void run(void);

    /**
     * Update status of the child, and wait for it to complete.
     * If noblock is true, wait() will return immediately if the child is still
     * running.  Otherwise, it will block the caller until the process stops.
     *
     * @param noblock
     * @return true if process is still running, false if it's stopped
     */
    virtual bool wait(bool noblock);

    /**
     * Sends a signal to the child.
     * If the signal can't be sent, an exception is thrown.
     *
     * @param signal The signal to send.
     */
    virtual void kill(int signal);

    /**
     * Returns the state of the child process.
     *
     * The process states possible match those listed under "Process State Codes" under "man ps"
     *
     * @return  If the child process isn't found, "Unknown" is returned
     */
    virtual ProcessState getProcessState();

    /**
     * Sets the signal that the child process will receive when the parent process dies.
     * Once the child process has started, changing this value will have no effect.
     *
     * @param signal 0=disabled, otherwise any valid signal
     */
    void setParentTerminationSignal(int signal);

    /**
     * Gets the current value for the signal the child will receive when the parent process dies.
     * @return
     */
    int getParentTerminationSignal();

    // create accessor methods and make them private later
    // path of executable
    std::string path;

    // vector holding arguments, in order
    std::vector<std::string> arguments;

    // static functions that are of general use for all processes

    /**
     * Sends a signal to the child.
     * If the signal can't be sent, an exception is thrown.
     *
     * @param[in] pid - process id of process to send signal to
     * @param[in] signal The signal to send.
     *
     * @throws IllegalArgumentException - An invalid signal was specified.
     *         PermissionException - The process does not have permission to send the signal to any
     *                               of the target processes
     *         ProcessNotFoundException - The process does not exist
     */
    static void kill(int pid, int signal);

    /**
     * Returns the state of the specified process
     *
     * @param pid   process id
     * @return  ProcessState.  Unknown is returned if pid can't be found
     */
    static ProcessState processStateForPid(int pid);

private:
    pid_t _pid;                 // child ID - non-zero implies it's running
    int _exitCode;
    int _exitSignal;            // exit signal if this was used to terminate child
    int _outputFds[2];          // file descriptors for the child pipe
    int _parentTermSignal;

    // no copy/assignment
    Process(Process&);
    Process operator=(Process &);
};

}
} // namespace

#endif /* PROCESS_H_ */
