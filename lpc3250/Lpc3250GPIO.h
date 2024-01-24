/** @file DigitalIO.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 18, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250GPIO_H_
#define LPC3250GPIO_H_

#include <IDigitalIO.h>
#include <commonIncludes.h>
#include <gpiod.h>
#include <string>

#include "lpc3250.h"

namespace zios {
namespace lpc3250 {

class Lpc3250Chip;


typedef struct {
    const char* mappingName;
    const char* chipLabel;
    uint8_t lineNumber;
    bool isReadOnly;
    bool isInverted;
    const char* swScanGpioName; // Only DIs 1-8 use this.
} gpio_config_t ;

class Lpc3250GPIO : public zios::hal::IDigitalIO
{
public:
    Lpc3250GPIO(const char* name, gpiod_line* line, PORTTYPE portType, Lpc3250GPIO* swScanGpio = NULL);
    ~Lpc3250GPIO();

    /**
     * return temporary reference for name of port
     * @return reference to name of port as a c string (null terminated).
     */
    const char* name() const;

    /**
     * return the label name of port that is stenciled on case
     * @return refefence to stencil name on case for port (null terminated)
     */
    const char* label() const;

    /**
     * This is a getter for integer type used for int8 and int32
     * If the output type is float, this will return 0.
     * @return integer32 value from the hardware pin
     */
    uint32_t getInt();

    /**
     * This is a getter for floating point used for, well, floating point.
     * If the output type of hardware point is Int, then this returns 0.
     * @return floating value
     */
    float getFloat();

    /**
     * gets the output type of the hardware point.
     * could be BOOL_T for a boolean type (or a unsigned 8 bit value)
     * or an INT32_t for a 32 bit integer , like a pulse counter
     * or a floating point value like for an adc
     * Note: should be thread safe....
     *
     * @retval BOOL_T  boolean(0/1)
     * @retval INT32_T 32 bit unsigned integer
     * @retval FLOAT_T floating point
     */
    IHardwareIO::DATATYPE getResponseType() const;

    /**
     * Returns current longtimestamp for value in hardware port
     *
     * @return current time of value
     */
    long_timestamp_t timestamp() const;

    /**
     * is this "port" readonly
     * @retval true if read only
     * @retval false if writable (DO only at moment)
     */
    bool isReadOnly() const;

    /**
     * Writes a value to a Digital Output.
     *
     * @param value - true of false (1 or 0
     *
     * @return True if the write was successful, false if the write failed OR if this
     * this IO is read-only.
     */
    bool writeBool(bool value);

    /**
     * Returns a reference to a swScan control IO on a Digital Input, if applicable.
     *
     * @return Pointer to a DigitalIO.  NULL if not applicable to this DI.
     * This is not a transfer of ownership.
     */
    IDigitalIO* getReferenceForScan();

    /**
     * Not applicable to the Lpc3250 because write operations are immediate.
     */
    void forceUpdate();

    /**
     * Not applicable to the Lpc3250 because read operations are immediate.
     */
    void forceRead();

    /**
     * Builds an Lpc3250GPIO object from a defintion gernally found in the eeprom.  The format of the line is:
     *
     *  <gpio-name>=chip=<chip-name>;line=<line-no>;ro=<is-readonly>;inv=<is-invert>[;sws=<sws-gpio>]
     *
     *  Where
     *      <gpio-name> - Is the name the GPIO is know by
     *      chip        - Is the GPIO chip name
     *      line        - The line number on the gpio chip
     *      ro          - Specifies if this gpio is readonly (1 = yes, 0 = no)
     *      inv         - Specifies is this gpio is inverted (1 = yes, 0 = no)
     *      sws         - Options gpio.  Used with AIDI ports 1-8 to switch them from reading
     *                    analog voltage to ditial (1, 0)
     *
     *
     * @param partiationLine - The GPIO line
     *
     * @param chipMap - Map containing gpiod_chip objects
     *
     * @param hardwareIOs - Hardware IO map that is searched for a GPIO matching the name specified by sws.
     *
     * @return Lpc3250GPIO, this is a transfer of owner shop, NULL if an error is detected. Cause logged
     * to system error log.
     */
    static Lpc3250GPIO* buildGpioFromPartitionLine(const std::string& partiationLine, const chip_map_t& chipMap, const hardware_io_map_t& hardwareIOs);

    static hardware_io_map_t* buildGpiosFromPartition(const std::string &partition, const chip_map_t &chipMap, hardware_io_map_t &hardwareIoMap);

    static Lpc3250GPIO* buildGpioFromGpioInfo(gpio_config_t& gpio, const chip_map_t& chipMap, const std::map<std::string, IHardwareIO*>& retHardwareIOs);

//    /**
//     *  Builds digital IO object available on the LPC3250.
//     *
//     */
//    static void buildGPIOsWithChips(lpc3250_model model, const chip_map_t& chipMap, std::map<std::string, zios::hal::IHardwareIO*>& retHardwareIOs);

private:
    char* _name;
    gpiod_line* _digitalLine;
    Lpc3250GPIO* _swScanGpio;
};

}
}
#endif /* LPC3250GPIO_H_ */
