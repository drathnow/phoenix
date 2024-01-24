/** @file Lpc3250PowerModuleEx.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun. 17, 2019 by daver:
 * @endhistory
 */
#include "Lpc3250PowerModule.h"

#include <log4cIncludes.h>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <Exception.h>
#include <cassert>
#include <StringUtl.h>

#include "Lpc3250GPIO.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::hal;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250.Lpc3250PowerModule");

Lpc3250PowerModule::Lpc3250PowerModule(const char *name,
        Lpc3250GPIO &enableIo,
        bool isInternal,
        int enableDelayMilliseconds,
        std::vector<Lpc3250GPIO*> &selectorIos,
        std::vector<Lpc3250GPIO*> &voltageRangeIos,
        Lpc3250GPIO *faultIo,
        zios::common::MonotonicTime &monotonicTime) :
        _name(::strdup(name)),
                _enableIo(enableIo),
                _faultIo(faultIo),
                _enableDelayMs(enableDelayMilliseconds),
                _selectedVoltage(0),
                _internal(isInternal),
                _modeId(0),
                _faultId(0),
                _selectId(0),
                _currentPowerState(IPowerModule::POWER_OFF),
                _monotonicTime(monotonicTime),
                _powerWillBeOnTime(0),
                _selectCount(0),
                _currentSelection(0)
{
    _selectorIos.insert(_selectorIos.end(), selectorIos.begin(), selectorIos.end());
    _voltageRangesIos.insert(_voltageRangesIos.end(), voltageRangeIos.begin(), voltageRangeIos.end());
    _selectCount = 1 << _selectorIos.size();
}

Lpc3250PowerModule::~Lpc3250PowerModule()
{
    ::free(_name);
}

const char* Lpc3250PowerModule::name() const
{
    return _name;
}

const char* Lpc3250PowerModule::label() const
{
    return _name;
}

bool Lpc3250PowerModule::isFaulted() const
{
    if (_faultIo != NULL)
    {
        switch (_faultIo->getInt())
        {
            case 0:
                default:
                return true;

            case 1:
                return false;
        }
    }
    return false;
}

bool Lpc3250PowerModule::isInternalPowerSupply() const
{
    return _internal;
}

int32_t Lpc3250PowerModule::enableDelayMilliseconds() const
{
    return _enableDelayMs;
}

void Lpc3250PowerModule::setEnableDelayMilliseconds(int32_t delayMilliseconds)
{
    _enableDelayMs = delayMilliseconds;
}

size_t Lpc3250PowerModule::rangeCount() const
{
    return _voltageRangesIos.size();
}

uint32_t Lpc3250PowerModule::setSelectRange(uint32_t range)
{
    uint32_t rangeout = (range < 1) ? range : range - 1; // adjust 1 base to 0 base, clamping to 0

    if (range > 0)
    {
        // catch for No selections
        if (_voltageRangesIos.size() > 0)
        {
            if (range > _selectCount)
                rangeout = _selectCount - 1;

            size_t index = 0;

            while (index < _selectorIos.size())
            {
                if (false == _selectorIos[index]->writeBool(1 == (rangeout & 0x01)))
                    return 0;
                index++;
                rangeout = rangeout >> 1;
            }
        }
        _currentSelection = range;
    }
    return range;
}

uint32_t Lpc3250PowerModule::selectedRange() const
{
    return _currentSelection;
}

uint32_t Lpc3250PowerModule::currentRangeValue() const
{
    return 0;
}

void Lpc3250PowerModule::setModeId(uint32_t id)
{
    _modeId = id;
}

uint32_t Lpc3250PowerModule::modeId() const
{
    return _modeId;
}

void Lpc3250PowerModule::setFaultId(uint32_t faultId)
{
    _faultId = faultId;
}

uint32_t Lpc3250PowerModule::faultId() const
{
    return _faultId;
}

void Lpc3250PowerModule::setSelectId(uint32_t id)
{
    _selectId = id;
}

