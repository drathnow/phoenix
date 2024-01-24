/** @file NetTagName.cpp
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
 * IncrDev Mar 24, 2015 by eb:  add in id numbers for Net points, rename to netTagName.cpp
 * @endhistory
 */

#include "NetTagName.h"

namespace zios {
namespace net {

const char* NetTagName::NetCellApnActive            = "net.cell.apn.active";
const char* NetTagName::NetCellApnRequested         = "net.cell.apn.requested";
const char* NetTagName::NetCellDebug                = "net.cell.debug";
const char* NetTagName::NetCellEnable               = "net.cell.enable";
const char* NetTagName::NetCellHostnameActive       = "net.cell.hostname.active";
const char* NetTagName::NetCellHostnameRequested    = "net.cell.hostname.requested";
const char* NetTagName::NetCellHwId                 = "net.cell.hw.id";
const char* NetTagName::NetCellHwRevision           = "net.cell.hw.revision";
const char* NetTagName::NetCellModemIMEI            = "net.cell.modem.imei";
const char* NetTagName::NetCellModemModel           = "net.cell.modem.model";
const char* NetTagName::NetCellModemRevision        = "net.cell.modem.revision";
const char* NetTagName::NetCellNetLocalIP           = "net.cell.net.localip";
const char* NetTagName::NetCellNetOperator          = "net.cell.net.operator";
const char* NetTagName::NetCellNetMSISDN            = "net.cell.net.msisdn";
const char* NetTagName::NetCellNetReg               = "net.cell.net.register";
const char* NetTagName::NetCellNetRSSI              = "net.cell.net.rssi";
const char* NetTagName::NetCellNetBER               = "net.cell.net.ber";
const char* NetTagName::NetCellSimId                = "net.cell.sim.id";
const char* NetTagName::NetCellSimIMSI              = "net.cell.sim.imsi";
const char* NetTagName::NetCellRecycle              = "net.cell.recycle";
const char* NetTagName::NetCellModemState           = "net.cell.modem.state";
const char* NetTagName::NetCellDataState            = "net.cell.data.state";
const char* NetTagName::NetCellDataRxTimeout        = "net.cell.data.rxtimeout";
const char* NetTagName::NetCellDataConBackoff       = "net.cell.data.con.backoff";
const char* NetTagName::NetCellDataConMax           = "net.cell.data.con.max";
const char* NetTagName::NetCellDataConMaxTotal      = "net.cell.data.con.maxtotal";
const char* NetTagName::NetModemType                = "net.modem.type";

const char* NetTagName::NetEth0MACAddress           = "net.eth0.macaddress";

const char* NetTagName::NetServerHostname           = "net.server.hostname";
const char* NetTagName::NetServerState              = "net.server.state";
const char* NetTagName::NetCellMaxDowntime          = "net.cell.max.downtime";

}
} // namespace



