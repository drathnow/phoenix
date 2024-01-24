/** @file io.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 23, 2019 by daver:
 * @endhistory
 */

#include "io.h"
#include "Exception.h"

#include <log4cplus/loggingmacros.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <stdio.h>

#include "poll.h"

namespace zios {
namespace common {

using namespace log4cplus;

const int PollKey::EV_READ = 0x10;
const int PollKey::EV_WRITE = 0x20;
const int PollKey:: EV_ERROR = 0x40;

static const Logger logger = Logger::getInstance("zios.io");

PollKey::PollKey(EventHandler* eventHandler, Poller* poller, int fd) :
                    _next(NULL),
                    _eventHandler(eventHandler),
                    _poller(poller),
                    _fd(fd),
                    _readyEvents(0) {
    ::memset(&_event, 0, sizeof(struct epoll_event));
    _event.data.ptr = this;
}

PollKey::~PollKey() {
}

bool PollKey::isReadable() const {
    return _readyEvents & EPOLLIN;
}

bool PollKey::isWritable() const {
    return _readyEvents & EPOLLOUT;
}

bool PollKey::hasError() const {
    return _readyEvents & EPOLLERR;
}

int PollKey::changeInterestEvents(uint32_t eventMask) {
    _event.events = eventMasktoEPollEventMask(eventMask);
    return _poller->changeInterestEvents(_fd, &_event);
}

int PollKey::eventMasktoEPollEventMask(int eventMask) const {
    int epollMask = ((eventMask & EV_READ) > 0 ? EPOLLIN : 0)
                    | ((eventMask & EV_READ) > 0 ? EPOLLHUP : 0)
                    | ((eventMask & EV_WRITE) > 0 ? EPOLLOUT : 0)
                    | ((eventMask & EV_ERROR) > 0 ? EPOLLERR : 0);

    return epollMask;
}

void PollKey::close() {
    _poller->forgetDescriptor(_fd, &_event);
    _fd = 0;
    _poller->closePollKey(this);
}

bool PollKey::isValid() const {
    return _fd != 0;
}

Poller* Poller::open() {
    return new Poller();
}

Poller::Poller() :
        _epfd(::epoll_create1(0)),
        _deletableKeys(NULL) {
}

Poller::~Poller() {
    close();
}

void Poller::close() {
    if (_epfd != 0)
        ::close(_epfd);
    _epfd = 0;
    if (_deletableKeys != NULL)
        deletePollKey(_deletableKeys);
    _deletableKeys = NULL;
}

PollKey* Poller::registerDescriptor(int fd, EventHandler& eventHandler, int eventMask) {
    PollKey* pollKey =  new PollKey(&eventHandler, this, fd);
    pollKey->_event.events = pollKey->eventMasktoEPollEventMask(eventMask);
    if (::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &pollKey->_event) == -1) {
        delete pollKey;
        pollKey = NULL;
    }
    return pollKey;
}

PollKey* Poller::registerPollable(zios::common::Pollable& pollable, EventHandler& eventHandler, int eventMask) {
    return registerDescriptor(pollable.fileDescriptor(), eventHandler, eventMask);
}

int Poller::changeInterestEvents(int fd, struct epoll_event* event) {
    return ::epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, event);
}

int Poller::forgetDescriptor(int fd, struct epoll_event* event) {
    return ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, event);
}

int Poller::poll(int timeoutMillesecons) {
    deletePollKey(_deletableKeys);
    _deletableKeys = NULL;

    int count = ::epoll_wait(_epfd, _readyEvents, IO_MAX_EVENTS, timeoutMillesecons);
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            _readyKeys[i] = (PollKey*)_readyEvents[i].data.ptr;
            _readyKeys[i]->setReadyEvents(_readyEvents[i].events);
        }
    }
    return count;
}

void Poller::closePollKey(PollKey* pollKey) {
    PollKey** ptr = &_deletableKeys;
    while (*ptr != NULL)
        ptr = &(*ptr)->_next;
    *ptr = pollKey;
}

void Poller::deletePollKey(PollKey* pollKey) {
    if (pollKey != NULL)
        deletePollKey(pollKey->_next);
    delete pollKey;
}


EventProcessor::EventProcessor() {
}

EventProcessor::~EventProcessor() {
}


DefaultEventProcessor::DefaultEventProcessor(Poller* poller) : _poller(poller) {
    assert(_poller != NULL);
}

DefaultEventProcessor::~DefaultEventProcessor() {
}

int DefaultEventProcessor::processEvents(int timeoutMilliseconds) {
    int cnt;
    if ((cnt = _poller->poll(timeoutMilliseconds)) > 0) {
        PollKey** readyKeys = _poller->readyKeys();
        for (int i = 0; i < cnt; i++) {
            if (readyKeys[i]->isValid() && readyKeys[i]->isReadable()) {
                try {
                    readyKeys[i]->eventHandler()->handleRead();
                } catch (Exception& e) {
                    LOG4CPLUS_ERROR(logger, "Unhandled exception thrown from EventHandler::handleRead" << e.what());
                }
            }

            if (readyKeys[i]->isValid() && readyKeys[i]->isWritable()) {
                try {
                    readyKeys[i]->eventHandler()->handleWrite();
                } catch (Exception& e) {
                    LOG4CPLUS_ERROR(logger, "Unhandled exception thrown from EventHandler::handleWrite" << e.what());
                }
            }

            if (readyKeys[i]->isValid() && readyKeys[i]->hasError()) {
                try {
                    readyKeys[i]->eventHandler()->handleError();
                } catch (Exception& e) {
                    LOG4CPLUS_ERROR(logger, "Unhandled exception thrown from EventHandler::handleError" << e.what());
                }
            }
        }
    }
    return cnt;
}


} /* namespace common */
} /* namespace zios */
