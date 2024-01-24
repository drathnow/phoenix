/** @file EEprom.h
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 13, 2015 by eganb:
 * IncrDev Apr 17, 2015 by eb:    updated and removed fakeeeprom
 * IncrDev Dec 09, 2015 by eb:    fix up constness
 * @endhistory
 */
#ifndef OLDEEPROM_H_
#define OLDEEPROM_H_

#include <log4cplus/loggingmacros.h>

using namespace std;

namespace zios {
namespace common {

class OldEEprom {
public:
    static const char* MAIN_EEPROM_NAME;
    static const char* MODEM_EEPROM_NAME;
    static const char* IOBOARD_EEPROM_NAME;
    static const char* IOBOARD_EEPROM2_NAME;

    OldEEprom(const std::string& deviceFile);
    ~OldEEprom();

    /**
     * read and EEPROM partition from the EEpromchip (one of:MAIN_EEPROM, MODEM_EEPROM, IOBOARD_EEPROM, orIOBOARD_EEPROM2(future))
     *
     * @parm returnVector vector to receive continuous string read from partitions
     *
     * @param maxPartition maximum partition to read (eg 1 or 2)
     *
     * @see IEEProm::EEPROM_CHIP
     */
    int readOldEEpromFileToVector(std::vector<std::string>& returnVector, int maxPartition = 0);

private:
    static const log4cplus::Logger eeLogger;
    std::string _device;

        // no copy/assignment
    OldEEprom(const OldEEprom &);
    OldEEprom& operator=(const OldEEprom &);
};


class EEpromFactory {
public:

    enum EEPROM_CHIP {
            MAIN_EEPROM = 1,
            MODEM_EEPROM = 2,
            IOBOARD_EEPROM = 3,
            IOBOARD_EEPROM2 = 4,
        };


    static OldEEprom* createOldEEprom(EEPROM_CHIP chip);

};

}}

#endif /* OLDEEPROM_H_ */
