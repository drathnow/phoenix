/** @file Eeprom.h 
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
#ifndef EEPROM_H_
#define EEPROM_H_

#include <vector>
#include <string>

namespace zios {
namespace hal {

using namespace std;

class EEProm
{
public:
    virtual ~EEProm() {}

    virtual std::vector<string>* readMainBoardEEprom(vector<string>& returnMap, const char* path = NULL) = 0;
    virtual std::vector<string>* readModemEEprom(vector<string>& returnMap, const char* path = NULL) = 0;
    virtual std::vector<string>* readIoBoardEEprom(vector<string>& returnMap, const char* path = NULL) = 0;

};

}
}




#endif /* EEPROM_H_ */
