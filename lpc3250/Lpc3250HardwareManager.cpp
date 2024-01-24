/** @file Lpc3250HardwareManager.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 28, 2019 by daver:
 * @endhistory
 */

#include "Lpc3250HardwareManager.h"

#include <Exception.h>
#include <IAnalogIO.h>
#include <IHardwareIO.h>
#include <IHardwareManager.h>
#include <IPowerModule.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <NotificationCenter.h>
#include <LED.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <list>
#include <vector>
#include <File.h>

#include "lpc3250.h"
#include "Lpc3250Button.h"
#include "Lpc3250EEProm.h"
#include "Lpc3250NetworkConfiguration.h"
#include "Lpc3250Tranceiver.h"
#include "Lpc3250VoltageReader.h"
#include "Lpc3250WakeButtonMonitor.h"
#include "Lpc3250LEDHandler.h"
#include "Lpc3250ADC8344.h"
#include "Lpc3250AnalogInputMux.h"
#include "Lpc3250AnalogReader.h"
#include "Lpc3250GPIO.h"
#include "Lpc3250PowerModule.h"
#include "Lpc3250RTD.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250.Lpc3250HardwareManager");
static const char* NA_ = "N/A";

Lpc3250HardwareManager::Lpc3250HardwareManager(bool isTest) :
                _wakeButtonMonitor(NULL),
                _voltageReader(NULL),
                _ledHandler(NULL),
                _rs232Transceiver(NULL),
                _rs485Transceiver(NULL),
                _analogReader(NULL),
                _pollLed(NULL),
                _srvLed(NULL),
                _systemInfo(NULL),
                _lpc3250_model(FG400)
{
}

Lpc3250HardwareManager::Lpc3250HardwareManager(lpc3250_model model,
                                               system_info_t* systemInfo,
                                               chip_map_t& chipMap,
                                               hardware_io_map_t& hardwareIoMap,
                                               power_module_map_t& powerModules,
                                               Lpc3250VoltageReader& voltageReader,
                                               zios::common::NotificationCenter& notificationCenter,
                                               Lpc3250AnalogReader* analogReader) :
                _wakeButtonMonitor(NULL),
                _voltageReader(&voltageReader),
                _ledHandler(NULL),
                _rs232Transceiver(NULL),
                _rs485Transceiver(NULL),
                _analogReader(analogReader),
                _pollLed(NULL),
                _srvLed(NULL),
                _systemInfo(systemInfo),
                _lpc3250_model(model)
{
    _chipMap.insert(chipMap.begin(), chipMap.end());
    chipMap.clear();

    _hardwareIoMap.insert(hardwareIoMap.begin(), hardwareIoMap.end());
    hardwareIoMap.clear();

    _powerModulesMap.insert(powerModules.begin(), powerModules.end());
    powerModules.clear();

    //
    // Build the LED handler
    //
    _ledHandler = new Lpc3250LEDHandler(*ledForLedNumber(NET_LED), *ledForLedNumber(SRV_LED), *ledForLedNumber(POLL_LED));
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::CONNECTION_CONNECTED_NOTIFICATION);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::CONNECTION_LOST_NOTIFICATION);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::CONNECTION_SHUTTING_DOWN);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::TURN_ON_POLL_VOTE_LED);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::TURN_OFF_POLL_VOTE_LED);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::TURN_ON_NETWORK_LED);
    notificationCenter.registerHandler(_ledHandler, NotificationDescriptions::TURN_OFF_NETWORK_LED);
}

Lpc3250HardwareManager::~Lpc3250HardwareManager()
{
    for (power_module_map_t::iterator iter = _powerModulesMap.begin(); iter != _powerModulesMap.end(); iter++)
        ::delete iter->second;
    _powerModulesMap.clear();

    if (_wakeButtonMonitor != NULL)
        _wakeButtonMonitor->stop();

    delete _analogReader;
    delete _wakeButtonMonitor;

    lpc3250_free_chip_map(_chipMap);
    lpc3250_free_system_info(_systemInfo);
}

Button* Lpc3250HardwareManager::buttonForNumber(int buttonNumber)
{
    return new Lpc3250Button();
}

EEProm* Lpc3250HardwareManager::eeProm()
{
    return new Lpc3250EEProm();
}

