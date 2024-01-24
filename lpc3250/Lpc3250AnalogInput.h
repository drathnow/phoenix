/** @file Lpc3250AnalogIO.h 
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
#ifndef LPC3250ANALOGINPUT_H_
#define LPC3250ANALOGINPUT_H_

#include <zhal.h>
#include <cstdint>

#include "lpc3250.h"

#define AI_ERR      -123.45

typedef struct {
    const char* mappingName;
    uint8_t muxChannelNumber;
    uint8_t adcChannelNumber;
    const char* switchVtoCGpioName;     // Voltage to current gpio
} analog_io_config_t ;

namespace zios {
namespace lpc3250 {

class Lpc3250AnalogReader;
class Lpc3250GPIO;

/**
 * The Lpc3250AnalogInput coordinates the reading of a value from the
 * ADC.  This class is thread safe but it's usage should be carefully
 * considered.  To use it, you must first call convert(), which causes
 * the actual read operation from the ADC.  However convert() does
 * not return the value to the caller.  Rather, it retreives the value from
 * the ADC and stores it in a instance variable. The value must be retrieved
 * by call int getFloat().
 *
 * The reason for this is that the read from the ADC can be done in a separate
 * thread that is dedicated to retrieving values from the IO board.  This allows
 * elimates the need for Mutexes to synchronize access to the ADC.
 *
 */
class Lpc3250AnalogInput: public zios::hal::IAnalogIO
{
public:

    /**
     * Constructs an Lpc3250AnalogInput
     *
     * @param name - The name of this analog input
     *
     * @param _analogReader - The analog reader object use to orchastrate the reading from the ADC.
     *
     * @param muxChannelNumber - Mux selector channel.  Valid values are 0-7
     *
     * @param adcChannelNumber - ADC Channel number.  Valid values are 0-3
     *
     * @param loopEnableGpio - GPIO line used to switch from reading current to voltage.
     */
    Lpc3250AnalogInput(const char* name,
                       Lpc3250AnalogReader* _analogReader,
                       uint8_t muxChannelNumber,
                       uint8_t adcChannelNumber,
                       Lpc3250GPIO* loopEnableGpio);
    ~Lpc3250AnalogInput();


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
     * This method is part of the IHardwareIO interface. It is not
     * implemented here and will always return 0.
     *
     * @return Always zero (0)
     */
    uint32_t getInt();

    /**
     * Get the value from the ADC, applying any conversion as defined by slope and offset
     * parameters.
     *
     * @return Value from the ADC. -123.45 if an error occurred during conversion.
     */
    float getFloat();

    /**
     * Part of the IHardwareIO interface.
     *
     * @return Always return FLOAT_T.
     */
    IHardwareIO::DATATYPE getResponseType() const;

    /**
     * Returns current longtimestamp for value in hardware port
     *
     * @return current time of value
     */
    long_timestamp_t timestamp() const;

    /**
     * Is this "port" readonly
     *
     * @return Always true - Analog inputs are always read only.
     */
    bool isReadOnly() const;

    /**
     * Part of the IHardwareIO interface.  Not implemented.
     *
     * @return Always false.
     */
    bool writeBool(bool value);

    /**
     * update internal scaling factor m in mx+b
     * @param[in] slope scaling factor
     */
    void setSlope(float slope);

    /**
     * update internal offset factor b in mx+b
     * @param offset offset factor
     */
    void setOffset(float offset);

    /**
     * Returns the current value as a float.  The value returned is depends on a couple of things:
     *
     * 1. If the associated 4 to 2 ma GPIO is enabled, then the resulting voltage is translated
     *    to a current value.  When enabled, the GPIO causes a 250 ohm resister to be placed
     *    in the circuit.
     *
     * 2. The value of the slope and offset.  These values are applied to the voltage (or current)
     *    calculated in 1.
     *
     * @return float value - Voltage or Current, scaled by the formula (mx + b)
     */
    float scaledValueAsFloat();

    /**
     * @return Pointer to an IDigitalIO. This is not a transfer of ownership. Callers should not hold
     * the refence beyond the scope of the point where the pointer is returned.
     */
    zios::hal::IDigitalIO* getReferenceFor4to20();

    /**
     * Trigger actual conversion, returning ADC units direct from ADC
     * @return 16 bit representation of % reference voltage
     */
    int32_t convert();

    static Lpc3250AnalogInput* buildAnalogInputFromPartitionLine(const std::string &partiationLine, Lpc3250AnalogReader& analogReader, const hardware_io_map_t& hardwareIoMap);

    static void buildAnalogIosWithAnalogReader(hardware_io_map_t& hardwareIos, Lpc3250AnalogReader* analogReader);

    static hardware_io_map_t* buildAnalogInputsFromPartition(const std::string &partition, Lpc3250AnalogReader& analogReader, hardware_io_map_t &hardwareIoMap);

private:
    char* _name;
    Lpc3250AnalogReader* _analogReader;
    uint8_t _muxChannelNumber;
    uint8_t _adcChannelNumber;
    Lpc3250GPIO* _loopEnableGpio;
    float _slope;
    float _offset;
    float _lastValue;
};

} /* namespace hal */
} /* namespace zios */

#endif /* LPC3250ANALOGINPUT_H_ */
