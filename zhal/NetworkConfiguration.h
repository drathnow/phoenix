/** @file NetworkConfiguration.h 
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
#ifndef NETWORKCONFIGURATION_H_
#define NETWORKCONFIGURATION_H_

#include <string>

namespace zios {
namespace hal {

class NetworkConfiguration
{
public:
    virtual ~NetworkConfiguration() {}

    /**
      * Enables/disbaled the DHCP Daemon (server) process
      *
      * @param enableDisable - True to enable, false to disable it.
      */
     virtual void setDhcpServerEnabled(bool enableDisable) = 0;

     /**
      * Indicates if the DHCPS Daemon (server) is enabled.
      *
      * @return true if enable, false if disabled.
      */
     virtual bool isDhcpServerEnabled() const = 0;

     /**
      * Enables/disbaled the DHCP client
      *
      * @param enableDisable - True to enable, false to disable it.
      */
     virtual void setDhcpClientEnabled(bool enableDisable) = 0;

     /**
      * Indicates if the DHCPS Client is enabled.
      *
      * @return true if enable, false if disabled.
      */
     virtual bool isDhcpClientEnabled() const = 0;

     /**
      * Enables or disableds the USB device.  The USB device can will only run with a static
      * IP and a DHCP server enabled.
      *
      * @param enableDisable - true if enabled, false otherwise.
      *
      * @return true if success, false on fail
      */
     virtual bool enableUsb(bool enableDisable) = 0;

     /**
      * Indicates if the USB device is enabled
      *
      * @return true if enabled, false if not
      */
     virtual bool isUsbEnabled() const = 0;

     /**
      * Sets the static IP address for the fig
      *
      * @param ipAddress - IP Address string
      *
      * @return true if the address is valid and has been set.  false if the
      * address does not have a valid IPv4 address format
      */
     virtual bool setIpAddress(const std::string& ipAddress) = 0;

     /**
      * Returns the IP Address
      *
      * @return string with the IP Address.
      */
     virtual const std::string& ipAddress() const = 0;

     /**
      * Set the Subnet mask.
      *
      * @param subnetMask - Subnet mask.  No
      *
      * @return true if the subnet mask if valid and has been set.  false if the
      * address does not have a valid IPv4 address format
      */
     virtual bool setSubnetMask(const std::string& subnetMask) = 0;

     /**
      * Returns the subnet mask
      *
      * @return subnet mask as a string
      */
     virtual const std::string& subnetMask() const = 0;

     /**
      * Sets the default gateway address.
      *
      * @param defaultGateway - Default gateway IP Address.
      *
      * @return true if the IP Address if valid and has been set.  false if the
      * address does not have a valid IPv4 address format
      */
     virtual bool setDefaultGateway(const std::string& defaultGateway) = 0;

     /**
      * Return the default gateway address
      *
      * @return Default gateway address as a string
      */
     virtual const std::string& defaultGateway() const = 0;

     /**
      * Restores the Network Configuration to its default values
      *
      */
     virtual void restoreDefaults() = 0;

     /**
      * Loads from persistent storage
      *
      * @return 0 on success. -1 error.
      */
     virtual int load() = 0;

     /**
      * Save's the configuration to persistent storage.
      *
      * @return 0 on success. -1 error.
      */
     virtual int save() = 0;
};

} /* namespace hal */
} /* namespace zios */

#endif /* NETWORKCONFIGURATION_H_ */
