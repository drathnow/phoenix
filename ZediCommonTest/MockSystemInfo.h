/** @file MockSystemInfo.h
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr 1, 2015 by eganb:
 * IncrDev Apr 08, 2015 by eb: update functions from real interface
 * IncrDev May 22, 2015 by eb: add in new function to support real object change
 * IncrDev Jun 26, 2015 by eb: rejig mock from refactoring of class
 * IncrDev Dec 11, 2015 by eb: add in IOBoard functions
 * @endhistory
 */
#ifndef MOCKSYSTEMINFO_H_
#define MOCKSYSTEMINFO_H_

#include <gmock/gmock.h>
#include <ISystemInfo.h>
#include <string>

namespace zios {
namespace common {

class MockSystemInfo: public ISystemInfo {
public:
    MockSystemInfo() {
    }
    ~MockSystemInfo() {
    }

    MOCK_CONST_METHOD0(serialNumber, const char*());
    MOCK_CONST_METHOD0(mainPCBSerial, const char*());
    MOCK_CONST_METHOD0(mainPCBVersion, const char*());
    MOCK_CONST_METHOD0(sharedKey, const std::vector<uint8_t>&());
    MOCK_CONST_METHOD1(macAddress, const std::string*(std::string& retMacAddress));
    MOCK_CONST_METHOD0(ziosVersion, const std::string&());
    MOCK_CONST_METHOD0(ioBoardType, const char*());
    MOCK_CONST_METHOD0(ioBoardPCBSerial, const char*());
    MOCK_CONST_METHOD0(ioBoardPCBVersion, const char*());
};

}
}  // namespace zios

#endif /* MOCKSYSTEMINFO_H_ */
