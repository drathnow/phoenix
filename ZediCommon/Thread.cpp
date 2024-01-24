#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <cstring>
#include <string>
#include <iostream>
#include <ctime>
#include <cerrno>
#include <signal.h>

#include "templates.h"
#include "Runnable.h"
#include "Thread.h"
#include "Exception.h"
#include "Mutex.h"
#include "Utl.h"
#include "IllegalStateException.h"

namespace zios {
namespace common {

using namespace log4cplus;
using namespace std;

static std::map<pthread_t, Thread*> threadMap;
static Mutex mutex;

const Logger Thread::_logger = Logger::getInstance("zios.common.Thread");

void *threadMain(void *ptr);

Thread::Thread(Runnable& runnable, const char* name) :
        _runnable(runnable),
        _name(name == NULL ? "Thread" : name),
        _running(false),
        _pthread(0U),
        _threadId(0),
        _detached(false) {
}

Thread::~Thread() {
    pthread_t self = ::pthread_self();
    ScopedLock<Mutex> lock(mutex);
    std::map<pthread_t, Thread*>::iterator iter = threadMap.find(self);
    if (iter != threadMap.end())
        threadMap.erase(iter);
}

int Thread::join(long waitTimeSeconds) const {
    if (waitTimeSeconds == 0) {
        return ::pthread_join(_pthread, NULL);
    } else {
        struct timespec abstime;
        ::clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += waitTimeSeconds;
        return ::pthread_timedjoin_np(_pthread, NULL, &abstime);
    }
}

int Thread::start() {
    if (_pthread != 0)
        return -1;
    int err;
    pthread_attr_t attr;
    if ((err = ::pthread_attr_init(&attr)) != 0)
        return err;
    if (_detached)
        ::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    err = ::pthread_create(&_pthread, &attr, threadMain, this);
    ::pthread_attr_destroy(&attr);
    return err;
}

void Thread::signal(int signo) const {
    ::pthread_kill(_pthread, signo);
}

bool Thread::isRunning() const {
    return _running;
}

Thread* Thread::currentThread() {
    pthread_t self = ::pthread_self();
    ScopedLock<Mutex> lock(mutex);
    std::map<pthread_t, Thread*>::iterator iter = threadMap.find(self);
    return iter == threadMap.end() ? NULL : iter->second;
}

void *threadMain(void *ptr) {
	Thread* thread = (Thread*)ptr;
	thread->_threadId = ::syscall(SYS_gettid);
	Logger log = Logger::getInstance("zios.common.Thread");
	{
	    ScopedLock<Mutex> lock(mutex);
	    std::pair<pthread_t, Thread*> newPair(thread->_threadId, thread);
	    threadMap.insert(newPair);
	}

    try {
        thread->_running = true;
        thread->_runnable.run();
    } catch (std::bad_alloc& e) {
        LOG4CPLUS_FATAL(Thread::_logger, thread->name() << " - Bad alloc detected!!!");
    } catch (std::exception& e) {
        LOG4CPLUS_ERROR(Thread::_logger, thread->name() << " - Unexpected exception from Runnable: " << e.what());
    } catch (...) {
        LOG4CPLUS_ERROR(Thread::_logger, thread->name() << " - Unexpected default exception from Runnable");
    }
    thread->_running = false;
    ::pthread_exit(0);
    return NULL;
}

} // namespace common
} // namespace zios
