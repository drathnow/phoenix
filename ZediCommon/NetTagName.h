/** @file NetTagName.h
 *
 * @copyright
 *  Copyright 2014,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 14, 2015 by andrewj:
 * IncrDev Mar 24, 2015 by eb:  add in id numbers for Net points, rename to netTagName.h
 * @endhistory
 */
#ifndef NETTAGNAMES_H_
#define NETTAGNAMES_H_

#include <stdint.h>
#include <string>

namespace zios{
namespace net{


class NetTagName {
public:
    enum ParameterId {
        NetCellModemIMEI_ID         = 95000,
        NetCellModemModel_ID        = 95001,
        NetCellModemRevision_ID     = 95002,
        NetCellSimIMSI_ID           = 95003,
        NetCellSimId_ID             = 95004,
        NetCellNetOperator_ID       = 95005,
        NetCellNetMSISDN_ID         = 95006,
        NetCellNetReg_ID            = 95007,
        NetCellNetRSSI_ID           = 95008,
        NetCellNetBER_ID            = 95009,
        NetCellApnActive_ID         = 95010,
        NetCellApnRequested_ID      = 95011,
        NetCellDebug_ID             = 95012,
        NetCellRecycle_ID           = 95013,
        NetCellEnable_ID            = 95014,
        NetCellHostnameActive_ID    = 95015,
        NetCellHostnameRequested_ID = 95016,
        NetCellModemState_ID        = 95017,
        NetCellDataState_ID         = 95018,
        NetCellNetLocalIP_ID        = 95019,
        NetCellHwId_ID              = 95020,
        NetCellHwRevision_ID        = 95021,
        NetCellDataRxTimeout_ID     = 95022,
        NetCellDataConBackoff_ID    = 95023,
        NetCellDataConMax_ID        = 95024,
        NetCellDataConMaxTotal_ID   = 95025,

        NetEth0MACAddress_ID        = 95100,

        NetServerHostname_ID        = 95200,
        NetServerState_ID           = 95201,
        NetModemType_ID             = 95202,
        NetCellMaxDowntime_ID       = 95203
    };

    static const char* NetCellModemIMEI;
    static const char* NetCellModemModel;
    static const char* NetCellModemRevision;
    static const char* NetCellSimIMSI;
    static const char* NetCellSimId;
    static const char* NetCellNetOperator;
    static const char* NetCellNetMSISDN;
    static const char* NetCellNetReg;
    static const char* NetCellNetRSSI;
    static const char* NetCellNetBER;
    static const char* NetCellApnActive;
    static const char* NetCellApnRequested;
    static const char* NetCellDebug;
    static const char* NetCellRecycle;
    static const char* NetCellEnable;
    static const char* NetCellHostnameActive;
    static const char* NetCellHostnameRequested;
    static const char* NetCellModemState;
    static const char* NetCellDataState;
    static const char* NetCellNetLocalIP;
    static const char* NetCellHwId;
    static const char* NetCellHwRevision;
    static const char* NetCellDataRxTimeout;
    static const char* NetCellDataConBackoff;
    static const char* NetCellDataConMax;
    static const char* NetCellDataConMaxTotal;
    static const char* NetModemType;
    static const char* NetEth0MACAddress;
    static const char* NetServerHostname;
    static const char* NetServerState;
    static const char* NetCellMaxDowntime;
};

}} // namespace

#endif /* TAGNAMES_H_ */
