/** @file Lpc3250AnalogIO.cpp 
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
#include "Lpc3250AnalogInput.h"
#include "Lpc3250AnalogReader.h"
#include "Lpc3250GPIO.h"

#include <cstdlib>
#include <log4cIncludes.h>
#include <cstdlib>
#include <stdint.h>
#include <cstring>
#include <Exception.h>
#include <cassert>
#include <stdio.h>
#include <StringUtl.h>
#include <string.h>

namespace zios {
namespace lpc3250 {

using namespace log4cplus;
using namespace zios::hal;
using namespace zios::common;

//
// libiio is not thread safe so we need to keep a separate context for each thread.
//

Lpc3250AnalogInput::Lpc3250AnalogInput(const char* name,
                                       Lpc3250AnalogReader* analogReader,
                                       uint8_t muxChannelNumber,
                                       uint8_t adcChannelNumber,
                                       Lpc3250GPIO* loopEnableGpio) :
        IAnalogIO(IHardwareIO::AI_T),
        _name(NULL),
        _analogReader(analogReader),
        _muxChannelNumber(muxChannelNumber),
        _adcChannelNumber(adcChannelNumber),
        _loopEnableGpio(loopEnableGpio),
        _slope(1.0),
        _offset(0),
        _lastValue(0.0)
{
    assert(name != NULL);
    _name = ::strdup(name);
}

Lpc3250AnalogInput::~Lpc3250AnalogInput()
{
    ::free(_name);
}

void Lpc3250AnalogInput::setSlope(float slope)
{
    _slope = slope;
}

void Lpc3250AnalogInput::setOffset(float offset)
{
    _offset = offset;
}

IDigitalIO* Lpc3250AnalogInput::getReferenceFor4to20()
{
    return _loopEnableGpio;
}

float Lpc3250AnalogInput::scaledValueAsFloat()
{
    double vOut;
    if (0 != _analogReader->voltageFromAdcChannel(vOut, _adcChannelNumber, _muxChannelNumber))
        vOut = -1;

    //
    // If the loop enable line is set on, then we have to apply a calculation
    // to convert the voltage reading to a current value
    //
    if (_loopEnableGpio->getInt() == 1)
        vOut = (vOut * 1000.0) / LPC3250_LOOP_RESISTOR_VALUE;

    //
    // Apply any scaling
    //
    if (vOut > 0)
        vOut = vOut * _slope + _offset;

    return (float)vOut;
}

const char* Lpc3250AnalogInput::name() const
{
    return _name;
}

const char* Lpc3250AnalogInput::label() const
{
    return _name;
}

uint32_t Lpc3250AnalogInput::getInt()
{
    return 0;
}

float Lpc3250AnalogInput::getFloat()
{
    return _lastValue;
}

IHardwareIO::DATATYPE Lpc3250AnalogInput::getResponseType() const
{
    return IHardwareIO::FLOAT_T;
}

long_timestamp_t Lpc3250AnalogInput::timestamp() const
{
    return TIMESTAMP_TO_LONG_TIME_STAMP(::time(NULL));
}

bool Lpc3250AnalogInput::isReadOnly() const
{
    return true;
}

bool Lpc3250AnalogInput::writeBool(bool value)
{
    return false;
}

int32_t Lpc3250AnalogInput::convert()
{
    _lastValue = scaledValueAsFloat();
    return _lastValue;
}

Lpc3250AnalogInput* buildAnalogInputFromAnalogIoConfig(analog_io_config_t& analogIoConfig, Lpc3250AnalogReader& analogReader, const hardware_io_map_t& hardwareIoMap)
{
    hardware_io_map_t::const_iterator iter = hardwareIoMap.find(analogIoConfig.switchVtoCGpioName);
    if (iter == hardwareIoMap.end()) {
        LOG4CPLUS_WARN(Logger::getInstance("zios.lpc3250"), "SWS GPIO " << analogIoConfig.switchVtoCGpioName << " not found for AI " << analogIoConfig.mappingName);
        return NULL;
    }

    return new Lpc3250AnalogInput(analogIoConfig.mappingName,
                                  &analogReader,
                                  analogIoConfig.muxChannelNumber,
                                  analogIoConfig.adcChannelNumber,
                                  (Lpc3250GPIO*)iter->second);
}

Lpc3250AnalogInput* Lpc3250AnalogInput::buildAnalogInputFromPartitionLine(const std::string &partiationLine, Lpc3250AnalogReader& analogReader, const hardware_io_map_t& hardwareIoMap)
{
    analog_io_config_t analogIoConfig;
    map<string, string> key_value_map;
    map<string, string>::iterator kvIter;
    Lpc3250AnalogInput* ai = NULL;

    ::memset(&analogIoConfig, 0, sizeof(analog_io_config_t));

    string::size_type pos = partiationLine.find_first_of('=');
    string name = partiationLine.substr(0, pos);
    string params = partiationLine.substr(pos + 1);
    analogIoConfig.mappingName = name.c_str();

    for (uint32_t i = 0; i < params.length(); i++)
        if (params[i] == ';')
            params[i] = '\n';

    key_value_map.clear();
    StringUtl::stringToMap(params, key_value_map);
    if (key_value_map.size() == 0)
        LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Invalid key/value map: " << params);
    else
    {


        kvIter = key_value_map.find("muxchan");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(Logger::getInstance("zios.lpc3250"), "Key map for " << name << "is missing required key 'muxchan'");
            return NULL;
        }
        analogIoConfig.muxChannelNumber = (uint8_t)::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("adcchan");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(Logger::getInstance("zios.lpc3250"), "Key map for " << name << "is missing required key 'adcchan'");
            return NULL;
        }
        analogIoConfig.adcChannelNumber = (uint8_t)::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("swsgpio");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(Logger::getInstance("zios.lpc3250"), "Key map for " << name << "is missing required key 'swsgpio'");
            return NULL;
        }
        analogIoConfig.switchVtoCGpioName = kvIter->second.c_str();

        ai = buildAnalogInputFromAnalogIoConfig(analogIoConfig, analogReader, hardwareIoMap);
    }

    return ai;
}

hardware_io_map_t* Lpc3250AnalogInput::buildAnalogInputsFromPartition(const string &partition, Lpc3250AnalogReader& analogReader, hardware_io_map_t &hardwareIoMap)
{
    char buf[128];
    istringstream iss(partition);
    string str;
    Lpc3250AnalogInput* ai = NULL;

    while (iss.getline(buf, sizeof(buf))) {
        str.assign(buf);
        StringUtl::trim(str);
        if (str.length() > 0) {
            if ((ai = Lpc3250AnalogInput::buildAnalogInputFromPartitionLine(str, analogReader, hardwareIoMap)) != NULL)
                hardwareIoMap.insert(pair<string, IHardwareIO*>(ai->name(), ai));
            else
                return NULL;
        }
    }

    return &hardwareIoMap;
}


} /* namespace hal */
} /* namespace zios */
