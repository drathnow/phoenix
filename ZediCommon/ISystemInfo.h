/** @file ISystemInfo.h 
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
#ifndef ISYSTEMINFO_H_
#define ISYSTEMINFO_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace zios {
namespace common {

/**
 * holds information about Async ports, held by EEPROM
 */
struct ee_port {
    std::string name;
    std::string device;
    std::string parameters;
    uint32_t type;
    uint32_t transceiver;
};

/**
 * holds information about Async ports, held by EEPROM
 */

class ISystemInfo
{
public:
    virtual ~ISystemInfo()
    {
    }

    /**
     *
     * @return serial number of FiG
     */
    virtual const char* serialNumber() const = 0;

    /**
     *
     * @return S/N of Main PCB board
     */
    virtual const char* mainPCBSerial() const = 0;

    /**
     *
     * @return hw version of mainpcb board
     */
    virtual const char* mainPCBVersion() const = 0;

    /**
     *
     * @return shared Key
     */
    virtual const std::vector<uint8_t>& sharedKey() const = 0;

    /**
     *
     * @return MAC address for FiG (eth0)
     */
    virtual const std::string* macAddress(std::string& retMacAddress) const = 0;

    /**
     *
     * @return FirmwareVersion if zios
     */
    virtual const std::string& ziosVersion() const = 0;

    /**
     *
     * @return type of IOBoard
     */
    virtual const char* ioBoardType() const = 0;

    /**
     *
     * @return S/N of IOBOARD PCB board
     */
    virtual const char* ioBoardPCBSerial() const = 0;

    /**
     *
     * @return hw version of IOBoard pcb board
     */
    virtual const char* ioBoardPCBVersion() const = 0;
};

}
}

#endif /* ISYSTEMINFO_H_ */
