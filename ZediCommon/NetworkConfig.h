/** @file NetworkConfig.h
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 22, 2018 by daver:
 * @endhistory
 */
#ifndef NETWORKCONFIG_H_
#define NETWORKCONFIG_H_

#include "File.h"
#include <string>

namespace zios {
namespace common {

//
// Export keys we are searching for when scanning the file
//
#define DHCPD_ENABLE_KEY                "DHCPD_ENABLE"
#define DHCP_ARG_KEY                    "DHCP_ARG"
#define IP_ADDRESS_KEY                  "IPADDR0"
#define SUBNET_MASK_KEY                 "NETMASK0"
#define DEF_GATEWAY_KEY                 "GATEWAY0"

//
// When saving, we make this many backup copies of rc.conf, starting with ascending numbers,
// lowest number being the latest. The latest is always rc.conf, the next is rc.conf.1, then
// rc.conf.2, up to rc.conf.<MAX_FILE_COUNT>
//
#define MAX_FILE_COUNT                  3

//
// The location of the various files.
#ifdef _ONARM
#define RC_CONF                         "/etc/rc.d/rc.conf"
#define RC_CONF_BAD                     "/etc/rc.d/rc.conf.bad"
#else
#define RC_CONF                         "~/etc/rc.d/rc.conf"
#define RC_CONF_BAD                     "~/etc/rc.d/rc.conf.bad"
#endif

//
// Default values as define in the original rc.conf file
//
#define DEFAULT_SUBNET_MASK             "255.255.255.0"
#define DEFAULT_GATEWAY                 ""
#define DEFAULT_IP_ADDRESS              "192.168.13.1"
#define DEFAULT_DHCPD_SERVER_ENABLE     true
#define DEFAULT_DHCPD_CLIENT_ENABLE     false

/**
 * The NetworkConfig class is a wrapper around the /etc/rc.d/rc.conf file.  This file controls
 * the setup of the Linux network components on the field gateway.  This class takes care of
 * parsing the file and noting the relavent values so they can be manipulated.
 *
 * When constructed, the rc.conf file (or similar file) can be passed into the constructor.  It will parse
 * the file and note relavent values. Clients can then manuiplate vairous setting by setter and getter methods.
 * Once all configuration changes are done, the client must call the save() method to persist values to
 * persistent storage.
 *
 * The original version of the rc.conf file on the Field Gateway did not contain a separate variable that determined
 * if the DHCPD daemon was started.  It was started by default.  To facilitate deploying the FIG into networks
 * where it must have a static IP but no DHCP server running, we added the DHCPD_ENABLE variable that
 * determines if the DHCPD sever shouldb started at system init time.  If the rc.conf file scanned by this
 * class does not have it, the variable will added when the save() method is called.
 *
 */
class NetworkConfig {
public:
    /**
     * Constructs a NeworkConfig object that will parse and make changes to any rc.conf file.  While this
     * class will parse the entire file, it will only effect four parameters:
     *
     *  DHCPD_ENABLE - Defines if the DHCP Daemon (server) is started
     *
     *  IPADDR0 - This variable serves two purposes:
     *
     *      1. If it is suffixed with the string ".dhcp", it inidcates that the FIG should use
     *         DHCP to obtain its IP address.  In most cases like this, the value of this variable will be
     *         "<ip-address>.dhcp", where the <ip-address> is ignored.
     *
     *      2. If it does not contain the ".dhcp" suffix, the <ip-address> is used as the static
     *         IP address for the FIG.
     *
     *  NETMASK0 - Defines the subnet mask for this gateway.  It is only used if the DHCP client is disabled.
     *
     *  GATEWAY0 - Defines the default gateway for the FIG.
     *
     * Not check is made by this class to verify that the specified file exists.  Any error that could occur
     * because it does not exist will be detected by the attempt to open the file. Therefore, it is left up to
     * the caller to ensure the exists if he wishes to perfor more thorough error handling.
     *
     * @throws Exception - This constructor throws exception if any of the following conditions
     * occur:
     *
     *      - The rc.conf file cannot be found or opened.
     *      - The expected export lines of the rc.conf file has an invalid format.
     */
    NetworkConfig(const File& rcConfFile);
    ~NetworkConfig();

    /**
     * Enables/disbaled the DHCP Daemon (server) process
     *
     * @param enableDisable - True to enable, false to disable it.
     */
    void setDhcpServerEnabled(bool enableDisable);

    /**
     * Indicates if the DHCPS Daemon (server) is enabled.
     *
     * @return true if enable, false if disabled.
     */
    bool isDhcpServerEnabled() const;

    /**
     * Enables/disbaled the DHCP client
     *
     * @param enableDisable - True to enable, false to disable it.
     */
    void setDhcpClientEnabled(bool enableDisable);

    /**
     * Indicates if the DHCPS Client is enabled.
     *
     * @return true if enable, false if disabled.
     */
    bool isDhcpClientEnabled() const;

    /**
     * Sets the static IP address for the fig
     *
     * @param ipAddress - IP Address string
     *
     * @return true if the address is valid and has been set.  false if the
     * address does not have a valid IPv4 address format
     */
    bool setIpAddress(const std::string& ipAddress);

    /**
     * Returns the IP Address
     *
     * @return string with the IP Address.
     */
    const std::string& ipAddress() const;

    /**
     * Set the Subnet mask.
     *
     * @param subnetMask - Subnet mask.  No
     *
     * @return true if the subnet mask if valid and has been set.  false if the
     * address does not have a valid IPv4 address format
     */
    bool setSubnetMask(const std::string& subnetMask);

    /**
     * Returns the subnet mask
     *
     * @return subnet mask as a string
     */
    const std::string& subnetMask() const;

    /**
     * Sets the default gateway address.
     *
     * @param defaultGateway - Default gateway IP Address.
     *
     * @return true if the IP Address if valid and has been set.  false if the
     * address does not have a valid IPv4 address format
     */
    bool setDefaultGateway(const std::string& defaultGateway);

    /**
     * Return the default gateway address
     *
     * @return Default gateway address as a string
     */
    const std::string& defaultGateway() const;

    /**
     * Restores the Network Configuration to its default values
     *
     */
    void restoreDefaults();

    int save();

private:
    File _rcConfFile;
    bool _dhcpServerEnabled;
    bool _dhcpClientEnabled;
    std::string _ipAddress;
    std::string _subnetMask;
    std::string _defaultGateway;
    bool _dhcpdKeyFound;

    void rename(File& existingFile, uint32_t index);
    void parseConfig();
};

} /* namespace watchdog */
} /* namespace zios */

#endif /* NETWORKCONFIG_H_ */
