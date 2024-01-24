/** @file HardwareManager.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Feb. 27, 2019 by daver:
 * @endhistory
 */
#ifndef IHARDWAREMANAGER_H_
#define IHARDWAREMANAGER_H_

#include <string>
#include <list>
#include <vector>

namespace zios {
namespace hal {

class IPowerModule;
class ITransceiver;
class IPowerModule;
class IHardwareIO;
class IDigitalIO;
class Button;
class IAnalogIO;
class EEProm;
class LED;
class WakeButtonListener;
class NetworkConfiguration;

typedef struct
{
    std::string name;
    std::string device;
    std::string parameters;
    uint32_t type;
} serial_port_info_t;

class IHardwareManager
{
public:
    virtual ~IHardwareManager()
    {
    }

    /**
     * Returns a Button object so receiver can monitor the state of that button.
     *
     * @return WakeButton* - This is a transfer of ownership. Callers are responsible for
     * destroying this object.
     */
    virtual Button* buttonForNumber(int buttonNumber) = 0;

    /**
     * Returns an EEProm object for this hardware platform
     * @return
     */
    virtual EEProm* eeProm() = 0;

    /**
     * read all active digital in ports from actual hardware into internal holding registers.
     * this is a very fast call.
     */
    virtual void updateDigitalIn() = 0;

    /**
     * Apply power for ADC power supplies
     * @param[in] on  indicator for power on (false means off)
     * @retval true if successful
     * @retval false if unsuccessful
     */
    virtual bool powerUpADCs(bool on) = 0;

    /**
     * Returns a IHardwareIO for a given name.
     *
     * @param[in] Source -  the name of the Digital port to be mapped to the IOPoint
     * @return pointer to HW point
     * @retval NULL if not able to map
     */
    virtual IHardwareIO* hardwareIOForName(const std::string &Source) = 0;

    /**
     * Populates a list with all Analog Inputs
     *
     * @param returnList
     */
    virtual void analogInputList(std::list<IAnalogIO*> &returnList) = 0;

    /**
     * Returns a list of power modules
     *
     * @param powerModules
     */
    virtual void powerModuleList(std::list<IPowerModule*> &powerModules) = 0;

    /**
     * find a power module in list by name
     * @param powerModuleName - name of power module to find
     * @return non owned pointer to power module object if found
     * @retval NULL if port not found.
     */
    virtual IPowerModule* powerModuleForName(const std::string &powerModuleName) = 0;

    /**
     * Returns the voltage for the system
     *
     * @param returnVoltage - place to return the voltage
     *
     * @return true if return.  flase if error occurred.
     */
    virtual bool systemVoltage(float &returnVoltage) = 0;

    /**
     * Returns the temperature for the system
     *
     * @param returnTemperature - place to return the temperature
     *
     * @return true if return.  flase if error occurred.
     */
    virtual bool systemTemperature(float &returnTemperature) = 0;

    /**
     * Returns an LED for a given LED numbers.
     *
     * @param ledNumber - The LED number
     *
     * @return Point to an LED.  See implementation for ownership details of the object returned.
     */
    virtual LED* ledForLedNumber(int ledNumber) = 0;

    /**
     * Populates a list with serial_port_info_t objects representing each serial port on the
     * system
     *
     * @param returnList - List to return serial_port_info_t. This list will be cleared at the start
     * of the method.
     */
    virtual void systemSerialPorts(std::vector<serial_port_info_t> &returnList) = 0;

    /**
     * Returns the hardware MAC address for the eithernet device on this platform
     *
     * @param returnAddress - string object that will receive the hardware MAC address
     *
     * @return address of string passed in returnAddress, NULL if an error occurs.
     */
    virtual std::string* macAddress(std::string &returnAddress) = 0;

    /**
     * Returns a transceiver object for a given number.  The number is implemenation specific.
     *
     * @param transceiverNumber - An implementation specific transceiver.
     *
     * @return ITransceiver* - pointer to a transceiver.  NULL if the number is invalid.  See implemenation
     * details for ownership details of the object.
     */
    virtual ITransceiver* transceiverForTranscieverNumber(int transceiverNumber) = 0;

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
     * @return type of IOBoard. NULL if IO Board is not present.
     */
    virtual const char* ioBoardType() const = 0;

    /**
     *
     * @return S/N of IOBOARD PCB board. NULL if IO Board is not present.
     */
    virtual const char* ioBoardPCBSerial() const = 0;

    /**
     *
     * @return hw version of IOBoard pcb board. NULL if IO Board is not present.
     */
    virtual const char* ioBoardPCBVersion() const = 0;

    /**
     * Returns the hashed key for this board
     *
     * @return
     */
    virtual const char* boardKey() const = 0;

    /**
     * Registers a listener for wake button events.
     *
     * @param buttonListener - Reference to a button listener.  This is a transfer of ownership
     */
    virtual void registerWakeButtonListener(WakeButtonListener &buttonListener) = 0;

    /**
     * Returns the network configuration object for this platform.
     *
     * @return NetworkConfiguration* this is a transfer of ownership.
     */
    virtual NetworkConfiguration* networkConfiguration() = 0;
};

}
}

#endif /* IHARDWAREMANAGER_H_ */
