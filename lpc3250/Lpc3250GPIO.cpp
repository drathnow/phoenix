/** @file DigitalIO.cpp 
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

#include "Lpc3250GPIO.h"

#include <log4cIncludes.h>
#include <zhal.h>
#include <Exception.h>
#include <stdlib.h>
#include <cstring>
#include <memory.h>
#include <time.h>
#include <iostream>
#include <StringUtl.h>

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::hal;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250.Lpc3250DigitalIO");

Lpc3250GPIO::Lpc3250GPIO(const char *name, gpiod_line *line, PORTTYPE portType, Lpc3250GPIO *swScanGpio) :
        IDigitalIO(portType),
                _name(NULL),
                _digitalLine(line),
                _swScanGpio(swScanGpio)
{
    _name = ::strdup(name);
}

Lpc3250GPIO::~Lpc3250GPIO()
{
    ::free(_name);
}

const char* Lpc3250GPIO::name() const
{
    return _name;
}

const char* Lpc3250GPIO::label() const
{
    return _name;
}

uint32_t Lpc3250GPIO::getInt()
{
    return ::gpiod_line_get_value(_digitalLine);
}

float Lpc3250GPIO::getFloat()
{
    return (float) getInt();
}

IHardwareIO::DATATYPE Lpc3250GPIO::getResponseType() const
{
    return IHardwareIO::BOOL_T;
}

long_timestamp_t Lpc3250GPIO::timestamp() const
{
    return ::time(NULL);
}

bool Lpc3250GPIO::isReadOnly() const
{
    return type() == DI_T;
}

bool Lpc3250GPIO::writeBool(bool value)
{
    assert(false == isReadOnly());
    return 0 == ::gpiod_line_set_value(_digitalLine, value ? 1 : 0);
}

IDigitalIO* Lpc3250GPIO::getReferenceForScan()
{
    return _swScanGpio;
}

void Lpc3250GPIO::forceUpdate()
{
}

void Lpc3250GPIO::forceRead()
{
}

Lpc3250GPIO* Lpc3250GPIO::buildGpioFromPartitionLine(const std::string &partiationLine, const chip_map_t &chipMap, const hardware_io_map_t& hardwareIoMap)
{
    gpio_config_t gpio_config;
    map<string, string> key_value_map;
    map<string, string>::iterator kvIter;
    Lpc3250GPIO* gpio = NULL;

    ::memset(&gpio_config, 0, sizeof(gpio_config_t));

    string::size_type pos = partiationLine.find_first_of('=');
    string name = partiationLine.substr(0, pos);
    string params = partiationLine.substr(pos + 1);
    gpio_config.mappingName = name.c_str();

    for (uint32_t i = 0; i < params.length(); i++)
        if (params[i] == ';')
            params[i] = '\n';

    key_value_map.clear();
    StringUtl::stringToMap(params, key_value_map);
    if (key_value_map.size() == 0)
        LOG4CPLUS_ERROR(logger, "Invalid key/value map '" << params << "' for " << name);
    else
    {
        kvIter = key_value_map.find("chip");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'chip'");
            return NULL;
        }
        gpio_config.chipLabel = kvIter->second.c_str();

        kvIter = key_value_map.find("line");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'line'");
            return NULL;
        }
        gpio_config.lineNumber = ::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("ro");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'ro'");
            return NULL;
        }
        gpio_config.isReadOnly = ::atoi(kvIter->second.c_str()) == 0 ? false : true;

        kvIter = key_value_map.find("inv");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'inv'");
            return NULL;
        }
        gpio_config.isInverted = ::atoi(kvIter->second.c_str()) == 0 ? false : true;

        kvIter = key_value_map.find("sws");
        if (kvIter == key_value_map.end())
            gpio_config.swScanGpioName = NULL;
        else
            gpio_config.swScanGpioName = kvIter->second.c_str();

        gpio = buildGpioFromGpioInfo(gpio_config, chipMap, hardwareIoMap);
    }

    return gpio;
}

hardware_io_map_t* Lpc3250GPIO::buildGpiosFromPartition(const string &partition, const chip_map_t &chipMap, hardware_io_map_t &hardwareIoMap)
{
    char buf[128];
    istringstream iss(partition);
    string str;
    Lpc3250GPIO* gpio = NULL;

    while (iss.getline(buf, sizeof(buf))) {
        str.assign(buf);
        StringUtl::trim(str);
        if (str.length() > 0) {
            if ((gpio = Lpc3250GPIO::buildGpioFromPartitionLine(str, chipMap, hardwareIoMap)) != NULL)
                hardwareIoMap.insert(pair<string, IHardwareIO*>(gpio->name(), gpio));
            else
                return NULL;
        }
    }

    return &hardwareIoMap;
}

Lpc3250GPIO* Lpc3250GPIO::buildGpioFromGpioInfo(gpio_config_t &gpio, const chip_map_t &chipMap, const hardware_io_map_t& retHardwareIOs)
{
    Lpc3250GPIO *result = NULL;
    gpiod_line *line = NULL;

    chip_map_t::const_iterator chipIter = chipMap.find(gpio.chipLabel);
    if (chipIter == chipMap.end()) {
        LOG4CPLUS_ERROR(logger, "Unable to lookup chip " << gpio.chipLabel << " : " << ::strerror(errno));
        return NULL;
    }

    line = ::gpiod_chip_get_line(chipIter->second, gpio.lineNumber);
    if (line == NULL) {
        LOG4CPLUS_ERROR(logger, "Unable to find line " << gpio.chipLabel << "/" << gpio.lineNumber << ": " << ::strerror(errno));
        return NULL;
    }

    //
    // If this GPIO is readonly, then we have to enable output and set it's initial value
    //
    int flags = gpio.isInverted ? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW : 0;
    if (gpio.isReadOnly) {
        if (-1 == ::gpiod_line_request_input_flags(line, "zios", flags)) {
            LOG4CPLUS_ERROR(logger, "Unable to set request line output " << gpio.chipLabel << "/" << gpio.lineNumber << ": " << ::strerror(errno));
            return NULL;
        }
    }
    else {
        if (-1 == ::gpiod_line_request_output_flags(line, "zios", flags, 0)) {
            LOG4CPLUS_ERROR(logger, "Unable to set request line output " << gpio.chipLabel << "/" << gpio.lineNumber << ": " << ::strerror(errno));
            return NULL;
        }
    }

    try {
        Lpc3250GPIO *swScan = NULL;

        if (gpio.swScanGpioName != NULL) {
            hardware_io_map_t::const_iterator iter = retHardwareIOs.find(gpio.swScanGpioName);
            if (iter != retHardwareIOs.end())
                swScan = (Lpc3250GPIO*) iter->second;
            else
                LOG4CPLUS_ERROR(logger, "Unable to find SW Scan gpio with name " << gpio.swScanGpioName);
        }

        result = new Lpc3250GPIO(gpio.mappingName,
                                 line,
                                 gpio.isReadOnly ? IHardwareIO::DI_T : IHardwareIO::DO_T,
                                 swScan);
    }
    catch (Exception &e)
    {
        LOG4CPLUS_ERROR(logger, "Unable to build GPOI: " << e.what());
    }

    return result;
}

}
}
