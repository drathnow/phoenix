/** @file HexDumper.h
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
#ifndef HEXDUMPER_H_
#define HEXDUMPER_H_

#include <string>
#include <ostream>

namespace zios {
namespace common {

/**
 * HexDumper is a utility class that will format an array of bytes into both
 * hex and ascii.  The display is split so that the hex output is displayed to
 * the left and the ascii output to the right.  The ascii display will show
 * all printable characters (i.e. those between 0x20 and 0x7e) and will display
 * a doc (.) for unprintable characters.  Below is a typical example of a output
 * from an HTTP web request with dumpWidth = 20:
 *
 *  Byte 0                                               Byte 19 Byte 0           Byte 19
 *   |                                                        |    |                  |
 *   v                                                        v    v                  v
 * | 2d 2d 2d 2d 2d 2d 57 65 62 4b 69 74 46 6f 72 6d 42 6f 75 6e | ------WebKitFormBoun |
 * | 64 61 72 79 0d 0a 43 6f 6e 74 65 6e 74 2d 44 69 73 70 6f 73 | dary..Content-Dispos |
 * | 69 74 69 6f 6e 3a 20 66 6f 72 6d 2d 64 61 74 61 3b 20 6e 61 | ition: form-data; na |
 * | 6d 65 3d 22 66 69 6c 65 31 22 3b 20 66 69 6c 65 6e 61 6d 65 | me="file1"; filename |
 * | 3d 22 66 6f 6f 2e 74 78 74 22 0d 0a 43 6f 6e 74 65 6e 74 2d | ="foo.txt"..Content- |
 * | 54 79 70 65 3a 20 74 65 78 74 2f 70 6c 61 69 6e 0d 0a 0d 0a | Type: text/plain.... |
 * | 54 68 69 73 20 69 73 20 61 20 62 75 6e 63 68 20 6f 66 20 63 | This is a bunch of c |
 * | 6f 6e 74 65 6e 74 0d 0a 2d 2d 2d 2d 57 65 62 4b 69 74 46 6f | ontent..----WebKitFo |
 * | 72 6d 42 6f 75 6e 64 61 72 79 2d 2d 0d 0a                   | rmBoundary--..       |
 *
 * | <-------------------- HEX Portion ------------------------> | <- ASCII portion --> |
 *
 * The "."s in the ASCII portion represent hex values 0x0d and 0x0a.
 *
 */
class HexDumper {
public:
    /**
     * Constructs a HexDumper object.
     *
     * @param dumpWidth - Sets the number of bytes that will be formatted
     * per line of display.
     */
    HexDumper(int dumpWidth = 40);
    virtual ~HexDumper();

    void dump(const char* buf, int length) const;
    void dumpToStream(const char* buf, int length, std::ostream& stream) const;

    static const std::string bytesAsHexString(const char* buffer, int bufferLength);

private:
    int _dumpWidth;
};

}
}

#endif /* HEXDUMPER_H_ */
