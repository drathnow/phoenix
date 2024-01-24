/** @file lpc3250EEPromTest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 21, 2019 by daver:
 * @endhistory
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <File.h>
#include <Lpc3250EEProm.h>
#include <StringUtl.h>

#include "EEPromCreator.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::common;

class Lpc3250EEPromExTest: public testing::Test
{
public:
    Lpc3250EEPromExTest()
    {
    }
    virtual ~Lpc3250EEPromExTest()
    {
    }

    bool keyExistsAndMatchesValue(const string& key, const string& value, const map<string, string>& aMap)
    {
        return (aMap.find(key) != aMap.end() && value.c_str(), aMap.find(key)->second.c_str());
    }
};

class WhenLpc3250EEPromIsAskedToReadRawMainBoardEEPromToVector: public Lpc3250EEPromExTest
{
public:
    WhenLpc3250EEPromIsAskedToReadRawMainBoardEEPromToVector() :
        _eepromFile(File::createTempFile("eepromMain", "test")),
        _expectedStringLength(0)
    {
    }

    ~WhenLpc3250EEPromIsAskedToReadRawMainBoardEEPromToVector()
    {
    }

    void SetUp()
    {
        ofstream os(_eepromFile.absolutePath(), ios_base::out);
        ASSERT_TRUE(os);
        IOBoardEEPromCreator eepromCreator;
        eepromCreator.createMainBoardEEprom(os);
        int pos = os.tellp();
        _expectedStringLength = pos - 1; // subtract the partition marker.
        for (;pos < 1024; pos++)
            os << (char)0xff;
        os.close();
    }

    void TearDown()
    {
        _eepromFile.turf();
    }

    File _eepromFile;
    int _expectedStringLength;
};

class WhenLpc3250EEPromIsAskedTorReadEEPromToVector: public Lpc3250EEPromExTest
{
public:
    WhenLpc3250EEPromIsAskedTorReadEEPromToVector() :
        _eepromFile(File::createTempFile("eepromMain", "test")),
        _expectedStringLength(0)
    {
    }

    ~WhenLpc3250EEPromIsAskedTorReadEEPromToVector()
    {
    }

    void SetUp()
    {
        char eof = LPC3250_EEPROM_EOF;
        char eop = LPC3250_EEPROM_PART_BOUNDRY;
        ofstream os(_eepromFile.absolutePath(), ios_base::out);
        ASSERT_TRUE(os);
        os << "#" << endl;
        os << "# This is a comment" << endl;
        os << "#" << endl;
        os << "this is line1 partion 1" << endl;
        os << "this is line2 partion 1 # Comment at the end" << endl;
        os << "this is line3 partion 1" << endl;
        os.write(&eop, 1);
        os << endl;
        os << "this is line1 partion 2" << endl;
        os << "this is line2 partion 2";
        os.write(&eof, 1);
        os.close();
    }

    void TearDown()
    {
        _eepromFile.turf();
    }

    File _eepromFile;
    int _expectedStringLength;
};

TEST_F(WhenLpc3250EEPromIsAskedTorReadEEPromToVector, shouldReadEEPromAndVector)
{
    char buf[124];
    Lpc3250EEProm eeprom;
    vector<string> theVector;

    eeprom.readMainBoardEEprom(theVector, _eepromFile.absolutePath());
    ASSERT_EQ(2, theVector.size());

    istringstream iss1(theVector[0]);
    ASSERT_TRUE(iss1.getline(buf, sizeof(buf)));
    ASSERT_STREQ("this is line1 partion 1", buf);
    ASSERT_TRUE(iss1.getline(buf, sizeof(buf)));
    ASSERT_STREQ("this is line2 partion 1 ", buf);
    ASSERT_TRUE(iss1.getline(buf, sizeof(buf)));
    ASSERT_STREQ("this is line3 partion 1", buf);
    ASSERT_FALSE(iss1.getline(buf, sizeof(buf)));

    istringstream iss2(theVector[1]);
    ASSERT_TRUE(iss2.getline(buf, sizeof(buf)));
    ASSERT_STREQ("this is line1 partion 2", buf);
    ASSERT_TRUE(iss2.getline(buf, sizeof(buf)));
    ASSERT_STREQ("this is line2 partion 2", buf);
    ASSERT_FALSE(iss1.getline(buf, sizeof(buf)));
}

//
// This test is disabled until we find teh eeprom_ioboard.txt eeprom file.  It seems to have
// gone missing.
//
TEST(WhenEEPromIsToReader, DISABLED_shouldRead)
{
    Lpc3250EEProm eeprom;
    vector<string> theVector;
    map<string, string> theMap;

    eeprom.readMainBoardEEprom(theVector, "../lpc3250/eeprom_ioboard.txt");
    ASSERT_EQ(5, theVector.size());
    for (unsigned i = 0; i < theVector.size(); i++) {
        theMap.clear();
        StringUtl::stringToMap(theVector[i], theMap);
        cout << "-------------------" << endl;
        for (map<string, string>::iterator iter = theMap.begin(); iter != theMap.end(); iter++)
            cout << iter->first << " = '" << iter->second << "'" << endl;

        cout << "-------------------" << endl;

    }
}

}
}
