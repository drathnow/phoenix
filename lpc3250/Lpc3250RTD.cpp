/** @file Lpc3250RTD.cpp
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
#include "Lpc3250RTD.h"

#include <IPowerModule.h>
#include <cmath>
#include <cstring>
#include <memory.h>
#include <cassert>
#include <unistd.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <iostream>
#include <StringUtl.h>

#include "Lpc3250AnalogReader.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace zios::hal;
using namespace log4cplus;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250.Lpc3250RTD");

const double Lpc3250RTD::TERM1 = 3383.80952380952;  // -A/2B
const double Lpc3250RTD::TERM2 = 11450166.893424;   // A^2/4B^2
const double Lpc3250RTD::TERM3 = -1731601.73160173; // 1/B
const double Lpc3250RTD::GAIN = 10.09;
const double Lpc3250RTD::CORRECTION_FACTOR = 0.9963;
const double Lpc3250RTD::REFERENCE_RESISTANCE = 150.0;

Lpc3250RTD::Lpc3250RTD(const char *name,
        char senseChannelAddress,
        char referenceChannelAddress,
        char groundChannelAddress,
        Lpc3250AnalogReader &analogReader,
        struct gpiod_line* rtdPowerLine,
        uint32_t delayMicroSec) :
                _name(::strdup(name)),
                _senseChannelAddress(senseChannelAddress),
                _referenceChannelAddress(referenceChannelAddress),
                _groundChannelAddress(groundChannelAddress),
                _analogReader(analogReader),
                _rtdPowerLine(rtdPowerLine),
                _powerOndelayMicroSec(delayMicroSec)
{
    assert(name != NULL);
    assert(ADC_CHANNEL(_senseChannelAddress) == 2 || ADC_CHANNEL(_senseChannelAddress) == 3);
    assert(ADC_CHANNEL(_referenceChannelAddress) == 2 || ADC_CHANNEL(_referenceChannelAddress) == 3);
    assert(ADC_CHANNEL(_groundChannelAddress) == 2 || ADC_CHANNEL(_groundChannelAddress) == 3);
}

Lpc3250RTD::Lpc3250RTD(const rtd_channel_t &rtdChannelDef,  struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader) :
                _name(::strdup(rtdChannelDef.name)),
                _senseChannelAddress(rtdChannelDef.senseChannel),
                _referenceChannelAddress(rtdChannelDef.referenceChannel),
                _groundChannelAddress(rtdChannelDef.groundChannel),
                _analogReader(analogReader),
                _rtdPowerLine(rtdPowerLine),
                _powerOndelayMicroSec(rtdChannelDef.channelDelay)
{
    assert(_name != NULL);
}

Lpc3250RTD::~Lpc3250RTD()
{
    ::free(_name);
}

const char* Lpc3250RTD::name() const
{
    return _name;
}

const char* Lpc3250RTD::label() const
{
    return _name;
}

uint32_t Lpc3250RTD::getInt()
{
    return 0;
}

float Lpc3250RTD::getFloat()
{
    return temperatureForResistance(rtdResistance());
}

double Lpc3250RTD::convertTemperature()
{
    return temperatureForResistance(rtdResistance());
}

IHardwareIO::DATATYPE Lpc3250RTD::getResponseType() const
{
    return IHardwareIO::FLOAT_T;
}

long_timestamp_t Lpc3250RTD::timestamp() const
{
    return 0;
}

bool Lpc3250RTD::isReadOnly() const
{
    return true;
}

bool Lpc3250RTD::writeBool(bool value)
{
    return false;
}

double Lpc3250RTD::rtdResistance()
{
    double sense = 0.0, referrence = 0.0, ground = 0.0;

    if (::gpiod_line_set_value(_rtdPowerLine, 1) != 0) {
        LOG4CPLUS_ERROR(logger, "Unable to power on RTD: " << ::strerror(errno));
        return -1;
    }
    ::usleep(_powerOndelayMicroSec);

    readValue(sense, MUX_CHANNEL(_senseChannelAddress), ADC_CHANNEL(_senseChannelAddress));
    readValue(referrence, MUX_CHANNEL(_referenceChannelAddress), ADC_CHANNEL(_referenceChannelAddress));
    readValue(ground, MUX_CHANNEL(_groundChannelAddress), ADC_CHANNEL(_groundChannelAddress));

    LOG4CPLUS_DEBUG(logger, name() << ": Sense=" << sense << " Ref=" << referrence << " Grnd=" << ground);

    if (gpiod_line_set_value(_rtdPowerLine, 0) != 0)
        LOG4CPLUS_WARN(logger, "Unable to power OFF RTD: " << ::strerror(errno));

    sense /= GAIN;
    referrence /= GAIN;
    ground /= GAIN;

    if (referrence != 0 && CORRECTION_FACTOR != 0)
        return ((sense - ground) / (referrence - ground)) * REFERENCE_RESISTANCE / CORRECTION_FACTOR;

    LOG4CPLUS_ERROR(logger, "Divide by 0 lastRefSample: " << referrence <<" factor: " << CORRECTION_FACTOR);
    return -1;
}

int Lpc3250RTD::readValue(double &result, char muxChannel, char adcChannel)
{
    //
    // We kind of ignore that ADC channel argument as the ADC only takes values from
    // the LPF side of the circuit.  If you do pass an invalid one, we retrun an error.
    //
    switch (adcChannel)
    {
        case 2:
        case 3:
            return _analogReader.voltageFromAdcChannel(result, adcChannel, muxChannel);
    }

    assert(false); // Yeah, this shouldn't happen!
    return -1;
}

double Lpc3250RTD::temperatureForResistance(float resistance)
{
    if (resistance >= 0)
    {
        // Temperature = -Ro *A + sqrt( ro^2 * A^2 -4Ro * B(Ro -Rt) / (2R0B)
        // where A = 0.0039083
        // B = -5.775 E-7
        // and Ro = 100 ohm
        // so T = -A/2B - sqroot(A^2/4*B^2 -1/B(1-Rt/100)
        // guarenteed that non zero of denominator because B is != 0
        // but need to check sign under sqroot to ensure not taking sqroot of -ve number
        //
        double sqrootTerm = (double) (TERM2 - TERM3 * (1.0 - resistance / 100.0));
        if (sqrootTerm >= 0)
            return (TERM1 - pow(sqrootTerm, 0.5));
        else
            LOG4CPLUS_ERROR(logger, "Unable to calculate temperature for resistance: SqrootTerm < 0 for resistance = " << resistance);
    }
    else
        LOG4CPLUS_ERROR(logger, "Unable to calculate temperature for resistance: Resistance < 0");

    return 9999;
}

Lpc3250RTD* Lpc3250RTD::buildRtdFromPartitionLine(const std::string &partitionLine, struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader)
{
    rtd_channel_t rtdConfig;
    map<string, string> key_value_map;
    map<string, string>::iterator kvIter;
    Lpc3250RTD *rtd = NULL;

    ::memset(&rtdConfig, 0, sizeof(rtd_channel_t));

    string::size_type pos = partitionLine.find_first_of('=');
    string name = partitionLine.substr(0, pos);
    string params = partitionLine.substr(pos + 1);
    rtdConfig.name = name.c_str();

    for (uint32_t i = 0; i < params.length(); i++)
        if (params[i] == ';')
            params[i] = '\n';

    key_value_map.clear();
    StringUtl::stringToMap(params, key_value_map);
    if (key_value_map.size() == 0)
        LOG4CPLUS_ERROR(Logger::getInstance("zios.lpc3250"), "Invalid key/value map: " << params);
    else
    {

        kvIter = key_value_map.find("senschan");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'senschan'");
            return NULL;
        }
        rtdConfig.senseChannel = ::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("refchan");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'refchan'");
            return NULL;
        }
        rtdConfig.referenceChannel = ::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("gndchan");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'gndchan'");
            return NULL;
        }
        rtdConfig.groundChannel = ::atoi(kvIter->second.c_str());

        kvIter = key_value_map.find("delay");
        if (kvIter == key_value_map.end()) {
            LOG4CPLUS_WARN(logger, "Key map for " << name << " is missing required key 'delay'");
            return NULL;
        }
        rtdConfig.channelDelay = ::atoi(kvIter->second.c_str());
        rtd = new Lpc3250RTD(rtdConfig, rtdPowerLine, analogReader);
    }

    return rtd;
}

hardware_io_map_t* Lpc3250RTD::buildRtdsFromPartition(const std::string &partition, hardware_io_map_t &hardwareIoMap, struct gpiod_line* rtdPowerLine, Lpc3250AnalogReader &analogReader)
{
    char buf[128];
    istringstream iss(partition);
    string str;
    Lpc3250RTD *rtd = NULL;

    while (iss.getline(buf, sizeof(buf)))
    {
        str.assign(buf);
        if (str.length() > 0)
        {
            if ((rtd = buildRtdFromPartitionLine(str, rtdPowerLine, analogReader)) != NULL)
                hardwareIoMap.insert(pair<string, IHardwareIO*>(rtd->name(), rtd));
            else
                return NULL;
        }
    }

    return &hardwareIoMap;
}

} /* namespace lpc3250 */
} /* namespace zios */
