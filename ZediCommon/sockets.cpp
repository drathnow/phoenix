/** @file sockets.cpp
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
#include "sockets.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <stdexcept>
#include <sstream>
#include <cassert>

namespace zios {
namespace common {

using namespace std;

Socket::Socket() :
        _sockfd(0), _portNumber(0) {
}

Socket::Socket(const char* address, int portNumber) :
        _sockfd(0), _portNumber(portNumber) {
    ::memset((char *) &_clientAddress, 0, sizeof(struct sockaddr_in));
    _clientAddress.sin_family = AF_INET;
    _clientAddress.sin_addr.s_addr = ::inet_addr(address);
    _clientAddress.sin_port = htons(_portNumber);
}

Socket::Socket(const Socket& otherSocket) {
    _sockfd = otherSocket._sockfd;
    _portNumber = otherSocket._portNumber;
    _clientAddress = otherSocket._clientAddress;
}

Socket& Socket::operator=(const Socket& otherSocket) {
    _sockfd = otherSocket._sockfd;
    _portNumber = otherSocket._portNumber;
    _clientAddress = otherSocket._clientAddress;
    return *this;
}

Socket::~Socket() {
    close();
}

int Socket::open() {
    if (_sockfd != 0)
        throw runtime_error("The socket is already open");

    ::memset((char *) &_clientAddress, 0, sizeof(struct sockaddr_in));
    _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd <= 0)
        return -1;
    return 0;
}

int Socket::configureBlocking(bool isBlocking) {
    assert(_sockfd != 0);
    if (isBlocking == false) {
        int opts;
        if ((opts = ::fcntl(_sockfd, F_GETFL)) < 0)
            return -1;
        if (::fcntl(_sockfd, F_SETFL, opts | O_NONBLOCK) < 0)
            return -1;
    }
    return 0;
}


int Socket::connect() {
    return ::connect(_sockfd, (struct sockaddr *) &_clientAddress, sizeof(_clientAddress));
}

void Socket::close() {
    if (_sockfd != 0)
        ::close(_sockfd);
    _sockfd = 0;
}

bool Socket::isOpen() const {
    return _sockfd != 0;
}

void Socket::dispose() {
    _sockfd = 0;
    ::memset((char *) &_clientAddress, 0, sizeof(struct sockaddr_in));
}

int Socket::receive(void* buffer, int bufferSize) {
    return ::read(_sockfd, buffer, bufferSize);
}

int Socket::send(const void* data, int dataLength) {
    return ::write(_sockfd, data, dataLength);
}

int Socket::fileDescriptor() const {
    return _sockfd;
}

ServerSocket::ServerSocket() : _sockfd(0) {
    ::memset((char *) &_sockaddr, 0, sizeof(struct sockaddr_in));
}

ServerSocket::ServerSocket(const ServerSocket& otherServerSocket) {
    _sockfd = otherServerSocket._sockfd;
    _sockaddr = otherServerSocket._sockaddr;
}

ServerSocket::~ServerSocket() {
    if (_sockfd > 0)
        ::close(_sockfd);
}

int ServerSocket::fileDescriptor() const {
    return _sockfd;
}

int ServerSocket::open() {
    assert(_sockfd == 0);
    ::memset((char *) &_sockaddr, 0, sizeof(struct sockaddr_in));
    _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd <= 0)
        return -1;
    return 0;
}

int ServerSocket::configureBlocking(bool isBlocking) {
    assert(_sockfd != 0);
    if (isBlocking == false) {
        int opts;
        if ((opts = ::fcntl(_sockfd, F_GETFL)) < 0)
            return -1;
        if (::fcntl(_sockfd, F_SETFL, opts | O_NONBLOCK) < 0)
            return -1;
    }
    return 0;
}


int ServerSocket::bind(const char* listeningAddress, int portNumber) {
    unsigned long tcp_ia = 0;

    if (_sockfd == 0)
        throw runtime_error("The socket is closed");

    ::memset((char *) &_sockaddr, 0, sizeof(struct sockaddr_in));

    if (listeningAddress == NULL) {
        tcp_ia = htonl(INADDR_ANY);
    } else {
        tcp_ia = ::inet_addr(listeningAddress);
        if (tcp_ia == INADDR_NONE) {
            struct hostent * hep;
            hep = ::gethostbyname(listeningAddress);
            if ((hep == NULL) || (hep->h_addrtype != AF_INET || !hep->h_addr_list[0])) {
                stringstream ss;
                ss << "Cannot resolve listening address " << listeningAddress ;
                throw runtime_error(ss.str());
            }
            if (hep->h_addr_list[1] != NULL) {
                stringstream ss;
                ss << "Host " << listeningAddress << " has multiple addresses. You must choose one explicitly";
                throw runtime_error(ss.str());
            }
            tcp_ia = ((struct in_addr *) (hep->h_addr))->s_addr;
        }
    }

    int flag = 1;
    if (::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &flag, sizeof(flag)) < 0)
        return -1;

    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = tcp_ia;
    _sockaddr.sin_port = htons(portNumber);

    if (::bind(_sockfd, (struct sockaddr*) &_sockaddr, sizeof(_sockaddr)) < 0)
        return -1;

    return 0;
}

int ServerSocket::bind(int portNumber) {
    return bind(NULL, portNumber);
}

int ServerSocket::listen(int backlog) {
    if (_sockfd == 0)
        throw runtime_error("The socket is not open");

    if (::listen(_sockfd, backlog) < 0)
        return -1;

    return 0;
}

int ServerSocket::accept(Socket& returnSocket) {
    if (_sockfd == 0)
        throw runtime_error("The socket is not open");

    int newSocketFd;
    returnSocket._sockfd = 0;
    ::memset((char *) &returnSocket._clientAddress, 0, sizeof(struct sockaddr_in));
    socklen_t length = sizeof(returnSocket._clientAddress);
    if ((newSocketFd = ::accept(_sockfd, (struct sockaddr *) &returnSocket._clientAddress, &length)) < 0)
        return -1;
    returnSocket._sockfd = newSocketFd;
    return 0;
}

bool ServerSocket::isOpen() const {
    return _sockfd != 0;
}

void ServerSocket::close() {
    if (_sockfd > 0)
        ::close(_sockfd);
    _sockfd = 0;
}

void ServerSocket::dispose() {
    _sockfd = 0;
    ::memset((char *) &_sockaddr, 0, sizeof(struct sockaddr_in));
}

ServerSocket& ServerSocket::operator=(const ServerSocket& otherServerSocket) {
    _sockfd = otherServerSocket._sockfd;
    _sockaddr = otherServerSocket._sockaddr;
    return *this;
}

} /* namespace common */
} /* namespace zios */
