/** @file SystemTagName.cpp
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb 23, 2016 by eganb:
 * IncrDev Mar 15, 2016 by eb:    add in some AI support system points.
 * IncrDev Apr 04, 2016 by eb:    change name of DC.enable on DI's
 * IncrDev Apr 11, 2016 by eb:    change tags for IOBoard system points
 * @endhistory
 */


#include "SystemTagName.h"

namespace zios {
namespace common {

const char* SystemTagName::LOOP_POWER_NAME  = "POW.LOOP";
const char* SystemTagName::V5_POWER_NAME = "POW.5V";

/*
 * General Main points
 */
const char* SystemTagName::SysSerialnumber              = "sys.serialnumber";
const char* SystemTagName::SysFirmwareRev               = "sys.fw.revision";
const char* SystemTagName::SysHwMainID                  = "sys.hw.main.id";
const char* SystemTagName::SysHwMainRev                 = "sys.hw.main.revision";
const char* SystemTagName::SysHwMainVolt                = "sys.hw.main.voltage";
const char* SystemTagName::SysHwMainTemp                = "sys.hw.main.temperature";


/* General IO Board system points */
const char* SystemTagName::SysHwIoID                    = "sys.hw.io.id";
const char* SystemTagName::SysHwIoREV                   = "sys.hw.io.revision";
const char* SystemTagName::SysHwIoType                  = "sys.hw.io.type";

/* HACK */
const char* SystemTagName::SysMbsOutputCoilsPollLimit   = "sys.mbs.outputCoilsPollLimit";
const char* SystemTagName::SysMbsInputCoilsPollLimit    = "sys.mbs.inputCoilsPollLimit";
const char* SystemTagName::SysMbsInputRegPollLimit      = "sys.mbs.inputRegistersPollLimit";
const char* SystemTagName::SysMbsHoldingRegPollLimit    = "sys.mbs.holdingRegistersPollLimit";
const char* SystemTagName::SysRocSrcAddress             = "sys.roc.srcAddress";
const char* SystemTagName::SysRocSrcGroup               = "sys.roc.srcGroup";

const char* SystemTagName::SysNetSrvAddress             = "sys.net.netSrvAddress";

/* AI "configuration" Points */
const char* SystemTagName::SysHwIoA1EngOffset           = "sys.io.eng.AI.1.offset";
const char* SystemTagName::SysHwIoA1EngSlope            = "sys.io.eng.AI.1.slope";
const char* SystemTagName::SysHwIoA2EngOffset           = "sys.io.eng.AI.2.offset";
const char* SystemTagName::SysHwIoA2EngSlope            = "sys.io.eng.AI.2.slope";
const char* SystemTagName::SysHwIoA3EngOffset           = "sys.io.eng.AI.3.offset";
const char* SystemTagName::SysHwIoA3EngSlope            = "sys.io.eng.AI.3.slope";
const char* SystemTagName::SysHwIoA4EngOffset           = "sys.io.eng.AI.4.offset";
const char* SystemTagName::SysHwIoA4EngSlope            = "sys.io.eng.AI.4.slope";
const char* SystemTagName::SysHwIoA5EngOffset           = "sys.io.eng.AI.5.offset";
const char* SystemTagName::SysHwIoA5EngSlope            = "sys.io.eng.AI.5.slope";
const char* SystemTagName::SysHwIoA6EngOffset           = "sys.io.eng.AI.6.offset";
const char* SystemTagName::SysHwIoA6EngSlope            = "sys.io.eng.AI.6.slope";
const char* SystemTagName::SysHwIoA7EngOffset           = "sys.io.eng.AI.7.offset";
const char* SystemTagName::SysHwIoA7EngSlope            = "sys.io.eng.AI.7.slope";
const char* SystemTagName::SysHwIoA8EngOffset           = "sys.io.eng.AI.8.offset";
const char* SystemTagName::SysHwIoA8EngSlope            = "sys.io.eng.AI.8.slope";

const char* SystemTagName::SysHwIoA1_4to20             = "sys.io.AI.1.enable4to20";
const char* SystemTagName::SysHwIoA2_4to20             = "sys.io.AI.2.enable4to20";
const char* SystemTagName::SysHwIoA3_4to20             = "sys.io.AI.3.enable4to20";
const char* SystemTagName::SysHwIoA4_4to20             = "sys.io.AI.4.enable4to20";
const char* SystemTagName::SysHwIoA5_4to20             = "sys.io.AI.5.enable4to20";
const char* SystemTagName::SysHwIoA6_4to20             = "sys.io.AI.6.enable4to20";
const char* SystemTagName::SysHwIoA7_4to20             = "sys.io.AI.7.enable4to20";
const char* SystemTagName::SysHwIoA8_4to20             = "sys.io.AI.8.enable4to20";


const char* SystemTagName::SysHwIoLOOPEnable         = "sys.io.LOOP.enable";
const char* SystemTagName::SysHwIoLOOPStatus         = "sys.io.LOOP.fault";
const char* SystemTagName::SysHwIoLOOPSelect         = "sys.io.LOOP.voltageRange";
const char* SystemTagName::SysHwIoLOOPDelay          = "sys.io.LOOP.delay";
const char* SystemTagName::SysHwIo5VEnable           = "sys.io.5V.enable";
const char* SystemTagName::SysHwIo5VStatus           = "sys.io.5V.fault";


const char* SystemTagName::SysHwIoD1Drive             = "sys.io.DI.1.enableDrive";
const char* SystemTagName::SysHwIoD2Drive             = "sys.io.DI.2.enableDrive";
const char* SystemTagName::SysHwIoD3Drive             = "sys.io.DI.3.enableDrive";
const char* SystemTagName::SysHwIoD4Drive             = "sys.io.DI.4.enableDrive";
const char* SystemTagName::SysHwIoD5Drive             = "sys.io.DI.5.enableDrive";
const char* SystemTagName::SysHwIoD6Drive             = "sys.io.DI.6.enableDrive";
const char* SystemTagName::SysHwIoD7Drive             = "sys.io.DI.7.enableDrive";
const char* SystemTagName::SysHwIoD8Drive             = "sys.io.DI.8.enableDrive";

bool SystemTagName::isSystemIOPointId(int ioId) {
    if (ioId >= 1 && ioId <= 11)
        return true;
    if (ioId >= 85000 && ioId <= 87002)
        return true;
    if (ioId >= 95000 && ioId <= 99000)
        return true;
    return false;
}


}}
