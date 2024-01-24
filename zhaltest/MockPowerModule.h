/** @file MockPowerModule.h
 *
 * @copyright
 *  Copyright 2015,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 17, 2016 by eganb:
 * @endhistory
 */
#ifndef MOCKPOWERMODULE_H_
#define MOCKPOWERMODULE_H_

#include <stdint.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ReturnCodes.h>
#include <zhal.h>

namespace zios {
namespace hal {

class MockPowerModule: public IPowerModule{
public:

    MockPowerModule(){}
    ~MockPowerModule(){}

    MOCK_METHOD0(init, bool());
    MOCK_METHOD2(convert, int32_t( uint32_t address, uint32_t delay));
    MOCK_METHOD2(convertVoltage, double( uint32_t address, uint32_t delay));
    MOCK_METHOD0(lastConvert, int32_t());
    MOCK_METHOD0(lastVoltageConvert, double());
    MOCK_METHOD1(voltageConvert, double(int32_t rawConvert));

    MOCK_METHOD0(turnOn, IPowerModule::POWER_STATE());
    MOCK_METHOD0(turnOff, IPowerModule::POWER_STATE());
    MOCK_METHOD0(isOn, bool());

    MOCK_CONST_METHOD0(name, const char*());
    MOCK_CONST_METHOD0(label, const char*());
    MOCK_CONST_METHOD0(displayOrder,  int());
    MOCK_CONST_METHOD0(isFaulted,  bool());
    MOCK_CONST_METHOD0(isInternalPowerSupply,  bool());
    MOCK_CONST_METHOD0(enableDelayMilliseconds, int32_t());
    MOCK_METHOD1(setEnableDelayMilliseconds, void(int32_t delayMilliseconds));
    MOCK_CONST_METHOD0(rangeCount,  size_t());
    MOCK_CONST_METHOD0(rangeList,  const std::vector<uint32_t>&());
    MOCK_METHOD1(setSelectRange, uint32_t(uint32_t range));
    MOCK_CONST_METHOD0(selectedRange,  uint32_t());
    MOCK_CONST_METHOD0(currentRangeValue,  uint32_t());
    MOCK_METHOD1(setModeId, void(uint32_t id));
    MOCK_CONST_METHOD0(modeId, uint32_t());
    MOCK_METHOD1(setFaultId, void(uint32_t id));
    MOCK_CONST_METHOD0(faultId, uint32_t());
    MOCK_METHOD1(setSelectId, void(uint32_t id));
    MOCK_CONST_METHOD0(selectId, uint32_t());
    MOCK_METHOD0(powerState, IPowerModule::POWER_STATE());
};

}
}

#endif /* MOCKPOWERMODULE_H_ */
