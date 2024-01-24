/** @file IOBoardEEPromCreator.h 
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
#ifndef EEPROMCREATOR_H_
#define EEPROMCREATOR_H_

#include <iostream>

namespace zios {
namespace lpc3250 {

class IOBoardEEPromCreator
{
public:
    IOBoardEEPromCreator();
    virtual ~IOBoardEEPromCreator();

    void createMainBoardEEprom(std::ostream &ostream);
    void createIOBordEEProm(std::ostream& os);
};

} /* namespace orm */
} /* namespace zios */

#endif /* EEPROMCREATOR_H_ */