void Lpc3250HardwareManager::updateDigitalIn()
{
}

bool Lpc3250HardwareManager::powerUpADCs(bool on)
{
    //
    // Turn main board ADC, which give us the system voltage monitoring.  This one
    // has to be here, if it's not, then something is seriously wrong.
    //
    power_module_map_t::iterator pmIter = _powerModulesMap.find("POW.LPC3250");
    if (pmIter == _powerModulesMap.end())
        THROW_EXCEPTION("System power module not found!");

    if (on)
        pmIter->second->turnOn();
    else
        pmIter->second->turnOff();

    if (_analogReader != NULL)
    {
        if (on)
            _analogReader->turnOn();
        else
            _analogReader->turnOff();
    }
    return true;
}

IHardwareIO* Lpc3250HardwareManager::hardwareIOForName(const std::string &name)
{
    hardware_io_map_t::iterator iter = _hardwareIoMap.find(name);
    return (iter == _hardwareIoMap.end()) ? NULL : iter->second;
}

void Lpc3250HardwareManager::analogInputList(std::list<IAnalogIO*> &returnList)
{
    for (hardware_io_map_t::iterator iter = _hardwareIoMap.begin(); iter != _hardwareIoMap.end(); iter++)
        if (isAnalogIo(iter->second->type()))
            returnList.push_back((IAnalogIO*) iter->second);
}

void Lpc3250HardwareManager::powerModuleList(std::list<IPowerModule*> &powerModules)
{
    for (power_module_map_t::iterator iter = _powerModulesMap.begin(); iter != _powerModulesMap.end(); iter++)
        powerModules.push_back(iter->second);
}

IPowerModule* Lpc3250HardwareManager::powerModuleForName(const std::string &name)
{
    power_module_map_t::const_iterator iter = _powerModulesMap.find(name);
    return iter == _powerModulesMap.end() ? NULL : iter->second;
}

bool Lpc3250HardwareManager::systemVoltage(float &returnVoltage)
{
    assert(_voltageReader != NULL);
    return _voltageReader->systemVoltage(returnVoltage);
}

bool Lpc3250HardwareManager::systemTemperature(float &returnTemperature)
{
    int value;

    FILE *file = ::fopen(LPC3250_TEMP_FILE, "r");
    if (file == NULL)
        return false;
    if (::fscanf(file, "%d", &value) <= 0)
    {
        ::fclose(file);
        return false;
    }
    ::fclose(file);
    returnTemperature = value / 1000.0f;
    return true;
}

LED* Lpc3250HardwareManager::ledForLedNumber(int ledNumber)
{
    switch (ledNumber)
    {
        //
        // The SIG and NET LEDs use pmws that are represented by sysfs entries so these
        // can be shared, although they could potentially result in odd behavior if they
        // are.  The only time they would be shared is during a reboot caused by holding
        // the "wake" button, which is rare, and would be shortly before a system restart
        // so we won't get too worked up over it.
        //
        case SIG_LED:
            return new Lpc3250LED(Lpc3250LED::SIG_LED_PATHS);

        case NET_LED:
            return new Lpc3250LED(Lpc3250LED::NET_LED_PATHS);

            //
            // The SRV and POL LEDs use gpio lines so they cannot be shared.  They
            // have to exists a singletons
            //
        case SRV_LED: {
            if (_srvLed == NULL)
            {
                assert(_chipMap.end() != _chipMap.find(LPC3250_GPO_P3_LABEL));
                gpiod_chip *chip = _chipMap.find(LPC3250_GPO_P3_LABEL)->second;
                assert(chip != NULL);
                gpiod_line *line = ::gpiod_chip_get_line(chip, LPC3250_LED_SRV_LINE);
                ::gpiod_line_request_output(line, "zios", 0);
                assert(line != NULL);
                _srvLed = new Lpc3250LED(line);
            }
            return _srvLed;
        }

        case POLL_LED: {
            if (_pollLed == NULL)
            {
                assert(_chipMap.end() != _chipMap.find(LPC3250_GPO_P3_LABEL));
                gpiod_chip *chip = _chipMap.find(LPC3250_GPO_P3_LABEL)->second;
                assert(chip != NULL);
                gpiod_line *line = ::gpiod_chip_get_line(chip, LPC3250_LED_POLL_LINE);
                ::gpiod_line_request_output(line, "zios", 0);
                assert(line != NULL);
                _pollLed = new Lpc3250LED(line);
            }
            return _pollLed;
        }
    }
    return NULL;
}

