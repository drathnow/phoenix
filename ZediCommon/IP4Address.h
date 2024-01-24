/** @file IP4Address.h 
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
#ifndef IP4ADDRESS_H_
#define IP4ADDRESS_H_

#include <string>
#include <stdint.h>

namespace zios {
namespace common {

class IP4Address {
public:
    /**
     * Splits an IP4 address with optional network size into its components.
     *
     * Supported formats:
     *   a.b.c.d/x  a.b.c.d = standard IP address, x=subnet range
     *   a.b.c.d    a.b.c.d = standard IP address, x=uses default value supplied
     *   name       name = assumed to be a hostname, x=assumed to be 32
     *
     * x in the above example is the number of subnet bits.
     *
     * @param address IP address or hostname, with optional /xx network size
     * @param defaultNetworkSize the assumed size of the network if not specified
     */
    IP4Address(std::string address, int defaultNetworkSize = 32);

    /**
     * The hostname portion of the address.
     * If the input address was a hostname and not an ip address, this will be the same
     * as the input address.
     * @return the hostname matching the address
     */
    inline std::string getHostname() { return _hostname; }

    /**
     * The network portion of the address.
     * If the input address was a hostname and not an ip address, this will be blank.
     *
     * If the network size is 32, this will be the same as the hostname.
     * Otherwise, this will reflect the address with the appropriate "host" bits cleared.
     * @return the network maching the address and network size
     */
    inline std::string getNetwork() { return _network; }

    /**
     * The netmask matching the hostname as a dotted-quad string
     * @return string representation of netmask
     */
    inline std::string getNetmask() { return _netmask; }

    /**
     * How many bits in the hostname are used for the "network number"
     * @return the number of bits in the address used for the "network"
     */
    inline int getNetworkSize() { return _networkSize; }

    /**
     * Indicates if the address was decoded properly.
     * @return true if the address was valid
     */
    inline bool isValid() { return _valid; }

    /**
     * Indicates if the address reflects an IP address or a hostname
     * @return address is a numeric IP address
     */
    inline bool isNumeric() { return _numeric; }

    /**
     * Convert a binary representation of a IP4 address to a string
     * using the "dotted-quad" format
     *
     * @param address ipv4 address as an unsigned int
     * @return dotted quad representation
     */
    static std::string addressToString(unsigned int address);

    /**
     * Convert a network size represented by the number of bits in the network
     * to a 32-bit netmask.
     * @param netsize network size from 0-32.  Invalid values are trimmed to be valid.
     * @return 32-bit representation of the netmask
     */
    static unsigned int networkSizeToNetmask(int netsize);

    /**
     * Returns true if the IP address is a valid IPv4 address
     *
     * @param ipAddress - String address
     *
     * @return true if valid, false if the address has invalid format
     */
    static bool isValidAddress(const char* ipAddress);

    /**
     * Decodes an encoded IPv4 address.  The encoded address has the form:
     *
     *      <ip-address>[:portNumber]
     *
     *  Where the portnumber is optional. Examples:
     *
     *      1.2.3.4
     *      54.6.7.8:6234
     *      54.6.7.8/23:6234
     *
     * @param encodedString - An encoded IPv4 address with the above format
     *
     * @param retAddress - Place to return the address portion
     *
     * @param retPortNumber - Place to return the port number.  Will return -1 if no port number provided.
     *
     * @return true if the string was decoded. false if
     *
     *      1. The address provided is not an IPv4 address.
     *      2. The port number is invalid. i.e. 65535 <= portnumber <= 0.
     */
    static bool decodeEncodedString(const std::string& encodedString, std::string& retAddress, int32_t& retPortNumber);

private:
    std::string _hostname;
    std::string _network;
    std::string _netmask;
    int _networkSize;
    bool _valid;
    bool _numeric;
};



} /* namespace common */
} /* namespace zios */

#endif /* IP4ADDRESS_H_ */
