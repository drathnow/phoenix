
/** @file MockHardwareIO.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created May 11, 2017 by daver:
 * @endhistory
 */
#ifndef MOCKHARDWAREMANAGER_H_
#define MOCKHARDWAREMANAGER_H_

#include <gmock/gmock.h>
#include <zhal.h>
#include <stdint.h>
#include <string>

namespace zios {
namespace hal {

class IHardwareIO;
class IPowerModule;
class IAnalogIO;
class NetworkConfiguration;

class MockHardwareManager : public IHardwareManager
{
public:
    ~MockHardwareManager() {}

    MOCK_METHOD0(updateDigitalIn, void());
    MOCK_METHOD0(updatePowerModules, void());
    MOCK_METHOD1(updateRTDIn, bool(IHardwareIO* rtd));
    MOCK_METHOD1(powerUpADCs, bool(bool on));
    MOCK_METHOD0(writeDigitalOut, void());
    MOCK_METHOD1(hardwareIOForName, IHardwareIO*(const std::string& Source));
    MOCK_METHOD1(transceiverForPortType, ITransceiver*(int deviceType));
    MOCK_METHOD1(analogInputList, void(std::list<IAnalogIO*>& returnList));
    MOCK_METHOD1(powerModuleList, void(std::list<IPowerModule*>& powerModules));
    MOCK_METHOD1(internalHardwareIOForName, IHardwareIO*(const std::string& Source));
    MOCK_METHOD1(powerModuleForName, IPowerModule*(const std::string& powerModuleName));
    MOCK_METHOD1(systemVoltage, bool(float& returnVoltage));
    MOCK_METHOD1(systemTemperature, bool(float& returnTemperature));
    MOCK_METHOD1(ledForLedNumber, LED*(int ledNumber));
    MOCK_METHOD1(systemSerialPorts, void(std::vector<serial_port_info_t>& returnList));
    MOCK_METHOD1(transceiverForTranscieverNumber, ITransceiver*(int transceiverNumber));
    MOCK_METHOD1(buttonForNumber, Button*(int));
    MOCK_METHOD0(eeProm, EEProm*());
    MOCK_METHOD1(macAddress, std::string*(std::string& returnAddress));
    MOCK_METHOD0(networkConfiguration, NetworkConfiguration*());
    MOCK_CONST_METHOD0(serialNumber, const char*());
    MOCK_CONST_METHOD0(mainPCBSerial, const char*());
    MOCK_CONST_METHOD0(mainPCBVersion, const char*());
    MOCK_CONST_METHOD0(ioBoardType, const char*());
    MOCK_CONST_METHOD0(ioBoardPCBSerial, const char*());
    MOCK_CONST_METHOD0(ioBoardPCBVersion, const char*());
    MOCK_CONST_METHOD0(boardKey, const char*());
    MOCK_METHOD1(registerWakeButtonListener, void(WakeButtonListener& buttonListener));
};

}
}

#endif
