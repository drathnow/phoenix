/** @file Process.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "StringUtl.h"
#include "IllegalArgumentException.h"
#include "PermissionException.h"
#include "ProcessNotFoundException.h"
#include "File.h"
#include "Process.h"
#include <cstdio>
#include <fcntl.h>
#include "Thread.h"

namespace zios {
namespace common {

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

static const Logger logger = Logger::getInstance("zios.common.process");
const int Process::ExitCodeFailedLaunch = 127;
//
//ProcessOutputReader::ProcessOutputReader(Process& process, const char* logfile) :
//        _process(&process),
//                _logFileFd(-1),
//                _logfileName(NULL),
//                _stop(true),
//                _thread(NULL)
//{
//    if (logfile != NULL) {
//        int len = ::strlen(logfile);
//        _logfileName = new char[len+1];
//        ::strncpy(_logfileName, logfile, len);
//        _logfileName[len] = '\0';
//    }
//}
//
//ProcessOutputReader::~ProcessOutputReader()
//{
//    close(_logFileFd);
//    delete _logfileName;
//}
//
//void ProcessOutputReader::run()
//{
//    char buffer[256];
//
//    _logFileFd = ::open(_logfileName, O_WRONLY | O_CREAT);
//    if (_logFileFd < 0)
//        LOG4CPLUS_WARN(logger, "Unable to open process log file " << _logfileName << ": " << ::strerror(errno));
//
//    _stop = false;
//    int pipeFd = _process->getOutputFileDescriptor();
//    while (_stop == false) {
//        // TODO: In reality, this loop wont work unless we use ::select :(
//        int len = ::read(pipeFd, buffer, sizeof(buffer));
//        ::write(_logFileFd, buffer, len);
//    }
//    ::close(_logFileFd);
//}
//
//void ProcessOutputReader::start()
//{
//    delete _thread;
//    _thread = new Thread(*this);
//    _thread->start();
//}

Process::Process(std::string path) :
        path(path)
{
    _pid = 0;
    _exitCode = 0;
    _exitSignal = 0;
    isRedirectStdout = false;
    _parentTermSignal = 0;

    // mark fds as invalid
    _outputFds[0] = -1;
    _outputFds[1] = -1;
}

Process::Process() :
        path("")
{
    _pid = 0;
    _exitCode = 0;
    _exitSignal = 0;
    isRedirectStdout = false;
    _parentTermSignal = 0;

    // mark fds as invalid
    _outputFds[0] = -1;
    _outputFds[1] = -1;
}

Process::~Process()
{
    // wait for child to finish?
    LOG4CPLUS_TRACE(logger, "ProcessRunner destructor.");

    // close file descriptors if not done yet
    if (_outputFds[0] >= 0) {
        ::close(_outputFds[0]);
        LOG4CPLUS_TRACE(logger, "Closed fd[0]=" << _outputFds[0]);
    }
    if (_outputFds[1] >= 1) {
        ::close(_outputFds[1]);
        LOG4CPLUS_TRACE(logger, "Closed fd[1]=" << _outputFds[1]);
    }

}

int Process::getPid()
{
    return _pid;
}

int Process::getExitCode()
{
    return _exitCode;
}

int Process::getExitSignal()
{
    return _exitSignal;
}

bool Process::isRunning() const
{
    return (_pid > 0 ? true : false);
}

int Process::getOutputFileDescriptor()
{
    return _outputFds[0];
}

void Process::setParentTerminationSignal(int signal)
{
    _parentTermSignal = signal;
}

int Process::getParentTerminationSignal()
{
    return _parentTermSignal;
}

//ProcessOutputReader* Process::processOutputReaderForOutputFile(const char* outputFile)
//{
//    isRedirectStdout = true;
//    return new ProcessOutputReader(*this, outputFile);
//}

void Process::run(void)
{
    if (path.length() == 0) {
        LOG4CPLUS_TRACE(logger, "Run: invalid path");
        THROW_NAMED_EXCEPTION(IllegalArgumentException, "Invalid path.");
    }

    // reset values in case class is reused
    _pid = 0;
    _exitCode = 0;
    _exitSignal = 0;

    // create a pipe.  File descriptors for two ends of the pipe are created in fds
    // outputFds[0] is the reading end, pppOutputFds[1] is the writing end
    if (isRedirectStdout) {
        if (::pipe(_outputFds) < 0) {
            THROW_EXCEPTION("Unable to create pipe.");
        }
    } else {
        // mark them as closed in case a previous run had initialized them
        if (_outputFds[0] >= 0) {
            ::close(_outputFds[0]);
        }
        _outputFds[0] = -1;
        if (_outputFds[1] >= 1) {
            ::close(_outputFds[1]);
        }
        _outputFds[1] = -1;
    }

    _pid = ::fork();
    if (_pid == (pid_t) 0) {
        // push the name of the program ahead of the list as convention indicates arv[0] = name of program
        zios::common::File exeFile(path);
        string filename(exeFile.name());
        arguments.insert(arguments.begin(), filename);

        char **args = StringUtl::createStringArray(arguments);

        // redirect stderr
        {
            int nullfd = ::open("/dev/null", O_RDWR);
            if (nullfd < 0)
                LOG4CPLUS_ERROR(logger, "Unable to open /dev/null for child process stderr.");
            else {
                ::dup2(nullfd, STDERR_FILENO);
                ::close(nullfd);  // STDERR is still open
            }
        }

        // redirect stdout to either a pipe to parent or to /dev/null
        if (isRedirectStdout) {
            //
            // Connect the write end of outputFds to stdout.
            // Don't log more items after this because STDOUT is now being captured by the pipe and
            // it causes unexpected output on the file descriptor looking for child process output
            //
            ::close(_outputFds[0]);
            ::dup2(_outputFds[1], STDOUT_FILENO);
        } else {
            int nullfd = ::open("/dev/null", O_RDWR);
            if (nullfd < 0)
                LOG4CPLUS_ERROR(logger, "Unable to open /dev/null for child process stdout.");
            else {
                ::dup2(nullfd, STDOUT_FILENO);
                ::close(nullfd); // STDOUT is still open
            }
        }

        // registers the signal the child will receive when the parent process dies
        if (_parentTermSignal)
            ::prctl(PR_SET_PDEATHSIG, _parentTermSignal, 0, 0, 0);

        // replace the child process with the pppd program
        ::execvp(path.c_str(), args);

        // if we get here, the execvp failed
        StringUtl::freeStringArray(args);

        // This one's OK to log because the child didn't launch anyway
        LOG4CPLUS_ERROR(logger, "Child: Unable to launch exec " << path << ": " << ::strerror(errno) << " (errno=" << errno << ")");
        ::exit(ExitCodeFailedLaunch);

    } else if (_pid < 0) {
        // this is the parent process, but the fork failed
        if (isRedirectStdout) {
            // close file descriptors
            ::close(_outputFds[0]);
            ::close(_outputFds[1]);

            // mark descriptors as invalid so we don't try to do anything with them later
            _outputFds[0] = -1;
            _outputFds[1] = -1;
        }

        // set pid back to 0 so we know there isn't a child
        _pid = (pid_t) 0;

        LOG4CPLUS_ERROR(logger, "Run: unable to fork.");
        THROW_EXCEPTION("Unable to fork.");
    } else {
        //
        // This is the parent process and the fork worked
        // pid is now valid
        //
        if (isRedirectStdout) {
            // close the write end of the pipe, we only need the read end
            ::close(_outputFds[1]);
            _outputFds[1] = -1;
        }
    }
}

bool Process::wait(bool noblock)
{
    int wait_status;
    int wait_result;
    int wait_options;

    // sanity check - handle case were there isn't a running child yet
    if (_pid == 0) {
        LOG4CPLUS_TRACE(logger, "Wait: pid==0, no process running.");
        return false;
    }

    if (noblock) {
        wait_options = WNOHANG;
    } else {
        wait_options = 0;
    }

    // set status to 0, as it might not be updated if pid can't be found
    wait_status = 0;
    wait_result = ::waitpid(_pid, &wait_status, wait_options);

    // error with waitpid - for this to error, pid must not exist any more which would indicate
    // a programming error.  Not sure how best to respond - for now at least log the error
    if (wait_result < 0) {
        LOG4CPLUS_WARN(logger, "Waitpid returned error=" << wait_result << " errno=" << errno);

        // must assume it's not running any more
        _pid = 0;
        return false;
    }

    // 0 result means the pid exists but it hasn't changed state yet
    // implies it's still running
    if (wait_result == 0) {
        return true;
    }

    if (WIFEXITED(wait_status)) {
        _pid = 0;
        _exitCode = WEXITSTATUS(wait_status);
        return false;
    }

    if (WIFSIGNALED(wait_status)) {
        _pid = 0;
        _exitSignal = WTERMSIG(wait_status);
        return false;
    }

    // if we get here, something unexpected happened.  Assume the child has died somehow
    _pid = 0;
    LOG4CPLUS_WARN(logger, "Unexpected wait return. wait_result=" << wait_result << " wait_status=" << wait_status);
    return false;
}

void Process::kill(int signal)
{
    kill(this->_pid, signal);
}

void Process::kill(int pid, int signal)
{
    // ignore case when child not running
    if (pid == 0)
        return;

    if (::kill(pid, signal) != 0) {
        //
        // save error in case something else clobbers it
        //
        int error = errno;

        LOG4CPLUS_DEBUG(logger, "kill failed with errno=" << error);

        switch (error) {
            case EINVAL:
                THROW_NAMED_EXCEPTION(IllegalArgumentException, "Invalid signal.");
                break;

            case EPERM:
                THROW_NAMED_EXCEPTION(PermissionException, "Insufficient permission.");
                break;

            case ESRCH:
                THROW_NAMED_EXCEPTION(ProcessNotFoundException, "No such process.");
                break;

            default:
                THROW_EXCEPTION("Unexpected error.");
        }
    }
}

Process::ProcessState Process::getProcessState()
{
    return processStateForPid(this->_pid);
}

/***
 * Obtain the process state for a given pid.
 * On Linux, the only way to find this is to use the /proc filesystem.
 * @param pid - ID of the process to query.
 * @return Status of the supplied process
 */
Process::ProcessState Process::processStateForPid(int pid)
{
    ProcessState result = Unknown;
    char statFilename[PATH_MAX];
    ::sprintf(statFilename, "/proc/%d/stat", pid);

    FILE * statFile = ::fopen(statFilename, "r");
    if (statFile == NULL)
        return Unknown;

    char state;
    if (::fscanf(statFile, "%*d %*s %c", &state) == 1) {
        switch (state) {
            case 'D':
                result = UninterruptibleSleep;
                break;
            case 'S':
                result = InterruptibleSleep;
                break;
            case 'R':
                result = Running;
                break;
            case 'T':
                result = Stopped;
                break;
            case 'X':
                result = Dead;
                break;
            case 'Z':
                result = Defunct;
                break;
        }
    }
    ::fclose(statFile);

    return result;
}

}
} // namespace

