/** @file NetworkConfig.cpp
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

#include "NetworkConfig.h"

#include <stdint.h>
#include <stddef.h>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cerrno>

#include "StringUtl.h"
#include "Exception.h"
#include "IP4Address.h"

namespace zios {
namespace common {

using namespace std;

static bool boolValueForChar(char boolChar) {
    switch (boolChar) {
        case 'y' :
        case 'Y' :
            return true;
    }
    return false;
}

static void trimQuotes(string& quotedString) {
    if (quotedString.size() > 1 && quotedString[0] == '\"')
        quotedString = quotedString.substr(1);
    if (quotedString.size() > 0 && quotedString[quotedString.size()-1] == '\"')
        quotedString.resize(quotedString.size()-1);
}

NetworkConfig::NetworkConfig(const zios::common::File& rcConfFile) :
        _rcConfFile(rcConfFile),
        _dhcpServerEnabled(DEFAULT_DHCPD_SERVER_ENABLE),
        _dhcpClientEnabled(DEFAULT_DHCPD_CLIENT_ENABLE),
        _ipAddress(DEFAULT_IP_ADDRESS),
        _subnetMask(DEFAULT_SUBNET_MASK),
        _defaultGateway(DEFAULT_GATEWAY),
        _dhcpdKeyFound(false) {
    parseConfig();
}

NetworkConfig::~NetworkConfig() {
}

void NetworkConfig::setDhcpServerEnabled(bool enabled) {
    _dhcpServerEnabled = enabled;
}

bool NetworkConfig::isDhcpServerEnabled() const {
    return _dhcpServerEnabled;
}

void NetworkConfig::setDhcpClientEnabled(bool enabled) {
    _dhcpClientEnabled = enabled;
}

bool NetworkConfig::isDhcpClientEnabled() const {
    return _dhcpClientEnabled;
}

bool NetworkConfig::setIpAddress(const std::string& ipAddress) {
    if (false == IP4Address::isValidAddress(ipAddress.c_str()))
        return false;
    _ipAddress = ipAddress;
    return true;
}

const std::string& NetworkConfig::ipAddress() const {
    return _ipAddress;
}

bool NetworkConfig::setSubnetMask(const std::string& subnetMask) {
    if (false == IP4Address::isValidAddress(subnetMask.c_str()))
        return false;
    _subnetMask = subnetMask;
    return true;
}

const std::string& NetworkConfig::subnetMask() const {
    return _subnetMask;
}

bool NetworkConfig::setDefaultGateway(const std::string& defaultGateway) {
    if (defaultGateway.length() > 0) {
        if (false == IP4Address::isValidAddress(defaultGateway.c_str()))
            return false;
    }
    _defaultGateway = defaultGateway;
    return true;
}

const std::string& NetworkConfig::defaultGateway() const {
    return _defaultGateway;
}

void NetworkConfig::restoreDefaults() {
    _dhcpClientEnabled = DEFAULT_DHCPD_CLIENT_ENABLE;
    _dhcpServerEnabled = DEFAULT_DHCPD_SERVER_ENABLE;
    _ipAddress = DEFAULT_IP_ADDRESS;
    _subnetMask = DEFAULT_SUBNET_MASK;
    _defaultGateway = DEFAULT_GATEWAY;
}

void NetworkConfig::rename(File& existingFile, uint32_t index) {
    if (index > MAX_FILE_COUNT) {
        existingFile.turf();
    } else {
        ostringstream oss;
        oss << _rcConfFile.absolutePath() << "." << index;
        File file(oss.str().c_str());
        if (file.exists())
            rename(file, index+1);
        existingFile.renameTo(file);
    }
}

int NetworkConfig::save() {
    File parent = _rcConfFile.parentFile();
    File newRcFile(File::createTempFilename("rcmod", "tmp", &parent));
    ofstream ofs(newRcFile.absolutePath(), fstream::out);
    if (ofs.bad())
        return -1;
    ifstream ifs(_rcConfFile.absolutePath(), fstream::in);
    if (ifs.bad())
        return -1;

    string nextLine;
    while (getline(ifs, nextLine)) {
        StringUtl::trim(nextLine);

        //
        // This first case deals with a situation where the rc.conf file does not have the DHCPD_ENABLE
        // key. If it was not found at construction time, this class will flag the condition and add
        // the key now.
        //
        if (nextLine.find(DHCP_ARG_KEY) != string::npos && _dhcpdKeyFound == false) {
            nextLine.append("\nexport DHCPD_ENABLE=");
            nextLine.append(_dhcpServerEnabled ? "y" : "n");
        } else if (nextLine.find(DHCPD_ENABLE_KEY) != string::npos) {
            nextLine = "export DHCPD_ENABLE=";
            nextLine.append(_dhcpServerEnabled ? "y" : "n");
        } else if (nextLine.find(IP_ADDRESS_KEY) != string::npos) {
            nextLine = "export IPADDR0=\"" + _ipAddress;
            nextLine.append(_dhcpClientEnabled ? ".dhcp" : "");
            nextLine.append("\"");
        } else if (nextLine.find(SUBNET_MASK_KEY) != string::npos) {
            nextLine = "export NETMASK0=\"" + _subnetMask + "\"";
        } else if (nextLine.find(DEF_GATEWAY_KEY) != string::npos) {
            nextLine = "export GATEWAY0=\"" + _defaultGateway + "\"";
        }
        ofs << nextLine << endl;
    }
    ofs.close();
    ifs.close();
    rename(_rcConfFile, 1);
    newRcFile.renameTo(_rcConfFile);
    return 0;
}

void NetworkConfig::parseConfig() {
    ifstream ifs(_rcConfFile.absolutePath(), fstream::in);
    if (ifs.bad())
        THROW_EXCEPTION("Unable to open file " << _rcConfFile.absolutePath() << ": " << ::strerror(errno));

    string nextLine;
    size_t pos;
    while (getline(ifs, nextLine)) {
        StringUtl::trim(nextLine);
        if (nextLine.find(DHCPD_ENABLE_KEY) != string::npos) {
            if ((pos = nextLine.find('=')) == string::npos)
                THROW_EXCEPTION("rc.conf file has bad format.  Missing '=' in DHCPD_ENABLE export");
            _dhcpdKeyFound = true;
            char yOrN = 'n';
            if (pos >= nextLine.size()-2) {
                string foo = nextLine.substr(pos+1);
                trimQuotes(foo);
                yOrN = foo[0];
            }
            _dhcpServerEnabled = boolValueForChar(yOrN);
        } else if (nextLine.find(IP_ADDRESS_KEY) != string::npos) {
            _ipAddress.clear();
            if ((pos = nextLine.find('=')) == string::npos)
                THROW_EXCEPTION("rc.conf file has bad format.  Missing '=' in IPADDR0 export");
            if (pos < nextLine.size()) {
                _ipAddress = nextLine.substr(pos+1);
                trimQuotes(_ipAddress);
                std::transform(_ipAddress.begin(), _ipAddress.end(), _ipAddress.begin(), ::tolower);
                if (_ipAddress.size() > 0) {
                    if ((pos = _ipAddress.find(".dhcp")) == string::npos)
                        _dhcpClientEnabled = false;
                    else {
                        _ipAddress.resize(pos);
                        _dhcpClientEnabled = true;
                    }
                }
            }
        } else if (nextLine.find(SUBNET_MASK_KEY) != string::npos) {
            _subnetMask.clear();
            if ((pos = nextLine.find('=')) == string::npos)
                THROW_EXCEPTION("rc.conf file has bad format.  Missing '=' in NETMASK0 export");
            if (pos < nextLine.size()) {
                _subnetMask = nextLine.substr(pos+1);
                trimQuotes(_subnetMask);
            }
        } else if (nextLine.find(DEF_GATEWAY_KEY) != string::npos) {
            _defaultGateway.clear();
            if ((pos = nextLine.find('=')) == string::npos)
                THROW_EXCEPTION("rc.conf file has bad format.  Missing '=' in NETMASK0 export");
            if (pos < nextLine.size()) {
                _defaultGateway = nextLine.substr(pos+1);
                trimQuotes(_defaultGateway);
            }
        }
    }
}

} /* namespace watchdog */
} /* namespace zios */
