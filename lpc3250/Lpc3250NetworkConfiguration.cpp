/** @file Lpc3250NetworkConfiguration.cpp 
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
#include "Lpc3250NetworkConfiguration.h"

#include <log4cIncludes.h>
#include <IP4Address.h>
#include <regex>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <StringUtl.h>

#define PARSE_LOOKING_FOR_AUTO      0
#define PARSE_IFACE                 1
#define PARSE_STATIC                2
#define PARSE_DONE                  3

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250.Lpc3250NetworkConfiguration");

Lpc3250NetworkConfiguration::Lpc3250NetworkConfiguration() :
        _interfaceFile(LPC3250_INTERFACES_CONFIG),
                _confFile(LPC3250_NETWORK_CONFIG),
                _dhcpClientEnabled(false),
                _dhcpServerEnabled(true),
                _ipAddress(LPC3250_ETH0_DEF_ADDRESS),
                _subnetMask(LPC2350_ETH0_DEF_SUBNET),
                _defaultGateway(""),
                _usb0Enabled(true),
                _usb0Address(LPC3250_USB0_DEF_ADDRESS)
{
}

Lpc3250NetworkConfiguration::Lpc3250NetworkConfiguration(const File &interfaceFile, const zios::common::File &confFile) :
        _interfaceFile(interfaceFile),
                _confFile(confFile),
                _dhcpClientEnabled(false),
                _dhcpServerEnabled(true),
                _ipAddress(LPC3250_ETH0_DEF_ADDRESS),
                _subnetMask(LPC2350_ETH0_DEF_SUBNET),
                _defaultGateway(""),
                _usb0Enabled(true),
                _usb0Address(LPC3250_USB0_DEF_ADDRESS)
{
}

Lpc3250NetworkConfiguration::~Lpc3250NetworkConfiguration()
{
    _networkConfLines.clear();
    _interfacesLines.clear();
}

int Lpc3250NetworkConfiguration::load()
{
    ifstream inputStream;

    inputStream.open(_interfaceFile.absolutePath(), ios_base::in);
    if (false == inputStream.is_open())
        LOG4CPLUS_WARN(logger, "Unable to open interfaces config - " << _interfaceFile.absolutePath() << ": " << ::strerror(errno));
    else {
        parseInterfacesStream(inputStream);
        inputStream.close();
    }

    inputStream.open(_confFile.absolutePath(), ios_base::in);
    if (false == inputStream.is_open())
        LOG4CPLUS_WARN(logger, "Unable to open network config - " << _confFile.absolutePath() << ": " << ::strerror(errno));
    else {
        parseNetworkConfStream(inputStream);
        inputStream.close();
    }

    return 0;
}

void Lpc3250NetworkConfiguration::setDhcpServerEnabled(bool enableDisable)
{
    _dhcpServerEnabled = enableDisable;
}

bool Lpc3250NetworkConfiguration::isDhcpServerEnabled() const
{
    return _dhcpServerEnabled;
}

void Lpc3250NetworkConfiguration::setDhcpClientEnabled(bool enableDisable)
{
    _dhcpClientEnabled = enableDisable;
    if (_dhcpClientEnabled) {
        _dhcpServerEnabled = false;
        _ipAddress.clear();
        _subnetMask.clear();
        _defaultGateway.clear();
    }
}

bool Lpc3250NetworkConfiguration::isDhcpClientEnabled() const
{
    return _dhcpClientEnabled;
}

bool Lpc3250NetworkConfiguration::setIpAddress(const std::string &ipAddress)
{
    if (false == IP4Address::isValidAddress(ipAddress.c_str()))
        return false;
    _ipAddress = ipAddress;
    return true;
}

bool Lpc3250NetworkConfiguration::isUsbEnabled() const
{
    return _usb0Enabled;
}

bool Lpc3250NetworkConfiguration::enableUsb(bool enableDisable)
{
    return _usb0Enabled = enableDisable;
}

const std::string& Lpc3250NetworkConfiguration::ipAddress() const
{
    return _ipAddress;
}

bool Lpc3250NetworkConfiguration::setSubnetMask(const std::string &subnetMask)
{
    _subnetMask = subnetMask;
    return true;
}

const std::string& Lpc3250NetworkConfiguration::subnetMask() const
{
    return _subnetMask;
}

bool Lpc3250NetworkConfiguration::setDefaultGateway(const std::string &defaultGateway)
{
    _defaultGateway = defaultGateway;
    return true;
}

const std::string& Lpc3250NetworkConfiguration::defaultGateway() const
{
    return _defaultGateway;
}

void Lpc3250NetworkConfiguration::restoreDefaults()
{
    _dhcpClientEnabled = false;
    _dhcpServerEnabled = true;
    _ipAddress = LPC3250_ETH0_DEF_ADDRESS;
    _subnetMask = LPC2350_ETH0_DEF_SUBNET;
    _defaultGateway = "";
    _usb0Enabled = true;
    _usb0Address = LPC3250_USB0_DEF_ADDRESS;
}

int Lpc3250NetworkConfiguration::saveInterfacesFile()
{
    string filename(_interfaceFile.name());
    filename.append(".bck");
    File backupFile(_interfaceFile.parent().c_str(), filename.c_str());

    filename = _interfaceFile.name();
    filename.append(".tmp");
    File tmpFile(_interfaceFile.parent().c_str(), filename.c_str());

    ofstream ofs(tmpFile.absolutePath());
    if (!ofs.is_open()) {
        LOG4CPLUS_ERROR(logger, "Unable to open " << tmpFile.absolutePath() << ": " << ::strerror(errno));
        return -1;
    }

    ofs << "#!!!WARNING!!! - This file is autogenerated.  Any changes you make could be wiped out. DO NOT EDIT THIS FILE" << endl;
    ofs << "auto lo" << endl;
    ofs << "iface lo inet loopback" << endl << endl;

    if (_dhcpClientEnabled) {
        ofs << "auto eth0" << endl;
        ofs << "iface eth0 inet dhcp" << endl;
    } else {
        if (_ipAddress.length() == 0 || _subnetMask.length() == 0)
            return -1;

        ofs << "auto eth0" << endl;
        ofs << "iface eth0 inet static" << endl;
        ofs << "  address " << _ipAddress << endl;
        ofs << "  netmask " << _subnetMask << endl;
        if (_defaultGateway.length() != 0)
            ofs << "  gateway " << _defaultGateway << endl;
    }

    if (_interfacesLines.size() > 0) {
        ofs << endl << LPC3250_KEEPIT_BEGIN << endl;
        for (vector<string>::iterator iter = _interfacesLines.begin(); iter != _interfacesLines.end(); iter++)
            ofs << *iter << endl;
        ofs << LPC3250_KEEPIT_END << endl;
    }

    ofs.close();

    //
    // If the original interfaces file is there, then we rename it.  However, there is always a chance that the
    // file was delete, for some reason.  If the latter is the case, we skip the rename.
    //
    if (_interfaceFile.exists() && !_interfaceFile.renameTo(backupFile)) {
        LOG4CPLUS_ERROR(logger, "Unable to rename " << _interfaceFile.absolutePath() << " to " << backupFile.absolutePath() << ": " << ::strerror(errno));
        return -1;
    }

    //
    // Now rename the backup file to the original file.
    //
    _interfaceFile.turf();
    if (!tmpFile.renameTo(_interfaceFile)) {
        LOG4CPLUS_ERROR(logger, "Unable to rename " << tmpFile.absolutePath() << " to " << _interfaceFile.absolutePath() << ": " << ::strerror(errno));
        backupFile.renameTo(_interfaceFile);
        backupFile.turf();
        return -1;
    }

    backupFile.turf();

    return 0;
}

int Lpc3250NetworkConfiguration::saveConfigFile()
{
    string filename(_confFile.name());
    filename.append(".bck");
    File backupFile(_confFile.parent().c_str(), filename.c_str());

    filename = _confFile.name();
    filename.append(".tmp");
    File tmpFile(_confFile.parent().c_str(), filename.c_str());

    ofstream ofs(tmpFile.absolutePath());
    if (!ofs.is_open()) {
        LOG4CPLUS_ERROR(logger, "Unable to open " << tmpFile.absolutePath() << ": " << ::strerror(errno));
        return -1;
    }

    ofs << "#!!!WARNING!!! - This file is autogenerated.  Any changes you make could be wiped out. DO NOT EDIT THIS FILE" << endl;
    ofs << LPC3250_DHCPD_ENABLED_VAR << "=" << (_dhcpServerEnabled ? "\"y\"" : "\"n\"") << endl;
    ofs << LPC3250_USB0_ENABLED_VAR << "=" << (_usb0Enabled ? "\"y\"" : "\"n\"") << endl;
    ofs << LPC3250_USB0_ADDRESS_VAR << "=\"" << _usb0Address << "\"" << endl;

    if (_networkConfLines.size() > 0) {
        ofs << endl << LPC3250_KEEPIT_BEGIN << endl;
        for (vector<string>::iterator iter = _networkConfLines.begin(); iter != _networkConfLines.end(); iter++)
            ofs << *iter << endl;
        ofs << LPC3250_KEEPIT_END << endl;
    }

    ofs.close();

    //
    // If the original network config file is there, then we rename it.  However, there is always a chance that the
    // file was delete, for some reason.  If the latter is the case, we skip the rename.
    //
    if (_confFile.exists() && !_confFile.renameTo(backupFile)) {
        LOG4CPLUS_ERROR(logger, "Unable to rename " << _confFile.absolutePath() << " to " << backupFile.absolutePath() << ": " << ::strerror(errno));
        return -1;
    }

    //
    // Now rename the backup file to the original file.
    //
    _confFile.turf();
    if (!tmpFile.renameTo(_confFile)) {
        LOG4CPLUS_ERROR(logger, "Unable to rename " << tmpFile.absolutePath() << " to " << _confFile.absolutePath() << ": " << ::strerror(errno));
        backupFile.renameTo(_interfaceFile);
        backupFile.turf();
        return -1;
    }

    backupFile.turf();

    return 0;
}

int Lpc3250NetworkConfiguration::save()
{
    //
    // Check the rules.
    //
    // 1. If the DHCP client is enabled, DHCP server must be disabled and IP address, netmask, and gateway must
    //    be clear.
    //
    if (_dhcpClientEnabled) {
        if (_dhcpServerEnabled || _ipAddress.length() > 0 || _subnetMask.length() > 0 || _defaultGateway.length() > 0) {
            LOG4CPLUS_ERROR(logger, "Invalid network configuration");
            return -1;
        }
    } else {
        //
        // If DHCP client is enabled, then you must provide IP address and netmask
        //
        if (_ipAddress.length() == 0 || _subnetMask.length() == 0) {
            LOG4CPLUS_ERROR(logger, "Invalid network configuration");
            return -1;
        }
    }

    return saveInterfacesFile() == 0 && saveConfigFile() == 0 ? 0 : -1;
}

int Lpc3250NetworkConfiguration::parseNetworkConfStream(std::istream &inputStream)
{
    string line, key, value;
    int state = 1;

    _networkConfLines.clear();
    while (getline(inputStream, line)) {
        switch (state) {
            case 1 :
                if (line.find(LPC3250_KEEPIT_BEGIN) == 0) {
                    state = 2;
                } else {

                    if (line.find("#") == 0)
                        continue;

                    if (StringUtl::stringToKeyValue(line, key, value)) {
                        value.erase(remove(value.begin(), value.end(), '\"'), value.end());
                        if (LPC3250_DHCPD_ENABLED_VAR == key)
                            _dhcpServerEnabled = (value[0] == 'y');

                        if (LPC3250_USB0_ENABLED_VAR == value)
                            _usb0Enabled = (value[0] == 'y');

                        if (LPC3250_USB0_ADDRESS_VAR == key)
                            _usb0Address = value;
                    }
                }
                break;

            case 2 :
                if (line.find(LPC3250_KEEPIT_END) == 0)
                    state = 1;
                else
                    _networkConfLines.push_back(line);
        }

    }

    return 0;
}

int Lpc3250NetworkConfiguration::parseInterfacesStream(istream &inputStream)
{
    string line;
    uint16_t parseState = PARSE_LOOKING_FOR_AUTO;
    std::regex *theRegEx = NULL;
    int state = 1;

    _interfacesLines.clear();
    while (getline(inputStream, line)) {
        switch (state) {
            case 1:
                if (line.find(LPC3250_KEEPIT_BEGIN) == 0) {
                    state = 2;
                } else {
                    if (line.find("#") == 0)
                        continue;

                    //
                    // Get rid of the fluff.
                    //
                    StringUtl::trim(line);

                    switch (parseState) {
                        case PARSE_DONE:
                            if (line.find(LPC3250_KEEPIT_BEGIN) == 0)
                                state = 2;
                            break;

                        case PARSE_LOOKING_FOR_AUTO:
                            if (theRegEx == NULL)
                                theRegEx = new regex(LPC3250_AUTO_REGEX, std::regex_constants::ECMAScript | std::regex_constants::icase);
                            if (std::regex_search(line, *theRegEx)) {
                                delete theRegEx;
                                theRegEx = new regex(LPC3250_IFACE_REGEX, std::regex_constants::ECMAScript | std::regex_constants::icase);
                                parseState = PARSE_IFACE;
                            }
                            break;

                        case PARSE_IFACE:
                            if (std::regex_search(line, *theRegEx)) {
                                if (line.find("dhcp") != string::npos) {
                                    _dhcpClientEnabled = true;
                                    _ipAddress.clear();
                                    _subnetMask.clear();
                                    _defaultGateway.clear();
                                    parseState = PARSE_DONE;
                                } else if (line.find("static") != string::npos) {
                                    _dhcpClientEnabled = false;
                                    parseState = PARSE_STATIC;
                                }
                            } else {
                                delete theRegEx;
                                LOG4CPLUS_ERROR(logger, "Parse Error: 'iface' line expected but not found");
                                return -1;
                            }
                            break;

                        case PARSE_STATIC:
                            if (line.find("address") != string::npos) {
                                _ipAddress = line.substr(line.find(' ') + 1);
                                StringUtl::trim(_ipAddress);
                            } else if (line.find("gateway") != string::npos) {
                                _defaultGateway = line.substr(line.find(' ') + 1);
                                StringUtl::trim(_defaultGateway);
                            }
                            if (line.find("netmask") != string::npos) {
                                _subnetMask = line.substr(line.find(' ') + 1);
                                StringUtl::trim(_subnetMask);
                            }
                            break;
                    }
                }

                break;

            case 2:
                if (line.find(LPC3250_KEEPIT_END) == 0)
                    state = 1;
                else
                    _interfacesLines.push_back(line);
                break;
        }
    }

    delete theRegEx;

    //
    // Sanity check.  If we are not static, then we must have address, gateway, and subnet mask
    //
    if (false == _dhcpClientEnabled) {
        if (_subnetMask.length() == 0 || _defaultGateway.length() == 0 || _subnetMask.length() == 0) {
            LOG4CPLUS_ERROR(logger, "Parse Error: static is missing parameter");
        }
    }

    return 0;
}

} /* namespace lpc3250 */
} /* namespace zios */