std::string* Lpc3250HardwareManager::macAddress(std::string &returnAddress)
{
    ifstream stream(LPC3250_MAC_ADDRESS_PATH);
    if (stream.is_open())
    {
        stream >> returnAddress;
        stream.close();
        return &returnAddress;
    }
    return NULL;
}

void Lpc3250HardwareManager::systemSerialPorts(std::vector<serial_port_info_t> &returnPorts)
{
    //
    // Technically, this information is stored in the lpc3250's main board eeprom but we are
    // hardcode it here because the code required to parse this would probably consume more room
    // than simply using statically allocated strings.
    //
    serial_port_info_t serialPortInfo;

    returnPorts.clear();

    serialPortInfo.name = "COM1";
    serialPortInfo.device = "/dev/ttyS0";
    serialPortInfo.parameters = "9600:8N1";
    serialPortInfo.type = PORT_TYPE_RS232;
    returnPorts.push_back(serialPortInfo);

    serialPortInfo.name = "COM2";
    serialPortInfo.device = "/dev/ttyS3";
    serialPortInfo.parameters = "9600:8N1";
    serialPortInfo.type = PORT_TYPE_RS485;
    returnPorts.push_back(serialPortInfo);

    serialPortInfo.name = "COM3";
    serialPortInfo.device = "/dev/ttyTX1";
    serialPortInfo.parameters = "9600:8N1";
    serialPortInfo.type = PORT_TYPE_RS232FC;
    returnPorts.push_back(serialPortInfo);
}

zios::hal::ITransceiver* Lpc3250HardwareManager::transceiverForTranscieverNumber(int transceiverNumber)
{
    //
    // There is only one RS485 port on the LPC3250 so we can create one here and return it.  If
    // another attempt is made to create an Lpc3250RS485Transceiver, it will throw an exception
    // because the gpo lines will already be in use.
    //
    // There are two RS232 ports so we have to maintain a singleton transceiver, even though
    // we turn on the RS232 Transceiver once and leave it on.
    //
    switch (transceiverNumber)
    {
        case 2:
            if (_rs485Transceiver == NULL)
                _rs485Transceiver = lpc3250_build_rs485_transceiver(_chipMap);
            return _rs485Transceiver;

        case 1:
            case 3:
            if (_rs232Transceiver == NULL)
                _rs232Transceiver = lpc3250_build_rs232_transceiver(_chipMap);
            return _rs232Transceiver;

        default:
            assert(false);
    }
    return NULL;
}

NetworkConfiguration* Lpc3250HardwareManager::networkConfiguration()
{
    Lpc3250NetworkConfiguration *networkConfig = new Lpc3250NetworkConfiguration();
    networkConfig->load();
    return networkConfig;
}

const char* Lpc3250HardwareManager::boardKey() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->main_board_info.key;
}

const char* Lpc3250HardwareManager::serialNumber() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->main_board_info.serial_number;
}

const char* Lpc3250HardwareManager::mainPCBSerial() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->main_board_info.pcb_serial;
}

const char* Lpc3250HardwareManager::mainPCBVersion() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->main_board_info.hwrev;
}

const char* Lpc3250HardwareManager::ioBoardType() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->ioboard_info == NULL ? NA_ : _systemInfo->ioboard_info->type;
}

const char* Lpc3250HardwareManager::ioBoardPCBSerial() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->ioboard_info == NULL ? NA_ : _systemInfo->ioboard_info->pcb_serial;
}

const char* Lpc3250HardwareManager::ioBoardPCBVersion() const
{
    assert(_systemInfo != NULL);
    return _systemInfo->ioboard_info == NULL ? NA_  : _systemInfo->ioboard_info->hwrev;
}

void Lpc3250HardwareManager::registerWakeButtonListener(WakeButtonListener &buttonListener)
{
    _wakeButtonMonitor = new Lpc3250WakeButtonMonitor(&buttonListener);
    _wakeButtonMonitor->start();
}

} /* namespace hal */
} /* namespace zios */
