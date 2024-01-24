/** @file RcConfigTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 23, 2018 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <fstream>
#include <string>
#include <File.h>
#include <NetworkConfig.h>

namespace zios {
namespace common {

using namespace std;

const char* RC_CONFIG_ORIG[] = {
        "all_services=\"mount-proc-sys mdev udev hostname devfsd depmod modules filesystems syslog network inetd portmap dropbear sshd boa smb dhcpd settime qtopia watchdog gtk2 pango dbus avahi\"\n",
        "all_services_r=\"avahi dbus pango gtk2 watchdog qtopia settime dhcpd smb boa sshd dropbear portmap inetd network syslog filesystems modules depmod devfsd hostname udev mdev mount-proc-sys\"\n",
        "\n",
        "cfg_services=\"mount-proc-sys mdev  hostname  depmod modules filesystems syslog network inetd  dropbear     settime      \"\n",
        "\n",
        "cfg_services_r=\"      settime     dropbear  inetd network syslog filesystems modules depmod  hostname  mdev mount-proc-sys\"\n",
        "\n",
        "export HOSTNAME=\"zedi3250\"\n",
        "export NTP_SERVER=\"\"\n",
        "export MODLIST=\"\"\n",
        "export RAMDIRS=\"/tmp /var\"\n",
        "export TMPFS=\"tmpfs\"\n",
        "export TMPFS_SIZE=\"3072k\"\n",
        "export READONLY_FS=\"\"\n",
        "export INETD_ARGS=\"\"\n",
        "export BOA_ARGS=\"\"\n",
        "export SMBD_ARGS=\"\"\n",
        "export NMBD_ARGS=\"\"\n",
        "export DHCP_ARG=\"\"\n",
        "export DEPLOYMENT_STYLE=\"RAMDISK\"\n",
        "export DHCPD_ENABLE=y\n",
        "export SYSCFG_DHCPC_CMD=\"udhcpc -b -i \"\n",
        "export DROPBEAR_ARGS=\"\"\n",
        "export SYSCFG_IPV4LL_CMD=\"avahi-autoipd --daemonize \"\n",
        "# net interface 0\n",
        "export SYSCFG_IFACE0=y\n",
        "export INTERFACE0=\"eth0\"\n",
        "export IPADDR0=\"192.168.13.1\"\n",
        "export NETMASK0=\"255.255.255.0\"\n",
        "export BROADCAST0=\"192.168.13.255\"\n",
        "export GATEWAY0=\"192.168.13.0\"\n",
        "export NAMESERVER0=\"\"\n",
        "\n",
        "\n",
        0};


class NetworkConfigTest : public testing::Test {
public:
    NetworkConfigTest(const char* testName) : _rcConfFile(testName) {}
    ~NetworkConfigTest() {}

    void SetUp() {
        _rcConfFile.turf();
        _rcConfFile.deleteOnExit();

        ofstream ofs(_rcConfFile.absolutePath(), fstream::trunc | fstream::out);
        ASSERT_FALSE(ofs.bad());

        int i = 0;
        const char* ptr;
        while ((ptr = RC_CONFIG_ORIG[i++]) != 0) {
            string str(ptr);
            ofs << morphRcConfLine(str);
        }
        ofs.close();
    }

    void TearDown() {
        _rcConfFile.turf();
        string saveFileName(_rcConfFile.absolutePath());
        saveFileName.append(".1");
        File saveFile(saveFileName);
        saveFile.turf();
    }

    bool lineExistsInRcConf(const string& theLine) {
        bool result = false;
        ifstream ifs(_rcConfFile.absolutePath(), fstream::in);
        string nextLine;
        while (getline(ifs, nextLine)) {
            if (nextLine == theLine) {
                result = true;
                break;
            }
        }
        ifs.close();
        return result;
    }

    //
    // Derived classes can override this to morph the rc.conf string into something other
    // than what is defined in the test
    //
    virtual string& morphRcConfLine(string& rcConfLine) {
        return rcConfLine;
    }

    istringstream _iss;
    File _rcConfFile;
};

class WhenNetworkConfigIsConstructedWithDhcpDisabledAndStaticIpAddress : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithDhcpDisabledAndStaticIpAddress() : NetworkConfigTest("WhenNetworkConfigIsConstructed") {}
    ~WhenNetworkConfigIsConstructedWithDhcpDisabledAndStaticIpAddress() {}
};

TEST_F(WhenNetworkConfigIsConstructedWithDhcpDisabledAndStaticIpAddress, shouldParseOutReleaventValues) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());
    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("192.168.13.1", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("255.255.255.0", networkConfig.subnetMask().c_str());
    ASSERT_STREQ("192.168.13.0", networkConfig.defaultGateway().c_str());
}

class WhenNetworkConfigIsConstructedWithDhcpEnabledAndStaticIpAddress : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithDhcpEnabledAndStaticIpAddress() : NetworkConfigTest("WhenNetworkConfigIsConstructed") {}
    ~WhenNetworkConfigIsConstructedWithDhcpEnabledAndStaticIpAddress() {}


    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(IP_ADDRESS_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export IPADDR0=\"192.168.13.1.dhcp\"\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsConstructedWithDhcpEnabledAndStaticIpAddress, shouldParseOutReleaventValues) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());
    ASSERT_TRUE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("192.168.13.1", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("255.255.255.0", networkConfig.subnetMask().c_str());
    ASSERT_STREQ("192.168.13.0", networkConfig.defaultGateway().c_str());
}

class WhenNetworkConfigIsConstructedWithGatewayEmpty : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithGatewayEmpty() : NetworkConfigTest("WhenNetworkConfigIsConstructed") {}
    ~WhenNetworkConfigIsConstructedWithGatewayEmpty() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(DEF_GATEWAY_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export GATEWAY0=\"\"\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsConstructedWithGatewayEmpty, shouldParseOutReleaventValues) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());
    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("192.168.13.1", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("255.255.255.0", networkConfig.subnetMask().c_str());
    ASSERT_STREQ("", networkConfig.defaultGateway().c_str());
}

//
// I have seen an instance of a FIG with the IPADDR0 export defined as ".dhcp".
// It's totally non-standard but we better test for it.
//
class WhenNetworkConfigIsConstructedWithOnlyDhcpClient : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithOnlyDhcpClient() : NetworkConfigTest("WhenNetworkConfigIsConstructed") {}
    ~WhenNetworkConfigIsConstructedWithOnlyDhcpClient() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(IP_ADDRESS_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export IPADDR0=\".dhcp\"\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsConstructedWithOnlyDhcpClient, shouldParseOutReleaventValues) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());
    ASSERT_TRUE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("255.255.255.0", networkConfig.subnetMask().c_str());
    ASSERT_STREQ("192.168.13.0", networkConfig.defaultGateway().c_str());
}

class WhenNetworkConfigIsConstructedWithDhcpServerDisabled : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithDhcpServerDisabled() : NetworkConfigTest("WhenNetworkConfigIsConstructed") {}
    ~WhenNetworkConfigIsConstructedWithDhcpServerDisabled() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(DHCPD_ENABLE_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export DHCPD_ENABLE=n\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsConstructedWithDhcpServerDisabled, shouldParseOutReleaventValues) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_FALSE(networkConfig.isDhcpServerEnabled());
    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("192.168.13.1", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("255.255.255.0", networkConfig.subnetMask().c_str());
    ASSERT_STREQ("192.168.13.0", networkConfig.defaultGateway().c_str());
}

class WhenNetworkConfigIsAskedToDisableDhcpServer : public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToDisableDhcpServer() : NetworkConfigTest("WhenNetworkConfigIsAskedToDisableDhcpServer") {}
    ~WhenNetworkConfigIsAskedToDisableDhcpServer() {}
};

TEST_F(WhenNetworkConfigIsAskedToDisableDhcpServer, shouldDisableServer) {
    NetworkConfig networkConfig(_rcConfFile);

    networkConfig.setDhcpServerEnabled(false);
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export DHCPD_ENABLE=n"));
}


class WhenNetworkConfigIsAskedToEnableDhcpServer : public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToEnableDhcpServer() : NetworkConfigTest("WhenNetworkConfigIsAskedToEnableDhcpServer") {}
    ~WhenNetworkConfigIsAskedToEnableDhcpServer() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(DHCPD_ENABLE_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export DHCPD_ENABLE=n\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsAskedToEnableDhcpServer, shouldEnableServer) {
    NetworkConfig networkConfig(_rcConfFile);

    networkConfig.setDhcpServerEnabled(true);
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export DHCPD_ENABLE=y"));
}

class WhenNetworkConfigIsAskedToEnableDhcpClient: public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToEnableDhcpClient() : NetworkConfigTest("WhenNetworkConfigIsAskedToEnableDhcpClient") {}
    ~WhenNetworkConfigIsAskedToEnableDhcpClient() {}
};

TEST_F(WhenNetworkConfigIsAskedToEnableDhcpClient, shouldEnableDhcpClient) {
    NetworkConfig networkConfig(_rcConfFile);

    networkConfig.setDhcpClientEnabled(true);
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export IPADDR0=\"192.168.13.1.dhcp\""));
}

class WhenNetworkConfigIsAskedToDisableDhcpClient: public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToDisableDhcpClient() : NetworkConfigTest("WhenNetworkConfigIsAskedToDisableDhcpClient") {}
    ~WhenNetworkConfigIsAskedToDisableDhcpClient() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(IP_ADDRESS_KEY) != string::npos) {
            rcConfLine.clear();
            rcConfLine = "export IPADDR0=\"192.168.13.1.dhcp\"\n";
        }
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsAskedToDisableDhcpClient, shouldEnableDhcpClient) {
    NetworkConfig networkConfig(_rcConfFile);

    networkConfig.setDhcpClientEnabled(false);
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export IPADDR0=\"192.168.13.1\""));
}


class WhenNetworkConfigIsAskedToUpdateSubnetMask : public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToUpdateSubnetMask() : NetworkConfigTest("WhenNetworkConfigIsAskedToUpdateSubnetMask") {}
    ~WhenNetworkConfigIsAskedToUpdateSubnetMask() {}
};

TEST_F(WhenNetworkConfigIsAskedToUpdateSubnetMask, shouldEnableDhcpClient) {
    NetworkConfig networkConfig(_rcConfFile);

    networkConfig.setSubnetMask("254.244.245.1");
    networkConfig.save();

    string saveFileName(_rcConfFile.absolutePath());
    saveFileName.append(".1");
    ASSERT_TRUE(File::exists(saveFileName.c_str()));
    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export NETMASK0=\"254.244.245.1\""));
}


class WhenNetworkConfigIsAskedToUpdateDefaultGateway : public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToUpdateDefaultGateway() : NetworkConfigTest("WhenNetworkConfigIsAskedToUpdateDefaultGateway") {}
    ~WhenNetworkConfigIsAskedToUpdateDefaultGateway() {}
};

TEST_F(WhenNetworkConfigIsAskedToUpdateDefaultGateway, shouldUpdateDefaultGateway) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.setDefaultGateway("192.168.1.1"));
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export GATEWAY0=\"192.168.1.1\""));
}

TEST_F(WhenNetworkConfigIsAskedToUpdateDefaultGateway, shouldClearDefaultGatewayIfEmptyString) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.setDefaultGateway(""));
    networkConfig.save();

    ASSERT_TRUE(_rcConfFile.exists());
    ASSERT_TRUE(lineExistsInRcConf("export GATEWAY0=\"\""));
}

class WhenNetworkConfigIsAskedToRestoreDefaults : public NetworkConfigTest {
public:
    WhenNetworkConfigIsAskedToRestoreDefaults() :
            NetworkConfigTest("WhenNetworkConfigIsAskedToRestoreDefaults"),
            _networkConfigUnderTest(_rcConfFile) {}
    ~WhenNetworkConfigIsAskedToRestoreDefaults() {}

    void SetUp() {
        _networkConfigUnderTest.setDhcpServerEnabled(false);
        _networkConfigUnderTest.setDhcpClientEnabled(true);
        _networkConfigUnderTest.setDefaultGateway("192.168.1.1");
        _networkConfigUnderTest.setIpAddress("1.2.3.4");
        _networkConfigUnderTest.setSubnetMask("3.4.5.6");
        _networkConfigUnderTest.setDefaultGateway("6.7.8.9");
    }

    NetworkConfig _networkConfigUnderTest;
};

TEST_F(WhenNetworkConfigIsAskedToRestoreDefaults, shouldRestoreValuesToDefaults) {
    _networkConfigUnderTest.restoreDefaults();

    ASSERT_FALSE(_networkConfigUnderTest.isDhcpClientEnabled());
    ASSERT_TRUE(_networkConfigUnderTest.isDhcpServerEnabled());
    ASSERT_STREQ(DEFAULT_IP_ADDRESS, _networkConfigUnderTest.ipAddress().c_str());
    ASSERT_STREQ(DEFAULT_SUBNET_MASK, _networkConfigUnderTest.subnetMask().c_str());
    ASSERT_STREQ(DEFAULT_GATEWAY, _networkConfigUnderTest.defaultGateway().c_str());
}

class WhenNetworkConfigIsConstructedWithDhcpdEnabledKeyMissingFromRcConfFile : public NetworkConfigTest {
public:
    WhenNetworkConfigIsConstructedWithDhcpdEnabledKeyMissingFromRcConfFile() : NetworkConfigTest("WhenNetworkConfigIsAskedToRestoreDefaults") {}
    ~WhenNetworkConfigIsConstructedWithDhcpdEnabledKeyMissingFromRcConfFile() {}

    string& morphRcConfLine(string& rcConfLine) {
        if (rcConfLine.find(DHCPD_ENABLE_KEY) != string::npos)
            rcConfLine.clear();
        return rcConfLine;
    }
};

TEST_F(WhenNetworkConfigIsConstructedWithDhcpdEnabledKeyMissingFromRcConfFile, shouldInsertKeyWhenFileSaved) {
    NetworkConfig networkConfig(_rcConfFile);

    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());

    networkConfig.save();
    ASSERT_TRUE(lineExistsInRcConf("export DHCPD_ENABLE=y"));
}

}
}


