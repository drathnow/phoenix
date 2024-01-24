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
 * Created Mar. 18, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250EEPROM_H_
#define LPC3250EEPROM_H_
#include <zhal.h>
#include <string>
#include <vector>

#define LPC3250_EEPROM_EOF              0xFF
#define LPC3250_EEPROM_PART_BOUNDRY     0x04


namespace zios {
namespace lpc3250 {

using namespace  std;

class Lpc3250EEProm : public zios::hal::EEProm
{
public:
    Lpc3250EEProm();
    virtual ~Lpc3250EEProm();

    static const char* MAIN_EEPROM_NAME;
    static const char* MODEM_EEPROM_NAME;
    static const char* IOBOARD_EEPROM_NAME;

    virtual std::vector<string>* readMainBoardEEprom(vector<string>& partitions, const char* path = MAIN_EEPROM_NAME);
    virtual std::vector<string>* readModemEEprom(vector<string>& partitions, const char* path = MODEM_EEPROM_NAME);
    virtual std::vector<string>* readIoBoardEEprom(vector<string>& partitions, const char* path = IOBOARD_EEPROM_NAME);
};

}
}

#endif /* LPC3250EEPROM_H_ */