uint32_t Lpc3250PowerModule::selectId() const
{
    return _selectId;
}

IPowerModule::POWER_STATE Lpc3250PowerModule::turnOn()
{
    POWER_STATE currentState = powerState();
    switch (currentState)
    {
        case POWER_ON:
            case POWER_PENDING:
            return _currentPowerState;

        case POWER_OFF: {
            if (_enableIo.writeBool(true))
            {
                if (_enableDelayMs > 0)
                {
                    _powerWillBeOnTime = _monotonicTime.getTimeMsec() + _enableDelayMs;
                    _currentPowerState = IPowerModule::POWER_PENDING;
                }
                else
                {
                    _currentPowerState = IPowerModule::POWER_ON;
                }
            }
            else
                THROW_EXCEPTION("Chip setline failed "<< ::strerror(errno));
        }
    }
    return _currentPowerState;
}

IPowerModule::POWER_STATE Lpc3250PowerModule::turnOff()
{
    int status = _enableIo.writeBool(false);
    if (status == -1)
        THROW_EXCEPTION("Set line off failed: " << strerror(errno));
    _currentPowerState = POWER_OFF;
    return _currentPowerState;
}

bool Lpc3250PowerModule::isOn()
{
    return POWER_ON == powerState();
}

IPowerModule::POWER_STATE Lpc3250PowerModule::powerState()
{
    switch (_currentPowerState)
    {
        case POWER_PENDING: {
            if ((_monotonicTime.getTimeMsec() >= _powerWillBeOnTime))
                _currentPowerState = POWER_ON;
            return _currentPowerState;
        }

        default:
            case POWER_ON:
            case POWER_OFF:
            return _currentPowerState;
    }
}

static Lpc3250PowerModule* buildPowerModuleFromPowerModuleConfig(power_module_config_t &powerModuleConfig,
        const hardware_io_map_t &hardwareIos,
        const chip_map_t &chips)
{
    vector<Lpc3250GPIO*> voltageRanges;
    vector<Lpc3250GPIO*> selectorLines;
    hardware_io_map_t::const_iterator hwIter;
    Lpc3250GPIO *faultDio = NULL;
    Lpc3250GPIO *enableGpio = NULL;
    uint32_t tmpRange;

    chip_map_t::const_iterator iter = chips.find(powerModuleConfig.gpiochipName);
    if (iter == chips.end())
    {
        LOG4CPLUS_ERROR(logger, "Invalid chipname " << powerModuleConfig.gpiochipName.c_str());
        return NULL;
    }

    hwIter = hardwareIos.find(powerModuleConfig.enableDioName);
    if (hwIter == hardwareIos.end())
    {
        LOG4CPLUS_ERROR(logger, "Invalid selector pin name " << powerModuleConfig.enableDioName.c_str());
        return NULL;
    }

    if (hwIter->second->type() != IHardwareIO::DO_T)
    {
        LOG4CPLUS_ERROR(logger, "Enable IO is not a GPO");
        return NULL;
    }
    enableGpio = (Lpc3250GPIO*) hwIter->second;

    if (powerModuleConfig.faultDioName.empty() == false)
    {
        hwIter = hardwareIos.find(powerModuleConfig.faultDioName);
        if (hwIter != hardwareIos.end())
        {
            if (hwIter->second->type() != IHardwareIO::DI_T)
            {
                LOG4CPLUS_ERROR(logger, "Fault IO is not a Digital Input");
                return NULL;
            }
            faultDio = (Lpc3250GPIO*) hwIter->second;
        }
    }

    if (powerModuleConfig.voltageRanges.empty() == false)
    {
        //
        // If you have voltage ranges, then you MUST have select pins for
        // those ranges.
        //
        assert(powerModuleConfig.selectPinNumbers.empty() == false);

        char buf[32];
        char *ctx = NULL;
        char *next = NULL;
        char *ptr = &buf[0];
        int j = 1;

        ::strcpy(buf, powerModuleConfig.selectPinNumbers.c_str());

        while (NULL != (next = ::strtok_r(ptr, ",", &ctx)))
        {
            ptr = NULL;
            if (StringUtl::stringToUint(string(next), tmpRange))
            {
                ostringstream oss;
                oss << "SelectorPin-" << j++;
                gpiod_line *line = ::gpiod_chip_get_line(iter->second, tmpRange);
                if (-1 == ::gpiod_line_request_output(line, "zios", 0))
                {
                    LOG4CPLUS_ERROR(logger, "Unable to enable output on selector pin: " << ::strerror(errno));
                    continue;
                }
                Lpc3250GPIO *dio = new Lpc3250GPIO(oss.str().c_str(), line, IHardwareIO::DO_T);
                voltageRanges.push_back(dio);
                selectorLines.push_back(dio);
            }
        }
    }

    return new Lpc3250PowerModule(powerModuleConfig.name.c_str(),
            *enableGpio,
            powerModuleConfig.isInternal,
            powerModuleConfig.enableDelayMilliseconds,
            selectorLines,
            voltageRanges,
            faultDio);
}

