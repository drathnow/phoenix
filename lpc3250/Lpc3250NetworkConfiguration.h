/** @file Lpc3250NetworkConfiguration.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul. 11, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250NETWORKCONFIGURATION_H_
#define LPC3250NETWORKCONFIGURATION_H_

#include <zhal.h>
#include <File.h>
#include <istream>
#include <vector>

#define LPC3250_INTERFACES_CONFIG       "/etc/network/interfaces"
#define LPC3250_NETWORK_CONFIG          "/etc/network/network.conf"
#define LPC3250_AUTO_REGEX              "^auto *eth0 *$"
#define LPC3250_IFACE_REGEX             "^iface *eth0 *inet *[static|dhcp]"

#define LPC3250_DHCPD_ENABLED_VAR       "ETH0_DHCPD_ENABLED"
#define LPC3250_ETH0_DEF_ADDRESS        "192.168.13.1"
#define LPC2350_ETH0_DEF_SUBNET         "255.255.255.0"

#define LPC3250_USB0_ENABLED_VAR        "USB0_ENABLED"
#define LPC3250_USB0_ADDRESS_VAR        "USB0_IPADDR"
#define LPC3250_USB0_DEF_ADDRESS        "10.0.0.1/24"

#define LPC3250_KEEPIT_BEGIN            "#>>"
#define LPC3250_KEEPIT_END              "#<<"

namespace zios {
namespace lpc3250 {


/**
 * The Lpc3250NetworkConfiguration manages the network configuration for the LPC3250 platform.  The platform
 * contains two network interfaces: an Ethernet device (eth0) and a usb device (usb0).  By default, these
 * are both enabled with the eth0 being assigned the address 192.168.13.1 and usb0 10.0.0.1.  As well, there is
 * a DHCP server (UDCHPD) enabled that will accept requests on eth0.
 *
 * The network configuration is stored in two files:
 *
 *      /etc/network/interfaces     - The default Linux network configuration file
 *
 *      /etc/network/network.conf   - Specific file create to manage USB and UDCHPD configuration.
 *
 */
class Lpc3250NetworkConfiguration : public zios::hal::NetworkConfiguration
{
public:

    /**
     * Constructs a default Lpc3250NetworkConfiguration using default configuration files.  The default
     * files are:
     *
     *      /etc/network/interfaces     - Contains interface definitions.  Used by Linux Init process to configure
     *                                    the network interface eth0
     *
     *      /etc/network/network.conf   - Managed and maintained by ZIOS to control the setup of DHCPD and USB0
     *
     */
    Lpc3250NetworkConfiguration();

    /**
     * Constructs an Lpc3250NetworkConfiguration using information from the file specified by configFile.
     *
     * @param configFile
     *
     * @throws Exception - thrown if the file does not exist
     */
    Lpc3250NetworkConfiguration(const zios::common::File& interfaceFile, const zios::common::File& confFile);

    ~Lpc3250NetworkConfiguration();

    /**
      * Enables/disbaled the DHCP Daemon (server) process
      *
      */
     void setDhcpServerEnabled(bool enableDisable);

     /**
      * Indicates if the DHCPS Daemon (server) is enabled.
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
      * Enables or disableds the USB device.  The USB device can will only run with a static
      * IP and a DHCP server enabled.
      *
      * @param enableDisable - true if enabled, false otherwise.
      *
      * @return true if success, false on fail
      */
     bool enableUsb(bool enableDisable);

     /**
      * Indicates if the USB device is enabled
      *
      * @return true if enabled, false if not
      */
     bool isUsbEnabled() const;

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

     /**
      * Loads a configuration from persistent storage.
      *
      * @return 0 if loaded successfully. -1 if an error occurs
      */
     int load();

     /**
      * Save's the configuration to persistent storage.
      */
     int save();


     friend class Lpc3250NetworkConfigurationTest;

private:
     zios::common::File _interfaceFile;
     zios::common::File _confFile;
     bool _dhcpClientEnabled;
     bool _dhcpServerEnabled;
     std::string _ipAddress;
     std::string _subnetMask;
     std::string _defaultGateway;
     bool _usb0Enabled;
     std::string _usb0Address;
     std::vector<std::string> _networkConfLines;
     std::vector<std::string> _interfacesLines;


     int parseInterfacesStream(std::istream& inputStream);
     int parseNetworkConfStream(std::istream& inputStream);
     int saveInterfacesFile();
     int saveConfigFile();
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250NETWORKCONFIGURATION_H_ */
