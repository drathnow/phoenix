/** @file Lpc3250HardwareManagerTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 24, 2019 by daver:
 * @endhistory
 */

#include <gtest/gtest.h>
#include <unistd.h>
#include <lpc3250.h>
#include <Lpc3250HardwareManager.h>
#include <File.h>
#include <string>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::common;

static const char* MAC_ADDRESS = "98:02:d8:40:07:09";

class Lpc3250HardwareManagerTest: public testing::Test
{
public:
    Lpc3250HardwareManagerTest() : _testableLpc3250HardwareManager(true)
    {
    }
    ~Lpc3250HardwareManagerTest()
    {
    }

    Lpc3250HardwareManager _testableLpc3250HardwareManager;
};

class WhenLpc3250HardwareManagerIsAskedToReturnMacAddress: public Lpc3250HardwareManagerTest
{
public:
    ~WhenLpc3250HardwareManagerIsAskedToReturnMacAddress()
    {
    }

    virtual void SetUp()
    {
        File macFile(LPC3250_MAC_ADDRESS_PATH);
        macFile.turf();

        FILE* file = ::fopen(LPC3250_MAC_ADDRESS_PATH, "w");
        ::fprintf(file, "%s", MAC_ADDRESS);
        ::fclose(file);
    }

    virtual void TearDown()
    {

    }
};

TEST_F(WhenLpc3250HardwareManagerIsAskedToReturnMacAddress, shouldReturnMacAddress)
{
    string macAddress;
    ASSERT_TRUE(NULL != _testableLpc3250HardwareManager.macAddress(macAddress));
    ASSERT_STREQ(MAC_ADDRESS, _testableLpc3250HardwareManager.macAddress(macAddress)->c_str());
}

}
}