Lpc3250PowerModule* Lpc3250PowerModule::buildPowerModuleFromPartitionLine(const std::string &partitionLine, const chip_map_t &chips, const hardware_io_map_t &hardwareIoMap)
{
    map<string, string> key_value_map;
    map<string, string>::iterator kvIter;
    Lpc3250PowerModule *pm = NULL;

    string::size_type pos = partitionLine.find_first_of('=');
    string name = partitionLine.substr(0, pos);
    string params = partitionLine.substr(pos + 1);

    for (uint32_t i = 0; i < params.length(); i++)
        if (params[i] == ';')
            params[i] = '\n';

    key_value_map.clear();
    StringUtl::stringToMap(params, key_value_map);
    if (key_value_map.size() == 0)
        LOG4CPLUS_ERROR(logger, "Invalid key/value map '" << params << "' for " << name);
    else
    {
        power_module_config_t powerModuleConfig;

        powerModuleConfig.name = name;

        kvIter = key_value_map.find("chip");
        if (kvIter == key_value_map.end())
        {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'chip'");
            return NULL;
        }
        powerModuleConfig.gpiochipName = kvIter->second;

        kvIter = key_value_map.find("enable");
        if (kvIter == key_value_map.end())
        {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'enable'");
            return NULL;
        }
        powerModuleConfig.enableDioName = kvIter->second;

        kvIter = key_value_map.find("delay");
        if (kvIter == key_value_map.end())
        {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'delay'");
            return NULL;
        }
        powerModuleConfig.enableDelayMilliseconds = ::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("fault");
        if (kvIter != key_value_map.end())
            powerModuleConfig.faultDioName = kvIter->second;

        kvIter = key_value_map.find("range");
        if (kvIter != key_value_map.end())
            powerModuleConfig.voltageRanges = kvIter->second;

        kvIter = key_value_map.find("select");
        if (kvIter != key_value_map.end())
            powerModuleConfig.selectPinNumbers = kvIter->second;

        pm = buildPowerModuleFromPowerModuleConfig(powerModuleConfig, hardwareIoMap, chips);
    }

    return pm;
}

power_module_map_t* Lpc3250PowerModule::buildPowerModulesFromPartition(power_module_map_t &powerModules, const string &partition, const chip_map_t &chipMap, const hardware_io_map_t &hardwareIoMap)
{
    char buf[128];
    istringstream iss(partition);
    string str;
    Lpc3250PowerModule *pm = NULL;

    while (iss.getline(buf, sizeof(buf)))
    {
        str.assign(buf);
        if (str.length() > 0)
        {
            if ((pm = buildPowerModuleFromPartitionLine(str, chipMap, hardwareIoMap)) != NULL)
                powerModules.insert(pair<string, IPowerModule*>(pm->name(), pm));
            else
                return NULL;
        }
    }

    return &powerModules;
}

} /* namespace lpc3250 */
} /* namespace zios */
