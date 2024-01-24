/** @file sockets.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 5, 2016 by daver:
 * @endhistory
 */
#ifndef SOCKETS_H_
#define SOCKETS_H_
#include "io.h"

#include <string>
#include <netinet/in.h>

namespace zios {
namespace common {

class Socket : public Pollable {
public:
    Socket();
    Socket(const char* address, int portNumber);
    Socket(const Socket& otherSocket);
    ~Socket();

    /**
     * Opens this Socket.  A new socket descriptor is created.
     *
     * @return 0 on success. -1 if error occurs.  errno contains the reason
     */
    int open();

    /**
     * Configures this socket as blooking or nonblocking.  By default, the socket is
     * created as blocking.  You must frist call open before calling this method.
     *
     * @param isBlocking - true will block; false is nonblocking
     *
     * @return 0 on success, false otherwise.
     */
    int configureBlocking(bool isBlocking = true);

    /**
     * Indicates if the Socket is open or closed.  Open or closed depends
     * on the value of the underlying socket descriptor variable.  If the value is non-zero,
     * it is assumed to be open.  If zero, it is closed.  However, if someone were to pull
     * the socket descriptor from this object and manually close it, all bets are off.
     *
     * @return true if the socket is open. false closed.
     */
    bool isOpen() const;

    /**
     * @verbatim
      Connects this socket to the remote end.  This method delegates to the
      underlying  OS ::connect().
      See the relevent documentation for a description.
      @endverbatim
     * @return int - See the relavent documentation for a description.
     */
    int connect();

    /**
     * closes this socket.
     */
    void close();

    /**
     * Use this method if this Socket object is only used to create or assign to another socket
     * object.  It will clear the internal socket descriptor and address to ensure
     * that closing or destroying this object will not close the open socket.
     */
    void dispose();

    /**
     * @verbatim
      Obtain the socket descriptor.  This method is provided as a convenience method so that
      the socket descriptor can be used with either epoll or select.  Never call this method
      and then call ::close()  on the returned socket descriptor.
      @endverbatim
     *
     * @return int - A socket descriptor
     */
    int fileDescriptor() const;

    /**
     * @verbatim
      Reads from the socket. This method delegates to the underlying OS ::read() function. See
      the relavent documentation for a description.
      @endverbatim
     *
     * @param buffer - Buffer to receive the data.
     * @param bufferSize - Size of the buffer
     *
     * @return @verbatim See ::read() for description of return value. @endverbatim
     */
    int receive(void* buffer, int bufferSize);

    /**
     * @verbatim
     Writes data to a socket.  This method delegates to the underlying
     OS ::write()  function. See the relavent documentation for a description.
     @endverbatim
     *
     * @param data - Data to be written
     * @param dataLength - Length of the data to write.
     *
     * @return @verbatim See ::write() for description of return value.@endverbatim
     */
    int send(const void* data, int dataLength);

    /**
     * Assigns this Socket to another socket.  After completing the assignement, the underlying socket
     * descriptor will be shared between two socket objects.  You should call dispose() on one of these
     * object so ensure the underlying socket descriptor is not closed by accident when the other object
     * is destroyed.
     *
     * @param otherSocket - The other socket
     *
     * @return Reference to the newly assigned Socket
     */
    Socket& operator=(const Socket& otherSocket);

    friend class ServerSocket;

private:
    int _sockfd;
    int _portNumber;
    struct sockaddr_in _clientAddress;
};


class ServerSocket : public Pollable {
public:
    ServerSocket();

    /**
     * A copy construtor. If you use this constructor, it will result in two objects sharing the
     * same socket descriptor.  So, if you use this constructor, you should call dispose() on the
     * other ServerSocket object in order to avoid accidently closing the underlying socket
     * descriptor.
     *
     * @param otherServerSocket
     */
    ServerSocket(const ServerSocket& otherServerSocket);
    virtual ~ServerSocket();

