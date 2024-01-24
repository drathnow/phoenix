/** @file OldEEprom.cpp
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
 * BugFix_ Nov 30, 2015 by eb: fixed file name/address for 2nd i2c interface eeproms (IOBoard)
 * BugFix_ Dec 09, 2015 by eb: fixed signedness on readCount (must recognize -1)
 * BugFix_ Dec 10, 2015 by eb: fix problem on reading large block with partition marker
 * @endhistory
 */


#include <sstream>          // string concatenation
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>  // memmove etc.
#include "OldEEprom.h"

using namespace log4cplus;
using namespace std;

namespace zios {
namespace common {

const Logger OldEEprom::eeLogger = Logger::getInstance("zios.hardware.eeprom");
const char* OldEEprom::MAIN_EEPROM_NAME = "/sys/class/i2c-adapter/i2c-0/0-0050/eeprom";
const char* OldEEprom::MODEM_EEPROM_NAME = "/sys/class/i2c-adapter/i2c-0/0-0051/eeprom";
const char* OldEEprom::IOBOARD_EEPROM_NAME = "/sys/class/i2c-adapter/i2c-1/1-0052/eeprom";
const char* OldEEprom::IOBOARD_EEPROM2_NAME = "/sys/class/i2c-adapter/i2c-1/1-0054/eeprom";

OldEEprom::OldEEprom(const string& device) : _device(device){
}

OldEEprom::~OldEEprom(){
}

// helper function to append part of char buffer to stream...
static void updateStringBuffer(std::ostringstream& theStream, char* buffer, size_t len){
    string tmpString(buffer, len);
    theStream << tmpString;
}

int OldEEprom::readOldEEpromFileToVector(std::vector<std::string>& returnVector, int maxPartition) {
    ostringstream sstream;
    unsigned char readbuffer[1025];
    int readCount;

    int currentPartition = 1;   // initialize currentPartition to be #1 :)

    // bound partition count and translate "special" maxPartition of 0
    if ((maxPartition == 0) || maxPartition > 99)
        maxPartition = 99;

    int efd = ::open(_device.c_str(), O_RDONLY);

    // if file exists and opened....
    if (efd != -1) {
        bool readOldEEprom = true;                     // prime "need to read eeprom to buffer" flag
        while (readOldEEprom) {
            int thisReadProcess = 0;                         //  to catch special case where we read 1024 without /0xFF
            readCount = ::read(efd, (char*) readbuffer, 1024);

            // short cct if nothing there....
            if (readCount < 1) {
                readOldEEprom = false;
                // check if something in sstream and put into out partition vector...
                if (sstream.str().length() > 0)
                    returnVector.push_back(sstream.str());
            }

            // process the buffer till it is empty.....
            while (readCount > 0) {
                // special catch for reaching end of buffer without any partition / eof....
                if (thisReadProcess >= readCount) {
                    // push all buffer into stream,
                    updateStringBuffer(sstream, (char*) readbuffer, thisReadProcess);
                    readCount = 0;  // and go get next gulp
                }
                // parse buffer segment.
                for (int i = 0; i < readCount; i++) {
                    // if hit "EOF" then we are done....
                    if (0xFF == readbuffer[i]) {
                        readCount = 0;      // to break inner while
                        readOldEEprom = false; // stop reading any more eeprom :)
                        if (i > 1)
                            updateStringBuffer(sstream, (char*) readbuffer, i);
                        returnVector.push_back(sstream.str());
                        //  clean up stream....
                        sstream.clear();
                        sstream.str("");
                        break;  // from for....
                    } else if (0x04 == readbuffer[i]) {
                        // hit partition marker.... need to process partition...
                        currentPartition++;
                        // at least 1 data character in addition to the partition marker
                        if (i > 1)
                            updateStringBuffer(sstream, (char*) readbuffer, i);
                        returnVector.push_back(sstream.str());
                        //  clean up stream so it can be used again....
                        sstream.clear();
                        sstream.str("");
                        thisReadProcess++;   // to take into account the 0x04 :)
                        // check if done.....
                        if (currentPartition > maxPartition) {
                            readOldEEprom = false;
                            readCount = 0;      // to break inner while....
                            break; // from for
                        } else {
                            // move rest of buffer down for next pass.
                            if ((readCount - i) > 0)
                                ::memmove(readbuffer, &readbuffer[i + 1], readCount - (i + 1));
                            readCount -= (i + 1);
                            thisReadProcess = 0;   // reset for this read process..

                            break; // from for.
                        }
                    }  // if 0x04 is char
                    thisReadProcess++;
                    // .. normal character -> just move up one char.... (default action i++)
                } //  for i = 0 through read count....
            } // while readCount > 0
        } // while eepromRead

        // check "last" vector for data and adjust vector if string is empty, then delete vector if it is empty.
        if (returnVector.size() > 0) {
            string tmpStr = returnVector.back();
            if (tmpStr.empty()) {
                returnVector.pop_back();
            } // if string empty....
        }
        ::close(efd);
    } else {
        LOG4CPLUS_ERROR(eeLogger, _device << " file NOT opened for reading");
    } // end else if file opened...

    return returnVector.size();
}

OldEEprom* EEpromFactory::createOldEEprom(EEPROM_CHIP chip){
    const char* eepromFile;

#ifdef _ONARM
    switch (chip) {
        case EEpromFactory::MAIN_EEPROM:
            eepromFile = OldEEprom::MAIN_EEPROM_NAME;
            break;
        case EEpromFactory::MODEM_EEPROM:
            eepromFile = OldEEprom::MODEM_EEPROM_NAME;
            break;
        case EEpromFactory::IOBOARD_EEPROM:
            eepromFile = OldEEprom::IOBOARD_EEPROM_NAME;
            break;
        case EEpromFactory::IOBOARD_EEPROM2:
            eepromFile = OldEEprom::IOBOARD_EEPROM2_NAME;
            break;
        default:
            eepromFile = NULL;
    } // switch
#else
    switch (chip) {
        case EEpromFactory::MAIN_EEPROM:
            eepromFile = "./eepromMain";
            break;
        case EEpromFactory::MODEM_EEPROM:
            eepromFile = "./eepromModem";
            break;
        case EEpromFactory::IOBOARD_EEPROM:
            eepromFile = "./eepromIOBoard";
            break;
        case EEpromFactory::IOBOARD_EEPROM2:
            eepromFile = "./eepromIOBoard2";
            break;
        default:
            eepromFile = NULL;
    } // switch
#endif
    if (eepromFile != NULL)
        return new OldEEprom(eepromFile);
    else
        return NULL;
}

}
} // namespace
