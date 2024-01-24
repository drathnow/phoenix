/** @file Lpc3250NetworkConfigurationTest.cpp 
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

#include <File.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <Lpc3250NetworkConfiguration.h>
#include <fstream>
#include <string>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::common;

static const string IP_ADDRESS("10.1.2.41");
static const string GATEWAY_ADDRESS("10.1.2.4");
static const string SUBNET_MASK("255.255.255.0");

class Lpc3250NetworkConfigurationTest: public testing::Test
{
public:
    Lpc3250NetworkConfigurationTest() :
            _interfacesFile("interfaces.txt"),
                    _networkConfFile("network.confg")
    {
    }
    ~Lpc3250NetworkConfigurationTest()
    {
    }

    virtual void TearDown()
    {
        _networkConfFile.turf();
        _interfacesFile.turf();
    }

    void createDhcpInterfacesFile()
    {
        _interfacesFile.turf();
        ofstream theStream(_interfacesFile.absolutePath());
        ASSERT_TRUE(theStream.is_open());
        theStream << "auto lo" << endl;
        theStream << "iface lo inet loopback" << endl << endl;
        theStream << "auto eth0" << endl;
        theStream << "iface eth0 inet dhcp" << endl;
        theStream.close();
    }

    void createNetworkConfFile(bool dhcpdEnabled, bool usb0Enabled, const string &usb0IpAddress)
    {
        _networkConfFile.turf();
        ofstream theStream(_networkConfFile.absolutePath());
        ASSERT_TRUE(theStream.is_open());
        theStream << LPC3250_DHCPD_ENABLED_VAR << "=" << (dhcpdEnabled ? "\"y\"" : "\"n\"") << endl;
        theStream << LPC3250_USB0_ENABLED_VAR << "=" << (usb0Enabled ? "\"y\"" : "\"n\"") << endl;
        theStream << LPC3250_USB0_ADDRESS_VAR << "=\"" << usb0IpAddress << "\"" << endl;
        theStream.close();
    }

    void createStaticInterfacesFile()
    {
        _interfacesFile.turf();
        ofstream theStream(_interfacesFile.absolutePath());
        ASSERT_TRUE(theStream.is_open());
        theStream << "auto lo" << endl;
        theStream << "iface lo inet loopback" << endl << endl;
        theStream << "auto eth0" << endl;
        theStream << "iface eth0 inet static" << endl;
        theStream << "  address " << IP_ADDRESS << endl;
        theStream << "  netmask " << SUBNET_MASK << endl;
        theStream << "  gateway " << GATEWAY_ADDRESS << endl;
        theStream.close();
    }

    File _interfacesFile;
    File _networkConfFile;
};

class WhenLpc3250NetworkConfigurationIsAskedToLoadAStaticConfiguration: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToLoadAStaticConfiguration()
    {
    }

    ~WhenLpc3250NetworkConfigurationIsAskedToLoadAStaticConfiguration()
    {
    }

    void SetUp()
    {
        createStaticInterfacesFile();
        createNetworkConfFile(true, true, LPC3250_USB0_DEF_ADDRESS);
    }

};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToLoadAStaticConfiguration, shouldParseDetails)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    networkConfig.load();

    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ(IP_ADDRESS.c_str(), networkConfig.ipAddress().c_str());
    ASSERT_STREQ(GATEWAY_ADDRESS.c_str(), networkConfig.defaultGateway().c_str());
    ASSERT_STREQ(SUBNET_MASK.c_str(), networkConfig.subnetMask().c_str());
    ASSERT_TRUE(networkConfig.isUsbEnabled());
    ASSERT_TRUE(networkConfig.isDhcpServerEnabled());
}

class WhenLpc3250NetworkConfigurationIsAskedToLoadADhcpConfiguration: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToLoadADhcpConfiguration()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedToLoadADhcpConfiguration()
    {
    }

    void SetUp()
    {
        createDhcpInterfacesFile();
        createNetworkConfFile(false, true, LPC3250_USB0_DEF_ADDRESS);
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToLoadADhcpConfiguration, shouldParseDetails)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());

    ASSERT_TRUE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("", networkConfig.defaultGateway().c_str());
    ASSERT_STREQ("", networkConfig.subnetMask().c_str());
    ASSERT_TRUE(networkConfig.isUsbEnabled());
    ASSERT_FALSE(networkConfig.isDhcpServerEnabled());
}

class WhenLpc3250NetworkConfigurationIsAskedToRestoreDefaults: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToRestoreDefaults()
    {
    }

    ~WhenLpc3250NetworkConfigurationIsAskedToRestoreDefaults()
    {
    }

    void SetUp()
    {
        createStaticInterfacesFile();
        createNetworkConfFile(false, false, "1.2.3.4/45");
    }

};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToRestoreDefaults, shouldRestoreDhcpSettings)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());

    networkConfig.restoreDefaults();

    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ(LPC3250_ETH0_DEF_ADDRESS, networkConfig.ipAddress().c_str());
    ASSERT_STREQ("", networkConfig.defaultGateway().c_str());
    ASSERT_STREQ(LPC2350_ETH0_DEF_SUBNET, networkConfig.subnetMask().c_str());
    ASSERT_TRUE(networkConfig.isUsbEnabled());
}

class WhenLpc3250NetworkConfigurationIsAskedToEnableDhcpClient: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToEnableDhcpClient()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedToEnableDhcpClient()
    {
    }

    void SetUp()
    {
        createStaticInterfacesFile();
        createNetworkConfFile(true, true, LPC3250_USB0_DEF_ADDRESS);
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToEnableDhcpClient, shouldResetEverythingBackToDefault)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());

    networkConfig.setDhcpClientEnabled(false);

    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ(IP_ADDRESS.c_str(), networkConfig.ipAddress().c_str());
    ASSERT_STREQ(GATEWAY_ADDRESS.c_str(), networkConfig.defaultGateway().c_str());
    ASSERT_STREQ(SUBNET_MASK.c_str(), networkConfig.subnetMask().c_str());

    networkConfig.setDhcpClientEnabled(true);

    ASSERT_TRUE(networkConfig.isDhcpClientEnabled());
    ASSERT_FALSE(networkConfig.isDhcpServerEnabled());
    ASSERT_STREQ("", networkConfig.ipAddress().c_str());
    ASSERT_STREQ("", networkConfig.defaultGateway().c_str());
    ASSERT_STREQ("", networkConfig.subnetMask().c_str());
}

class WhenLpc3250NetworkConfigurationIsAskedToDisableDhcpClient: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToDisableDhcpClient()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedToDisableDhcpClient()
    {
    }

    void SetUp()
    {
        createDhcpInterfacesFile();
        createNetworkConfFile(true, true, LPC3250_USB0_DEF_ADDRESS);
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToDisableDhcpClient, shouldSetThingsToValuePassed)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());

    networkConfig.setDhcpClientEnabled(false);
    networkConfig.setIpAddress(IP_ADDRESS);
    networkConfig.setDefaultGateway(GATEWAY_ADDRESS);
    networkConfig.setSubnetMask(SUBNET_MASK);

    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ(IP_ADDRESS.c_str(), networkConfig.ipAddress().c_str());
    ASSERT_STREQ(GATEWAY_ADDRESS.c_str(), networkConfig.defaultGateway().c_str());
    ASSERT_STREQ(SUBNET_MASK.c_str(), networkConfig.subnetMask().c_str());
}

class WhenLpc3250NetworkConfigurationIsAskedSaveItsConfiguration: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedSaveItsConfiguration()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedSaveItsConfiguration()
    {
    }

    void SetUp()
    {
        createDhcpInterfacesFile();
        createNetworkConfFile(false, true, LPC3250_USB0_DEF_ADDRESS);
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedSaveItsConfiguration, shouldSaveToBothFiles)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());
    _interfacesFile.turf();
    _networkConfFile.turf();

    ASSERT_EQ(0, networkConfig.save());
    ASSERT_TRUE(_interfacesFile.exists());
    ASSERT_TRUE(_networkConfFile.exists());

    Lpc3250NetworkConfiguration newNetworkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, newNetworkConfig.load());

    ASSERT_TRUE(newNetworkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("", newNetworkConfig.ipAddress().c_str());
    ASSERT_STREQ("", newNetworkConfig.defaultGateway().c_str());
    ASSERT_STREQ("", newNetworkConfig.subnetMask().c_str());
    ASSERT_TRUE(newNetworkConfig.isUsbEnabled());
    ASSERT_FALSE(newNetworkConfig.isDhcpServerEnabled());
}

class WhenLpc3250NetworkConfigurationIsAskedLoadAndSaveAConfigurationWithExtraLines: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedLoadAndSaveAConfigurationWithExtraLines()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedLoadAndSaveAConfigurationWithExtraLines()
    {
    }

    void SetUp()
    {
        createDhcpInterfacesFile();
        ofstream ofs;
        ofs.open(_interfacesFile.absolutePath(), std::ofstream::app);
        ofs << LPC3250_KEEPIT_BEGIN << endl;
        ofs << "This is an extra line to hold onto" << endl;
        ofs << "This is yet another line to hold onto" << endl;
        ofs << LPC3250_KEEPIT_END << endl;
        ofs.close();

        createNetworkConfFile(false, true, LPC3250_USB0_DEF_ADDRESS);
        ofs.open(_networkConfFile.absolutePath(), std::ofstream::app);
        ofs << LPC3250_KEEPIT_BEGIN << endl;
        ofs << "This is an extra line to hold onto" << endl;
        ofs << "This is yet another line to hold onto" << endl;
        ofs << LPC3250_KEEPIT_END << endl;
        ofs.close();
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedLoadAndSaveAConfigurationWithExtraLines, shouldSaveAllData)
{
    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, networkConfig.load());
    _interfacesFile.turf();
    _networkConfFile.turf();

    ASSERT_EQ(0, networkConfig.save());
    ASSERT_TRUE(_interfacesFile.exists());
    ASSERT_TRUE(_networkConfFile.exists());

    Lpc3250NetworkConfiguration newNetworkConfig(_interfacesFile, _networkConfFile);
    ASSERT_EQ(0, newNetworkConfig.load());

    ASSERT_TRUE(newNetworkConfig.isDhcpClientEnabled());
    ASSERT_STREQ("", newNetworkConfig.ipAddress().c_str());
    ASSERT_STREQ("", newNetworkConfig.defaultGateway().c_str());
    ASSERT_STREQ("", newNetworkConfig.subnetMask().c_str());
    ASSERT_TRUE(newNetworkConfig.isUsbEnabled());
    ASSERT_FALSE(newNetworkConfig.isDhcpServerEnabled());
}

class WhenLpc3250NetworkConfigurationIsAskedToLoadAndConfigurationFilesDontExist: public Lpc3250NetworkConfigurationTest
{
public:
    WhenLpc3250NetworkConfigurationIsAskedToLoadAndConfigurationFilesDontExist()
    {
    }
    ~WhenLpc3250NetworkConfigurationIsAskedToLoadAndConfigurationFilesDontExist()
    {
    }
};

TEST_F(WhenLpc3250NetworkConfigurationIsAskedToLoadAndConfigurationFilesDontExist, shouldLoadDefaultValuesAndThenCreateFilesWhenSaved)
{
    ASSERT_FALSE(_interfacesFile.exists());
    ASSERT_FALSE(_networkConfFile.exists());

    Lpc3250NetworkConfiguration networkConfig(_interfacesFile, _networkConfFile);

    ASSERT_EQ(0, networkConfig.load());
    ASSERT_FALSE(networkConfig.isDhcpClientEnabled());
    ASSERT_STREQ(LPC3250_ETH0_DEF_ADDRESS, networkConfig.ipAddress().c_str());
    ASSERT_STREQ("", networkConfig.defaultGateway().c_str());
    ASSERT_STREQ(LPC2350_ETH0_DEF_SUBNET, networkConfig.subnetMask().c_str());
    ASSERT_TRUE(networkConfig.isUsbEnabled());

    ASSERT_EQ(0, networkConfig.save());
    ASSERT_TRUE(_interfacesFile.exists());
    ASSERT_TRUE(_networkConfFile.exists());

}

}
}
