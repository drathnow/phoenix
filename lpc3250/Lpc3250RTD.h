/** @file Lpc3250RTD.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 17, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250RTD_H_
#define LPC3250RTD_H_

#include "lpc3250.h"

#include <stdint.h>
#include <IHardwareIO.h>
#include <RTD.h>
#include <string>

namespace zios {
namespace hal {
    class IPowerModule;
}
namespace lpc3250 {

/**
 *
 * In order to read a value from the sense, reference, and ground pins we need to first
 * select the channel of the SN74LV4051APWR multiplexer (mux) and then read the approriate
 * channel on the ADS8344 ADC. These two numbers are encoded into a single 8 bit number
 * where bits 0-2 are the mux channel and bits 3-5 are the adc channel number:
 *
 *        5  4  3  2  1  0
 * +--+--+--+--+--+--+--+--+
 * |  |  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+--+
 *        <muxadr> <adcchan>
 *         3 bits   3 bits
 */
#define MUX_CHANNEL(address)    (address & 0x7)
#define ADC_CHANNEL(address)    ((address & 0x38) >> 3)

typedef struct
{
    const char* name;
    uint8_t senseChannel;
    uint8_t referenceChannel;
    uint8_t groundChannel;
    uint32_t channelDelay;
} rtd_channel_t;

#define RTD_CHANNEL_COUNT           4
#define RTD_POWER_DELAY_uSEC        2000    // uSeconds

class Lpc3250AnalogReader;

/**
 * The Lpc3250RTD represents an RTD port on the back of the FG600 that consists of three lines: sense,
 * reference, and ground.  The RTD offers readings from the ADC through a filtered and non filtered
 * channel but this class will only take readings from the filtered (Low Pass Filter) channel.
 *
 */
class Lpc3250RTD: public zios::hal::RTD
{
public:

    static const double TERM1;      // -A/2B
    static const double TERM2;      // A^2/4B^2
    static const double TERM3;      // 1/B
    static const double GAIN;
    static const double CORRECTION_FACTOR;
    static const double REFERENCE_RESISTANCE;

    Lpc3250RTD(const char *name,
               char senseChannelAddress,
               char referenceChannelAddress,
               char groundChannelAddress,
               Lpc3250AnalogReader &analogReader,
               struct gpiod_line* rtdPowerLine,
               uint32_t delayMicroSec = RTD_POWER_DELAY_uSEC);

    ~Lpc3250RTD();

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
     * Part of the RTD interface.  This method doesn't actually do anything.  It is intended to
     * be used for delayed power on object but since RTDs control their own power source, it doesn't
     * need this feature.
     *
     * @return 0
     */
    double convertTemperature();

    /**
     * Method not implemented
     *
     */
    long_timestamp_t timestamp() const;

    /**
     * is this "port" readonly
     * @retval true if read only
     * @retval false if writable (DO only at moment)
     */
    bool isReadOnly() const;

    /**
     * This method is not implemented.
     *
     * @param value
     * @return
     */
    bool writeBool(bool value);

    /**
     * Uses the internal calculation to convert a resistence to a temperature
     *
     * @param resistance - Resistance value
     *
     * @return Tempurature
     */
    static double temperatureForResistance(float resistance);

    /**
     * Builds an Lpc3250RTD object from a configuration line, generally stored in the IO Boards eeprom.
     *
     * @param partitionLine - The partition line.
     *
     *      <rtd-name>=senschan=<sens-chan>;refchan=<ref-chan>;gndchan=<grnd-chan>;delay=<power-on-delay>
     *
     *      NOTES:
     *          - See top of this file for format of <sens-chan>, <ref-chan>, and <grnd-chan>
     *
     *          - delay is the amount of time, in micro seconds, to wait after turning on the RTD power line
     *            and taking a sample
     *
     * @param rtdPowerLine - RTD gpio power line used to turn power on and off to the RTDs.
     *
     * @param analogReader - Analog reader used to retrieve values from the RTD's sense, ref, and ground lines
     *
     * @return Pointer to an Lpc3250RTD.  This is a transfer of ownership.
     */
    static Lpc3250RTD* buildRtdFromPartitionLine(const std::string& partitionLine, struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader);

    /**
     * Builds RTD from a partition that contains new-line termated strings that define an RTD.
     *
     * @param partition - The partion containing RTD definition lines
     *
     * @param hardwareIoMap - Hardware IO map that will receive the RTDs built from the partition
     *
     * @param rtdPowerLine - RTD power line used to turn RTD's power on and off
     *
     * @param analogReader - Analog reader used to read sense, ref, and ground values
     *
     * @return Pointer to an Lpc3250RTD.  This is a transfer of ownership.
     */
    static hardware_io_map_t* buildRtdsFromPartition(const std::string& partition, hardware_io_map_t& hardwareIoMap, struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader);

private:
    char* _name;
    char _senseChannelAddress;
    char _referenceChannelAddress;
    char _groundChannelAddress;
    Lpc3250AnalogReader &_analogReader;
    struct gpiod_line* _rtdPowerLine;
    uint32_t _powerOndelayMicroSec;

    Lpc3250RTD(const rtd_channel_t& rtdChannelDef, struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader);

    int readValue(double& result,char muxChannel, char adcChannel);

    double rtdResistance();
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250RTD_H_ */
