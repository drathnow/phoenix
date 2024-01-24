/** @file Eeprom.cpp 
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
#include "Lpc3250EEProm.h"

#include <log4cIncludes.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <string>
#include <cassert>
#include <StringUtl.h>
#include <File.h>
#include <Utl.h>
#include <iostream>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

const char *Lpc3250EEProm::MAIN_EEPROM_NAME = "/sys/bus/nvmem/devices/1-00500/nvmem";
const char *Lpc3250EEProm::MODEM_EEPROM_NAME = "/sys/bus/nvmem/devices/1-00510/nvmem";
const char *Lpc3250EEProm::IOBOARD_EEPROM_NAME = "/sys/bus/nvmem/devices/2-00520/nvmem";

#define STATE_EEPROM    1
#define STATE_COMMENT   2

static int parse_eeprom_into_vector(const char *eepromPath, std::vector<std::string> &returnVector, int maxPartition)
{
    ostringstream sstream;
    unsigned char readbuffer[1025];
    int readCount;
    int state = STATE_EEPROM;
    int charCount = 0;
    int currentPartition = 1;   // initialize currentPartition to be #1 :)

    // bound partition count and translate "special" maxPartition of 0
    if ((maxPartition == 0) || maxPartition > 99)
        maxPartition = 99;
    int efd = ::open(eepromPath, O_RDONLY);

    // if file exists and opened....
    if (efd == -1)
        return -1;

    bool readEEprom = true;                     // prime "need to read eeprom to buffer" flag
    while (readEEprom)
    {
        readCount = ::read(efd, (char*) readbuffer, 1024);

        //
        // If nothing to read, we are done.
        //
        if (readCount < 1)
                {
            readEEprom = false;
            if (sstream.str().length() > 0)
                returnVector.push_back(sstream.str());
        }

        for (int i = 0; i < readCount; i++) {
            switch (state)
            {
                case STATE_EEPROM: {
                    switch (readbuffer[i])
                    {
                        case LPC3250_EEPROM_EOF:
                            if (sstream.str().length() > 0)
                                returnVector.push_back(sstream.str());
                            readEEprom = false;
                            i = readCount;
                            charCount = 0;
                            break;

                        case LPC3250_EEPROM_PART_BOUNDRY:
                            if (sstream.str().length() > 0) {
                                returnVector.push_back(sstream.str());
                                sstream.clear();
                                sstream.str("");
                                if (currentPartition > maxPartition)
                                        {
                                    readEEprom = false;
                                    readCount = 0;
                                }
                                charCount = 0;
                            }
                            break;

                        case '#':
                            if (charCount > 0) {
                                sstream << endl;
                                charCount = 0;
                            }
                            state = STATE_COMMENT;
                            break;

                        case 0x0a:
                            if (charCount > 0) {
                                charCount = 0;
                                sstream << readbuffer[i];
                            }
                            break;

                        default:
                            charCount++;
                            if (::isprint(readbuffer[i]))
                                sstream << readbuffer[i];
                    }
                }
                    break;

                case STATE_COMMENT:
                    switch (readbuffer[i])
                    {
                        case 0x0a:
                            state = STATE_EEPROM;
                            break;

                        default:
                            break;
                    }
                    break;
            }
        }
    }
    ::close(efd);
    return returnVector.size();
}

Lpc3250EEProm::Lpc3250EEProm()
{
}

Lpc3250EEProm::~Lpc3250EEProm()
{
}

std::vector<string>* Lpc3250EEProm::readMainBoardEEprom(vector<string> &partitions, const char *path)
{
    if (NULL == path)
        path = MAIN_EEPROM_NAME;

    partitions.clear();
    parse_eeprom_into_vector(path, partitions, 2);
    return &partitions;
}

std::vector<string>* Lpc3250EEProm::readModemEEprom(vector<string> &partitions, const char *path)
{
    if (NULL == path)
        path = MODEM_EEPROM_NAME;

    partitions.clear();
    parse_eeprom_into_vector(path, partitions, 1);
    return &partitions;
}

std::vector<string>* Lpc3250EEProm::readIoBoardEEprom(vector<string> &partitions, const char *path)
{
    if (NULL == path)
        path = IOBOARD_EEPROM_NAME;
    if (File::exists(path))
        parse_eeprom_into_vector(path, partitions, 2);
    return &partitions;
}

}
}
