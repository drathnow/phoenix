/** @file EEPromUtil.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 27, 2019 by daver:
 * @endhistory
 */
#ifndef EEPROMUTIL_H_
#define EEPROMUTIL_H_

namespace zios {
namespace hal {

class EEPromUtil
{
public:
    EEPromUtil();
    virtual ~EEPromUtil();

    static void createMainBoardEEProm(const char* path);
};

} /* namespace hal */
} /* namespace zios */

#endif /* EEPROMUTIL_H_ */
