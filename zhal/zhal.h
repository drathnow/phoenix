/** @file hal.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 1, 2019 by daver:
 * @endhistory
 */
#ifndef HAL_H_
#define HAL_H_

#include <string>
#include <stdint.h>

#define PORT_TYPE_RS232         1
#define PORT_TYPE_RS485         2
#define PORT_TYPE_RS232FC       3

#include "EEProm.h"
#include "IHardwareIO.h"
#include "IAnalogIO.h"
#include "IDigitalIO.h"
#include "IPowerModule.h"
#include "ITransceiver.h"
#include "RTD.h"
#include "LED.h"
#include "Button.h"
#include "WakeButtonListener.h"
#include "IHardwareManager.h"
#include "NetworkConfiguration.h"

#endif /* hal_H_ */
