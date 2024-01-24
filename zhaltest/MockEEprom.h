/** @file MockEeprom.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 25, 2019 by daver:
 * @endhistory
 */
#ifndef MOCKEEPROM_H_
#define MOCKEEPROM_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <EEProm.h>

namespace zios {
namespace hal {

using namespace std;

class MockEEprom : public EEProm
{
public:
    ~MockEEprom() {}

    MOCK_METHOD2(readMainBoardEEprom, map<string, string>*(map<string, string>& returnMap, const char* path));
    MOCK_METHOD2(readModemEEprom, map<string, string>*(map<string, string>& returnMap, const char* path));
    MOCK_METHOD2(readIoBoardEEprom, map<string, string>*(map<string, string>& returnMap, const char* path));
};
}
}



#endif /* MOCKEEPROM_H_ */
