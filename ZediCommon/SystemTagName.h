/** @file SystemTagName.h
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
 * IncrDev Apr 11, 2016 by eb:    change tags for IOBoard system points
 * @endhistory
 */
#ifndef SYSTEMTAGNAME_H_
#define SYSTEMTAGNAME_H_

#include <stdint.h>


namespace zios {
namespace common {

#define OUTPUT_COILS_ID                 1
#define INPUT_COILS_ID                  2
#define INPUT_REGISTER_ID               3
#define HOLDING_REGISTER_ID             4


class SystemTagName {
public:

    enum ParameterId {

        // General Main system points
        SysSerialnumber_ID          = 1,
        SysFirmwareRev_ID           = 2,
        SysHwMainID_ID              = 3,
        SysHwMainRev_ID             = 4,
        SysHwMainVolt_ID            = 10,
        SysHwMainTemp_ID            = 11,

        // General IO Board points
        SysHwIoID_ID                = 85000,
        SysHwIoREV_ID               = 85001,
        SysHwIoType_ID              = 85002,

        // Power points
        SysHwIoLOOPEnable_ID        = 85095,
        SysHwIoLOOPStatus_ID        = 85096,
        SysHwIoLOOPSelect_ID        = 85097,
        SysHwIo5VEnable_ID          = 85098,
        SysHwIo5VStatus_ID          = 85099,

        // AI configuration and calibration(future) points
        SysHwIoA1EngOffset_ID       = 85100,
        SysHwIoA1EngSlope_ID        = 85101,
        SysHwIoA1CalOffset_ID       = 85102,
        SysHwIoA1CalSlope_ID        = 85103,
        SysHwIoA2EngOffset_ID       = 85104,
        SysHwIoA2EngSlope_ID        = 85105,
        SysHwIoA2CalOffset_ID       = 85106,
        SysHwIoA2CalSlope_ID        = 85107,
        SysHwIoA3EngOffset_ID       = 85108,
        SysHwIoA3EngSlope_ID        = 85109,
        SysHwIoA3CalOffset_ID       = 85110,
        SysHwIoA3CalSlope_ID        = 85111,
        SysHwIoA4EngOffset_ID       = 85112,
        SysHwIoA4EngSlope_ID        = 85113,
        SysHwIoA4CalOffset_ID       = 85114,
        SysHwIoA4CalSlope_ID        = 85115,
        SysHwIoA5EngOffset_ID       = 85116,
        SysHwIoA5EngSlope_ID        = 85117,
        SysHwIoA5CalOffset_ID       = 85118,
        SysHwIoA5CalSlope_ID        = 85119,
        SysHwIoA6EngOffset_ID       = 85120,
        SysHwIoA6EngSlope_ID        = 85121,
        SysHwIoA6CalOffset_ID       = 85122,
        SysHwIoA6CalSlope_ID        = 85123,
        SysHwIoA7EngOffset_ID       = 85124,
        SysHwIoA7EngSlope_ID        = 85125,
        SysHwIoA7CalOffset_ID       = 85126,
        SysHwIoA7CalSlope_ID        = 85127,
        SysHwIoA8EngOffset_ID       = 85128,
        SysHwIoA8EngSlope_ID        = 85129,
        SysHwIoA8CalOffset_ID       = 85130,
        SysHwIoA8CalSlope_ID        = 85131,

        // AI 4to20  Control
        SysHwIoA1_4to20_ID          = 85200,
        SysHwIoA2_4to20_ID          = 85201,
        SysHwIoA3_4to20_ID          = 85202,
        SysHwIoA4_4to20_ID          = 85203,
        SysHwIoA5_4to20_ID          = 85204,
        SysHwIoA6_4to20_ID          = 85215,
        SysHwIoA7_4to20_ID          = 85216,
        SysHwIoA8_4to20_ID          = 85217,

        // DI Scan control
        SysHwIoD1Drive_ID           = 85301,
        SysHwIoD2Drive_ID           = 85302,
        SysHwIoD3Drive_ID           = 85303,
        SysHwIoD4Drive_ID           = 85304,
        SysHwIoD5Drive_ID           = 85305,
        SysHwIoD6Drive_ID           = 85306,
        SysHwIoD7Drive_ID           = 85307,
        SysHwIoD8Drive_ID           = 85308,

