/** @file Lpc3250Initializer.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 4, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250INITIALIZER_H_
#define LPC3250INITIALIZER_H_

#include <StartupInitializer.h>

namespace zios {
namespace lpc3250 {

class Lpc3250Initializer : public zios::common::StartupInitializer
{
public:
    Lpc3250Initializer();
    ~Lpc3250Initializer();

    void registerWithAppManager(zios::common::AppManager& appManager);

    void initialize();
    void start();
    void shutdown();
    const char* name();
};

} /* namespace hal */
} /* namespace zios */

#endif /* LPC3250INITIALIZER_H_ */
