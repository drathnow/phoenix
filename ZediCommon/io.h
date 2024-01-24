/** @file io.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 6, 2016 by daver:
 * @endhistory
 */
#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <sys/epoll.h>

namespace zios {
namespace common {

#define IO_MAX_EVENTS      64

class Pollable {
public:
    virtual ~Pollable() {}

    virtual int fileDescriptor() const = 0;

};

class Poller;

class EventHandler {
public:
    virtual ~EventHandler() {}

    virtual int handleRead() = 0;
    virtual int handleWrite() = 0;
    virtual int handleError() = 0;

    inline bool operator==(const EventHandler& otherHandler) const {
        return this == &otherHandler;
    }
};

/** class PollKey
 *
 * A PollKey represents an association between a Poller and a file descriptor.
 *
 */
class PollKey {
public:
    virtual ~PollKey();

    static const int EV_READ;
    static const int EV_WRITE;
    static const int EV_ERROR;

    /**
     * Changes the events of interest for the file descriptor associated with this PollKey
     *
     * @param[in] eventMask - A bit mask made from a bitwise or of some, or all of
     *      EV_READ
     *      EV_WRITE
     *      EV_ERROR
     *
     * @return int 0 if the operation succeeded, -1 if not. errno has the cause.
     */
    virtual int changeInterestEvents(uint32_t eventMask);

    /**
     * Determines if this PollKey is valid.  This method should be invoked before any of the
     * is*() methods to make sure that the current pollkey is still valid. A PollKey could be
     * invalid if a call to the associated EventHandler closes the pollkey.
     */
    virtual bool isValid() const;

    /**
     * Indicates whether or not a read operation is outstanding on the associated file descriptor.
     *
     */
    virtual bool isReadable() const;

    /**
     * Indicates whether the assciated file descriptor is read for a write operation.
     *
     */
    virtual bool isWritable() const;

    /**
     * Indicates whether or not an error was detected on the associated file descriptor.
     *
     */
    virtual bool hasError() const;

    /**
     * Closes this PollKey.  The associated file descriptor is deregistered from
     * the associated poller.  Once this method is called, no more access to this
     * object can occur.  NOTE: calling this method does not close the associated
     * file descriptor.
     */
    virtual void close();

    /**
     * Sets the EventHandler for the associated file descriptor.  If an EventHandler is already
     * set, the new object superceeds it. This object will not take ownership of the EventHandler.
     * The caller is responsible for cleaning up the object after this Pollkey is closed.
     */
    virtual void setEventHandler(EventHandler& eventHandler) { _eventHandler = &eventHandler; }

    /**
     * Returns the event handler associated with this PollKey.
     */
    virtual inline EventHandler* eventHandler() { return _eventHandler; }

    friend class Poller;

protected:
    PollKey(EventHandler* eventHandler, Poller* poller, int fd);

    virtual int eventMasktoEPollEventMask(int eventMask) const;

private:
    PollKey(const PollKey& otherKey);
    PollKey& operator=(const PollKey& otherKey);

    PollKey* _next;
    EventHandler* _eventHandler;
    Poller* _poller;
    int _fd;
    uint32_t _readyEvents;
    struct epoll_event _event;

    void setReadyEvents(uint32_t readyEvents) { _readyEvents = readyEvents; }
};

class Poller {
public:
    static Poller* open();

    virtual ~Poller();

    /**
     * Invoked the underlying system calls to wait for events on a file descriptor.
     *
     * @param[in] timeoutMilliseconds - the number of milliseconds to block before returning.
     * You can specify no argument or pass -1 to block until an event is detected.
     *
     * @return int - the number of file descriptors that events were detected on.
     */
    virtual int poll(int timeoutMilliseconds = -1);

    /**
     * Register a file descriptor with this Poller.
     *
     * @param[in] fd - A file descriptor
     *
     * @param[in] eventHandler - A reference to an EventHandler object that will handle
     * events detected on the associated file descriptor.  Callers should not delete this
     * object until after the close() method has been called on the returned PollKey, or the
     * event handle is replaced on the PollKey by a call to setEventHandler().
     *
     * @param[in] eventMask - Optional events of interest. The value is a bit mask where each
     * bit represents an event type.  See the description epoll_ctrl and the descriptions of
     * the events field in the 'epoll_event' struct.  If you do not set this value, you can
     * call changeInterestEvents
     */
    virtual PollKey* registerDescriptor(int fd, EventHandler& eventHandler, int eventMask = 0);

    virtual PollKey* registerPollable(zios::common::Pollable& pollable, EventHandler& eventHandler, int eventMask = 0);
    virtual void closePollKey(PollKey* pollKey);

    virtual int forgetDescriptor(int fd, struct epoll_event* event);
    virtual int changeInterestEvents(int fd, struct epoll_event* event);
    virtual PollKey** readyKeys() { return _readyKeys; }
    virtual void close();

protected:
    Poller();

private:
    Poller(const Poller& otherPoller);
    Poller& operator=(const Poller& otherPoller);

    int _epfd;
    struct epoll_event _readyEvents[IO_MAX_EVENTS*sizeof(struct epoll_event)];
    PollKey* _readyKeys[IO_MAX_EVENTS];
    PollKey* _deletableKeys;

    void deletePollKey(PollKey* pollKey);
};

class EventProcessor {
public:
    EventProcessor();
    virtual ~EventProcessor();

    /**
     * Processes events.
     *
     * @return int - The number of events that were processed. Zero if a timeout value
     * was specified and no events were detected. -1 if an error occurs. errno has the cause.
     */
    virtual int processEvents(int timeoutMilliseconds = -1) = 0;
    virtual inline Poller& poller() = 0;
};

class DefaultEventProcessor : public EventProcessor {
public:
    DefaultEventProcessor(Poller* poller);
    virtual ~DefaultEventProcessor();

    virtual int processEvents(int timeoutMilliseconds = -1);
    virtual inline Poller& poller() { return *_poller; }

private:
    Poller* _poller;
};

} /* namespace common */
} /* namespace zios */

#endif /* IO_H_ */