        // HACK to set limits on Modbus polling
        SysMbsOutputCoilsPollLimit_ID  = 86000,
        SysMbsInputCoilsPollLimit_ID   = 86001,
        SysMbsInputRegPollLimit_ID     = 86002,
        SysMbsHoldingRegPollLimit_ID   = 86003,

        SysHWIoLOOPDelay_ID            = 87000,

        // Another HACK. ROC protocol values
        SysRocSrcAddress_ID            = 87001,
        SysRocSrcGroup_ID              = 87002,

        //
        // Holds the server (bridge) address
        //
        SysNetSrvAddress_ID            = 87003

//  Network manager reserves 95000 to 99000...

    };

    // System power names

    static const char* LOOP_POWER_NAME;
    static const char* V5_POWER_NAME;

    // General Main system points

    static const char* SysSerialnumber;
    static const char* SysFirmwareRev;
    static const char* SysHwMainID;
    static const char* SysHwMainRev;
    static const char* SysHwMainVolt;
    static const char* SysHwMainTemp;

    static const char* SysHwIoID;
    static const char* SysHwIoREV;
    static const char* SysHwIoType;

//  AI calibration points....
    static const char* SysHwIoA1EngOffset;
    static const char* SysHwIoA1EngSlope;
    static const char* SysHwIoA2EngOffset;
    static const char* SysHwIoA2EngSlope;
    static const char* SysHwIoA3EngOffset;
    static const char* SysHwIoA3EngSlope;
    static const char* SysHwIoA4EngOffset;
    static const char* SysHwIoA4EngSlope;
    static const char* SysHwIoA5EngOffset;
    static const char* SysHwIoA5EngSlope;
    static const char* SysHwIoA6EngOffset;
    static const char* SysHwIoA6EngSlope;
    static const char* SysHwIoA7EngOffset;
    static const char* SysHwIoA7EngSlope;
    static const char* SysHwIoA8EngOffset;
    static const char* SysHwIoA8EngSlope;

    // Loop Control for AI
    static const char* SysHwIoA1_4to20;
    static const char* SysHwIoA2_4to20;
    static const char* SysHwIoA3_4to20;
    static const char* SysHwIoA4_4to20;
    static const char* SysHwIoA5_4to20;
    static const char* SysHwIoA6_4to20;
    static const char* SysHwIoA7_4to20;
    static const char* SysHwIoA8_4to20;


    // Power points
    static const char* SysHwIoLOOPEnable;
    static const char* SysHwIoLOOPStatus;
    static const char* SysHwIoLOOPSelect;
    static const char* SysHwIoLOOPDelay;
    static const char* SysHwIo5VEnable;
    static const char* SysHwIo5VStatus;
    static const char* SysHwIoSWSCAN;

    // DI control points
    static const char* SysHwIoD1Drive;
    static const char* SysHwIoD2Drive;
    static const char* SysHwIoD3Drive;
    static const char* SysHwIoD4Drive;
    static const char* SysHwIoD5Drive;
    static const char* SysHwIoD6Drive;
    static const char* SysHwIoD7Drive;
    static const char* SysHwIoD8Drive;

    static const char* SysMbsOutputCoilsPollLimit;
    static const char* SysMbsInputCoilsPollLimit;
    static const char* SysMbsInputRegPollLimit;
    static const char* SysMbsHoldingRegPollLimit;
    static const char* SysRocSrcAddress;
    static const char* SysRocSrcGroup;
    static const char* SysNetSrvAddress;

    /**
     * Tells you whether or not an IO ID is a system piont ID, as defined by this class.  In order to
     * maintain the integrity of this method, you must be sure to update this method with any changes
     * (additions or deletions) to the list of known system tag IDs.
     *
     * @param ioId
     * @return
     */
    static bool isSystemIOPointId(int ioId);

};

}} // namespace



#endif /* SYSTEMTAGNAME_H_ */
