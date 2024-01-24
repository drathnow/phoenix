/** @file Lpc3250HardwareManager.h 
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
#ifndef LPC3250HARDWAREMANAGER_H_
#define LPC3250HARDWAREMANAGER_H_

#include <zhal.h>
#include <map>
#include <string>
#include <ReadWriteLock.h>

#include "Lpc3250AnalogInput.h"
#include "Lpc3250LED.h"

namespace zios {
namespace common {
    class NotificationCenter;
}
namespace lpc3250 {

using namespace std;
using namespace zios::hal;
using namespace zios::common;

class Lpc3250EEProm;
class Lpc3250WakeButtonMonitor;
class Lpc3250VoltageReader;
class Lpc3250LEDHandler;
class Lpc3250RS232Transceiver;
class Lpc3250RS485Transceiver;

/**
 * So here are a few facts about the LPC3250 hardware platforms:
 *
 * Chips:  The platform has a total of 7 chips that are mapped to folders in
 * /sys/bus/gpio/devices/.  6 will be present on the FG400 with a seventh present
 * on the FG600.  The names of the chips can change from 400 to 600 but the labels
 * are always the same.
 *
 *
 *  Name        Label           Lines
 *  ----        -----           -----
 * gpiochip0    2-0021          16
 * gpiochip1    gpio_p0         8
 * gpiochip2    gpio_p1         24
 * gpiochip3    gpio_p2         13
 * gpiochip4    gpio_p3         6
 * gpiochip5    gpi_p3          29
 * gpiochip6    gpo_p3          24
 *
 */

class Lpc3250HardwareManager: public IHardwareManager
{
public:
    Lpc3250HardwareManager(lpc3250_model model,
                           system_info_t* systemInfo,
                           chip_map_t& chipMap,
                           hardware_io_map_t& hardwareIoMap,
                           power_module_map_t& powerModules,
                           Lpc3250VoltageReader& voltageReader,
                           zios::common::NotificationCenter& notificationCenter,
                           Lpc3250AnalogReader* analogReader = NULL);
    ~Lpc3250HardwareManager();

    /**
     * Returns a Button object for a specific button number.  Current button numbers
     * supported are defined in lpc3250.h
     *
     *      WAKE_BUTTON_NUMBER - The wake button on the back of the FG400/600.
     *
     * @param buttonNumber
     * @return Button* - This is a transfer of ownership.
     */
    Button* buttonForNumber(int buttonNumber);

    /**
     * Returns an EEProm for the LPC3250 platform.  The platform contains either two or three
     * eeproms, depending on whether this is an FG600 or FG400.
     *
     * @return EEprom* - This is a transfer of ownership. Receivers should clean up this object
     * when done.
     */
    EEProm* eeProm();

    /**
     * read all active digital in ports from actual hardware into internal holding registers.
     * this is a very fast call.
     */
    void updateDigitalIn();

    /**
     * Apply power for ADC power supplies
     * @param[in] on  indicator for power on (false means off)
     * @retval true if successful
     * @retval false if unsuccessful
     */
    bool powerUpADCs(bool on);

    /**
     * Returns a IHardwareIO for a given name.
     *
     * @param[in] Source -  the name of the Digital port to be mapped to the IOPoint
     * @return pointer to HW point
     * @retval NULL if not able to map
     */
    IHardwareIO* hardwareIOForName(const std::string& name);

    /**
     * Populates a list with all Analog Inputs
     *
     * @param returnList
     */
    void analogInputList(std::list<IAnalogIO*>& returnList);

    /**
     * Returns a list of power modules
     *
     * @param powerModules
     */
    void powerModuleList(std::list<IPowerModule*>& powerModules);

    /**
     * find a power module in list by name
     * @param name - name of power module to find
     * @return non owned pointer to power module object if found
     * @retval NULL if port not found.
     */
    IPowerModule* powerModuleForName(const std::string& name);

    /**
     * Returns the voltage for the system
     *
     * @param returnVoltage - place to return the voltage
     *
     * @return true if return.  flase if error occurred.
     */
    bool systemVoltage(float& returnVoltage);

    /**
     * Returns the temperature for the system
     *
     * @param returnTemperature - place to return the temperature
     *
     * @return true if return.  flase if error occurred.
     */
    bool systemTemperature(float& returnTemperature);

    /**
     * Returns an LED for a given LED numbers.
     *
     * @param ledNumber - The LED number
     *
     *      SIG_LED
     *      NET_LED
     *      SRV_LED
     *      POLL_LED
     *
     * @return Point to an LED. NULL if the LED number is invalid. This is a transfer of ownership.
     *
     */
    LED* ledForLedNumber(int ledNumber);

    /**
     * Returns the hardware MAC address for the eithernet device on this platform
     *
     * @param returnAddress - string object that will receive the hardware MAC address
     *
     * @return address of string passed in returnAddress, NULL if an error occurs.
     */
    std::string* macAddress(std::string& returnAddress);

    /**
     * Populates a list with serial_port_info_t objects representing each serial port on the
     * system
     *
     * @param returnList - List to return serial_port_info_t. This list will be cleared at the start
     * of the method.
     */
    void systemSerialPorts(std::vector<serial_port_info_t>& returnList);

    /**
     *
     * @return serial number of FiG
     */
    const char*  serialNumber() const;

    /**
     *
     * @return S/N of Main PCB board
     */
    const char*  mainPCBSerial() const;

    /**
     *
     * @return hw version of mainpcb board
     */
    const char*  mainPCBVersion() const;

    /**
     *
     * @return type of IOBoard. NULL if IO Board is not present.
     */
    const char*  ioBoardType() const;

    /**
     *
     * @return S/N of IOBOARD PCB board. NULL if IO Board is not present.
     */
    const char*  ioBoardPCBSerial() const;

    /**
     *
     * @return hw version of IOBoard pcb board. NULL if IO Board is not present.
     */
    const char*  ioBoardPCBVersion() const;

    /**
     * Returns the hashed board key
     *
     * @return
     */
    const char* boardKey() const;

    /**
     * Registers a listener for wake button events.
     *
     * @param buttonListener - Reference to a button listener.  This is a tranfer of ownership.
     */
    void registerWakeButtonListener(WakeButtonListener& buttonListener);

    /**
     * Returns an ITransceiver object for a Transceiver Number
     *
     * @param transceiverNumber - The number
     *
     * @return ITransceiver*.  This is a transfer of ownership.  Receiver is responsible for cleanup
     */
    zios::hal::ITransceiver* transceiverForTranscieverNumber(int transceiverNumber);

    /**
     * Returns the network configuration for the LPC3250 platform
     *
     * @return NetworkConfiguration* this is a transfer of ownership.
     */
    NetworkConfiguration* networkConfiguration();


    friend class Lpc3250HardwareManagerTest;

private:
    Lpc3250WakeButtonMonitor* _wakeButtonMonitor;
    Lpc3250VoltageReader* _voltageReader;
    Lpc3250LEDHandler* _ledHandler;
    Lpc3250RS232Transceiver* _rs232Transceiver;
    Lpc3250RS485Transceiver* _rs485Transceiver;
    Lpc3250AnalogReader* _analogReader;
    Lpc3250LED* _pollLed;
    Lpc3250LED* _srvLed;
    power_module_map_t _powerModulesMap;
    hardware_io_map_t _hardwareIoMap;
    chip_map_t _chipMap;
    system_info_t* _systemInfo;
    lpc3250_model _lpc3250_model;

    Lpc3250HardwareManager(bool isTest);
};

} /* namespace hal */
} /* namespace zios */

#endif /* LPC3250HARDWAREMANAGER_H_ */