    /**
     * Prepares it for listening
     *
     * @param configureBlocking - If true, the socket will be blocking.  If false, the socket will be
     * configured for none blocking operations. (Default is false)
     *
     * @return 0 if successful, -1 if failure. errno will contain the reason.
     *
     * @throws runtime_error - thrown if the socket is already open.
     *
     */
    int open();

    /**
     * Configures this serverSocket as blooking or nonblocking.  By default, the socket is
     * created as blocking.  You must frist call open before calling this method.
     *
     * @param isBlocking - true will block; false is nonblocking
     *
     * @return 0 on success, false otherwise.
     */
    int configureBlocking(bool isBlocking = true);

    /**
     * Starts the socket listening.
     *
     * @param backlog - Sets the listener backlog.
     *
     * @return 0 if successful, -1 if failure. errno will contain the reason.
     *
     * @throws runtime_error will be thrown if listen has been called on a ServerSocket
     * that is not open.
     */
    int listen(int backlog = 500);

    /**
     * Accepts an incoming connection.  You must call listen() before calling accept.
     *
     * @param returnSocket - If the return code is zero, the returnSocket argument will recieve
     * the new socket descriptor and client address.
     *
     * @return 0 if a new connection was accepted and created, -1 if an error occurs,
     * errno will contain the error.  If the socket is configured as a nonblocking socket,
     * errno will be either EAGAIN or EWOULDBLOCK if no connection request was detected.
     *
     * @throws runtime_error will be thrown if listen has been called on a ServerSocket
     * that is not open.
     */
    int accept(Socket& returnSocket);

    /**
     * Binds this ServerSocket to port number.
     *
     * @return 0 success, -1 error. errno contains the reason
     */
    int bind(int portNumber);

    /**
     * Binds this ServerSocket to specific address and port number.  If the host has multiple addresses,
     * you must specify a single address.  You cannot specify the host name.
     *
     * @return 0 success, -1 error. errno contains the reason
     *
     * throws runtime_error - throw if any of these conditions are true:
     *
     * 1. If this ServerSocket has been closed or disposed
     * 2. If the address specified cannot be resolved to an
     */
    int bind(const char* listeningAddress, int portNumber);

    /**
     * Closes this socket.
     */
    void close();

    /**
     * Use this method if this Socket object is only used to create or assign to another socket
     * object.  It will clear the internal socket descriptor and client address to ensure
     * that closing or destroying this object will not close the open socket.
     */
    void dispose();

    /**
     * Indicates if the ServerSocket is open or closed.  Open or closed depends
     * on the value of the underlying socket descriptor variable.  If the value is non-zero,
     * it is assumed to be open.  If zero, it is closed.  However, if someone were to pull
     * the socket descriptor from this object and manually close it, all bets are off.
     *
     * @return true if the socket is open.  Closed otherwise.
     */
    bool isOpen() const;

    /**
     * Obtain the socket descriptor.  This method is provided as a convenience method so that
     * the socket descriptor can be used with either epoll or select.  Never call this method
     * and then call
     * @code ::close()
     * @endcode
     *  on the returned socket descriptor.
     *
     * @return int - A socket descriptor
     */
    int fileDescriptor() const;

    /**
     * Assigns this ServerSocket to another.  Since calling this will result in two objects with
     * a reference to the underlying socket descriptor, you should call dispose() on the other
     * ServerSocket to avoid accidently closing the underlying descriptor.
     *
     * @param otherServerSocket Another ServerSocket
     *
     * @return ServerSocket& reference to the newly assigned ServerSocket
     */
    ServerSocket& operator=(const ServerSocket& otherServerSocket);

    static ServerSocket& open(int& status, bool configureBlocking = false);


private:
    int _sockfd;
    struct sockaddr_in _sockaddr;
};

} /* namespace common */
} /* namespace zios */

#endif /* SOCKETS_H_ */
