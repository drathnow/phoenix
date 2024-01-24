/** @file HexDumper.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar 14, 2016 by daver:
 * @endhistory
 */

#include "HexDumper.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace zios {
namespace common {

using namespace std;

static const std::string stringForBytes(const char* buffer, int bufferLength) {
    std::stringstream stream;
    for (int i = 0; i < bufferLength; ++i) {
        if (buffer[i] >= 0x20 && buffer[i] <= 0x7e)
            stream << buffer[i];
        else
            stream << '.';
    }
    return stream.str();
}

const std::string HexDumper::bytesAsHexString(const char* buffer, int bufferLength) {
    std::stringstream stream;
    const unsigned char* uBuf = (const unsigned char*)buffer;
    for (int i = 0; i < bufferLength; ++i) {
        stream << std::hex << std::setfill('0') << std::setw(2) << (int) uBuf[i] << " ";
    }
    return stream.str();
}

HexDumper::HexDumper(int dumpWidth) : _dumpWidth(dumpWidth) {
}

HexDumper::~HexDumper() {
}

void HexDumper::dumpToStream(const char* buf, int length, std::ostream& stream) const {
    int whatsLeft = length;
    int pos = 0;

    while (whatsLeft > 0) {
        int bytesToDump = _dumpWidth;
        if (whatsLeft <= _dumpWidth)
            bytesToDump = whatsLeft;

        string hexBytes = bytesAsHexString(&buf[pos], bytesToDump);
        if (_dumpWidth-whatsLeft > 0) {
            for (int i = 0; i < (_dumpWidth-whatsLeft); i++)
                hexBytes.append("   ");
        }
        string strBytes = stringForBytes(&buf[pos], bytesToDump);
        if (_dumpWidth-whatsLeft > 0) {
            for (int i = 0; i < (_dumpWidth-whatsLeft); i++)
                strBytes.append(" ");
        }
        stream << "| "<< hexBytes << "| " << strBytes << " |" << endl;
        whatsLeft -= _dumpWidth;
        pos += _dumpWidth;
    }

}

void HexDumper::dump(const char* buf, int length) const {
    dumpToStream(buf, length, cout);
}


}
}
