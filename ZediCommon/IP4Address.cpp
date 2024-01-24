/** @file IP4Address.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 24, 2018 by daver:
 * @endhistory
 */
#include "IP4Address.h"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include <cstdio>

#include "StringUtl.h"
#include "InvalidDeviceException.h"

namespace zios {
namespace common {

using namespace std;
using namespace zios::common;

IP4Address::IP4Address(std::string address, int defaultNetworkSize) {
    StringUtl::trim(address);
    // empty after trimming
    if (address.size() == 0) {
        _valid = false;
        _numeric = false;
        _networkSize=32;
        _netmask="255.255.255.255";
        _network="";
        _hostname="";
        return;
    }

    unsigned int a, b, c, d, n;
    int scan;

    // form:  a.b.c.d/n
    scan = ::sscanf(address.c_str(), "%u.%u.%u.%u/%u", &a, &b, &c, &d, &n);

    // assume a non-empty address that doesn't have 4 decimal numbers separated by .s is valid
    // but not numeric.  Set the network size to 32 indicating it's a hostname only
    if (scan < 4 ) {
        _valid = true;
        _numeric = false;
        _networkSize=defaultNetworkSize;
        _netmask=addressToString(networkSizeToNetmask(defaultNetworkSize));
        // can't determine the network because we don't have a numeric host to start from
        _network="";

        // look for the / and trim it off if found
        size_t slash_pos = address.find('/');
        if (slash_pos != std::string::npos) {
            _hostname = address.substr(0, slash_pos);
            // if more after the /, try parse in the network size
            if (address.size() > slash_pos+1) {
                if (sscanf(address.substr(slash_pos+1, string::npos).c_str(), "%u", &n) == 1) {
                    // found a number for n - validate it
                    if (n < 1) {
                        n = 1;
                    }
                    if (n > 32) {
                        n = 32;
                    }
                    _netmask = addressToString(networkSizeToNetmask(n));
                    _networkSize=n;
                }
            }
        }
        else {
            _hostname=address;
        }
        // not numeric
        return;
    }

    // default network size to 32 if not found on line
    if (scan == 4) {
        n=defaultNetworkSize;
    }

    // limit network size
    if (n < 1) {
        n = 1;
    }
    if (n > 32) {
        n = 32;
    }

    // check for invalid ranges
    if ( (a > 255) || (b > 255) || (c > 255) || (d > 255) ) {
        _valid = false;
        _numeric = false;
        _networkSize=32;
        _netmask="255.255.255.255";
        _network="";
        _hostname="";
        return;
    }

    // convert dotted-quad to 32-bit int
    unsigned int numericHost = (a << 24) | (b << 16) | (c << 8) | d;

    unsigned int mask = networkSizeToNetmask(n);

    // apply mask to host to create network number
    unsigned int numericNetwork = numericHost & mask;

    _valid = true;
    _numeric = true;
    _networkSize = n;
    _hostname = addressToString(numericHost);
    _network = addressToString(numericNetwork);
    _netmask = addressToString(mask);
}

std::string IP4Address::addressToString(unsigned int address) {
    unsigned int a, b, c, d;
    stringstream ss;

    // note - check if top bits carry down
    a = (address >> 24) & 0xFF;
    b = (address >> 16) & 0xFF;
    c = (address >> 8) & 0xFF;
    d = address & 0xFF;

    ss << a << "." << b << "." << c << "." << d;
    return ss.str();
}

unsigned int IP4Address::networkSizeToNetmask(int netsize) {
    if (netsize < 0) {
        netsize = 0;
    }
    if (netsize > 32) {
        netsize = 32;
    }
    // convert network number to 32-bit mask
    unsigned int mask = 0;
    for (unsigned int i = 32-netsize; i < 32; i++) {
        mask |= 1 << i;
    }
    return mask;
}

bool IP4Address::isValidAddress(const char* ipAddress) {
    size_t pos = 0;
    struct sockaddr_in sa;
    string address(ipAddress);
    if ((pos = address.find("/")) != string::npos)
        address = address.substr(0, pos);
    return ::inet_pton(AF_INET, address.c_str(), &(sa.sin_addr));
}

bool IP4Address::decodeEncodedString(const std::string& encodedString, std::string& retAddress, int32_t& retPortNumber) {
    string::size_type pos = 0;
    if ((pos = encodedString.find(':')) == string::npos) {
        retAddress = encodedString;
        retPortNumber = -1;
    } else {
        retAddress = encodedString.substr(0, pos);
        string portStr = encodedString.substr(pos+1);
        retPortNumber = ::atoi(portStr.c_str());
    }
    return isValidAddress(retAddress.c_str());
}


}} // namespace
