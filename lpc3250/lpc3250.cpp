/** @file lpc3250.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jun. 12, 2019 by daver:
 * @endhistory
 */
#include "lpc3250.h"

#include <map>
#include <cassert>
#include <cstring>
#include <string.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <log4cIncludes.h>
#include <StringUtl.h>
#include <Exception.h>

#include "Lpc3250AnalogInput.h"
#include "Lpc3250GPIO.h"
#include "Lpc3250PowerModule.h"
#include "Lpc3250ADC8344.h"
#include "Lpc3250AnalogInputMux.h"
#include "Lpc3250AnalogReader.h"
#include "Lpc3250Tranceiver.h"
#include "Lpc3250EEProm.h"
#include "Lpc3250HardwareManager.h"
#include "Lpc3250RTD.h"
#include "Lpc3250VoltageReader.h"

namespace zios {
namespace lpc3250 {

using namespace std;
using namespace log4cplus;
using namespace zios::common;

static const Logger logger = Logger::getInstance("zios.lpc3250");

static system_info_t* build_system_info_from_partition(const string &partition)
{
    map<string, string> theMap;
    map<string, string>::iterator iter;
    system_info_t *system_info;

    system_info = new system_info_t;
    ::memset(system_info, 0, sizeof(system_info_t));

    StringUtl::stringToMap(partition, theMap, '=', true);

    //
    // We can live without this
    //
    iter = theMap.find("brd.hwrev");
    if (iter == theMap.end())
        LOG4CPLUS_WARN(logger, "brd.hwrev not found in eeprom");
    else
        system_info->main_board_info.hwrev = ::strdup(iter->second.c_str());

    //
    // We can live without this
    //
    iter = theMap.find("brd.pcbserial");
    if (iter == theMap.end())
        LOG4CPLUS_WARN(logger, "brd.pcbserial not found in eeprom");
    else
        system_info->main_board_info.pcb_serial = ::strdup(iter->second.c_str());

    //
    // We can't live without this
    //
    iter = theMap.find("brd.serial");
    if (iter == theMap.end()) {
        LOG4CPLUS_FATAL(logger, "brd.serial not found in eeprom");
        delete system_info;
        return NULL;
    }
    system_info->main_board_info.serial_number = ::strdup(iter->second.c_str());

    //
    // We can't live without this
    //
    iter = theMap.find("brd.key");
    if (iter == theMap.end()) {
        LOG4CPLUS_FATAL(logger, "brd.key not found in eeprom");
        delete system_info;
        return NULL;
    }
    system_info->main_board_info.key = ::strdup(iter->second.c_str());

    return system_info;
}

static ioboard_info_t* build_ioboard_info_from_partition(const string &partition)
{
    map<string, string> theMap;
    map<string, string>::iterator iter;
    ioboard_info_t *ioboard_info;

    StringUtl::stringToMap(partition, theMap);

    //
    // We can't live without this
    //
    iter = theMap.find("brd.type");
    if (iter == theMap.end()) {
        LOG4CPLUS_FATAL(logger, "brd.type not found in eeprom");
        return NULL;
    }

    ioboard_info = new ioboard_info_t;
    ::memset(ioboard_info, 0, sizeof(ioboard_info_t));
    ioboard_info->type = ::strdup(iter->second.c_str());

    //
    // We can live without this
    //
    iter = theMap.find("brd.hwrev");
    if (iter == theMap.end())
        LOG4CPLUS_WARN(logger, "brd.hwrev not found in eeprom");
    else
        ioboard_info->hwrev = ::strdup(iter->second.c_str());

    //
    // We can live without this
    //
    iter = theMap.find("brd.pcbserial");
    if (iter == theMap.end())
        LOG4CPLUS_WARN(logger, "brd.pcbserial not found in eeprom");
    else
        ioboard_info->pcb_serial = ::strdup(iter->second.c_str());

    return ioboard_info;
}

static Lpc3250AnalogInputMux* buildAnalogInputMux(gpiod_chip *chip)
{
    gpiod_line *line1 = ::gpiod_chip_find_line(chip, LPC3250_MUX_SELECT_LINE1_NAME);
    if (line1 == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to find line selector line " << LPC3250_MUX_SELECT_LINE1_NAME);
        return NULL;
    }

    gpiod_line *line2 = ::gpiod_chip_find_line(chip, LPC3250_MUX_SELECT_LINE2_NAME);
    if (line2 == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to find line selector line " << LPC3250_MUX_SELECT_LINE2_NAME);
        return NULL;
    }

    gpiod_line *line3 = ::gpiod_chip_find_line(chip, LPC3250_MUX_SELECT_LINE3_NAME);
    if (line3 == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to find line selector line " << LPC3250_MUX_SELECT_LINE3_NAME);
        return NULL;
    }

    return new Lpc3250AnalogInputMux(line1, line2, line3);
}

void lpc3250_free_system_info(system_info_t *system_info)
{
    if (system_info != NULL)
    {
        if (system_info->main_board_info.hwrev != NULL)
            ::free((char*)system_info->main_board_info.hwrev);

        if (system_info->main_board_info.key != NULL)
            ::free((char*)system_info->main_board_info.key);

        if (system_info->main_board_info.pcb_serial != NULL)
            ::free((char*)system_info->main_board_info.pcb_serial);

        if (system_info->main_board_info.serial_number != NULL)
            ::free((char*)system_info->main_board_info.serial_number);

        if (system_info->ioboard_info != NULL)
        {
            if (system_info->ioboard_info->hwrev != NULL)
                ::free((char*)system_info->ioboard_info->hwrev);

            if (system_info->ioboard_info->pcb_serial != NULL)
                ::free((char*)system_info->ioboard_info->pcb_serial);

            if (system_info->ioboard_info->type != NULL)
                ::free((char*)system_info->ioboard_info->type);

            delete system_info->ioboard_info;
        }
    }

    delete system_info;
}

void lpc3250_free_chip_map(chip_map_t &chipMap)
{
    for (chip_map_t::iterator iter = chipMap.begin(); iter != chipMap.end(); iter++)
        ::gpiod_chip_close(iter->second);
    chipMap.clear();
}

int lpc3250_build_chip_map(chip_map_t &returnMap)
{
    gpiod_chip *chip = NULL;
    struct gpiod_chip_iter *chipIter = NULL;
    const char *label = NULL;

    chipIter = ::gpiod_chip_iter_new();
    gpiod_foreach_chip_noclose(chipIter, chip)
    {
        label = ::gpiod_chip_label(chip);
        returnMap.insert(pair<string, gpiod_chip*>(label, chip));
    }

    gpiod_chip_iter_free_noclose(chipIter);

    return returnMap.size();
}

Lpc3250RS232Transceiver* lpc3250_build_rs232_transceiver(chip_map_t &chipMap)
{
    chip_map_t::const_iterator iter = chipMap.find(LPC3250_GPO_P3_LABEL);
    if (iter == chipMap.end())
            {
        LOG4CPLUS_ERROR(logger, "Unable to build RS232 Transceiver. Transceiver chip '" << LPC3250_GPO_P3_LABEL << "'not found");
        return NULL;
    }

    struct gpiod_line *forceOnLine = ::gpiod_chip_find_line(iter->second, LPC3250_GPIO_RS232_FORCEON_LINE);
    if (forceOnLine == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to build RS232 Transceiver. FORCEON line not found" << ::strerror(errno));
        return NULL;
    }

    struct gpiod_line *forceOffNLine = ::gpiod_chip_find_line(iter->second, LPC3250_GPIO_RS232_FORCEOF0F_N_LINE);
    if (forceOnLine == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to build RS232 Transceiver. FORCEOFF_N line not found" << ::strerror(errno));
        return NULL;
    }

    return new Lpc3250RS232Transceiver(forceOnLine, forceOffNLine);
}

Lpc3250RS485Transceiver* lpc3250_build_rs485_transceiver(chip_map_t &chipMap)
{
    chip_map_t::const_iterator iter = chipMap.find(LPC3250_GPO_P3_LABEL);
    if (iter == chipMap.end())
            {
        LOG4CPLUS_ERROR(logger, "Unable to build RS485 Transceiver. Transceiver chip '" << LPC3250_GPO_P3_LABEL << "'not found");
        return NULL;
    }

    struct gpiod_line *trxLine = ::gpiod_chip_find_line(iter->second, LPC3250_GPIO_RS485_TRX_LINE);
    if (trxLine == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to build RS485 Transceiver. TRX line not found" << ::strerror(errno));
        return NULL;
    }

    struct gpiod_line *rcvNLine = ::gpiod_chip_find_line(iter->second, LPC3250_GPIO_RS485_RCV_LINE_N);
    if (rcvNLine == NULL)
    {
        LOG4CPLUS_ERROR(logger, "Unable to build RS485 Transceiver. RCV_N line not found" << ::strerror(errno));
        return NULL;
    }

    return new Lpc3250RS485Transceiver(trxLine, rcvNLine);
}

Lpc3250HardwareManager* lpc3250_init_board(NotificationCenter& notificationCenter)
{
    vector<string> partitions;
    map<string, string> theMap;
    Lpc3250EEProm eeprom;
    lpc3250_model model = FG400;
    system_info_t *system_info;
    chip_map_t chipMap;
    hardware_io_map_t hardwareIoMap;
    power_module_map_t powerModules;
    Lpc3250VoltageReader* voltageReader = NULL;
    Lpc3250AnalogReader* analogReader = NULL;
    struct iio_context* globalIioContext = ::iio_create_default_context();

    voltageReader = new Lpc3250VoltageReader(::iio_context_clone(globalIioContext));

    lpc3250_build_chip_map(chipMap);

    eeprom.readMainBoardEEprom(partitions);
    if (partitions.size() == 0) {
        LOG4CPLUS_FATAL(logger, "Main board eeprom contains too few partitions. Expected 1 but was 0");
        return NULL;
    }

    if (partitions.size() > 1 && partitions.size() < 4)
    {
        LOG4CPLUS_FATAL(logger, "Main board eeprom contains too few partitions. Expected 4 but was " << partitions.size());
        return NULL;
    }

    if ((system_info = build_system_info_from_partition(partitions[0])) == NULL)
        return NULL;

    if (Lpc3250GPIO::buildGpiosFromPartition(partitions[2], chipMap, hardwareIoMap) == NULL)
        return NULL;

    if (Lpc3250PowerModule::buildPowerModulesFromPartition(powerModules, partitions[3], chipMap, hardwareIoMap) == NULL)
        return NULL;

    //
    // Check for the IO board.  If it's there, then it must contain 2 partitions
    //
    partitions.clear();
    eeprom.readIoBoardEEprom(partitions);
    if (partitions.size() > 0)
    {
        struct gpiod_line* powerLine = NULL;

        if (partitions.size() > 0 && partitions.size() < 2) {
            LOG4CPLUS_FATAL(logger, "IO board eeprom contains too few partitions. Expected 2 but was " << partitions.size());
            return NULL;
        }

        if ((system_info->ioboard_info = build_ioboard_info_from_partition(partitions[0])) == NULL)
            return NULL;

        if (Lpc3250GPIO::buildGpiosFromPartition(partitions[1], chipMap, hardwareIoMap) == NULL)
            return NULL;

        if (Lpc3250PowerModule::buildPowerModulesFromPartition(powerModules, partitions[2], chipMap, hardwareIoMap) == NULL)
            return NULL;

        chip_map_t::const_iterator chiter = chipMap.find(LPC3250_GPO_P3_LABEL);
        if (chiter == chipMap.end()) {
            LOG4CPLUS_ERROR(logger, "Unable to find " << LPC3250_GPO_P3_LABEL);
            return NULL;
        }

        Lpc3250ADC8344 *adc8344 = new Lpc3250ADC8344(::iio_context_clone(globalIioContext));
        Lpc3250AnalogInputMux *inputMux = buildAnalogInputMux(chiter->second);

        powerLine = ::gpiod_line_find(LPC3250_ANA_POWER_LINE_NAME);
        if (powerLine == NULL)
        {
            LOG4CPLUS_ERROR(logger, "ADC power line not found!");
            return NULL;
        }

        if (::gpiod_line_request_output(powerLine, "zios", 0) != 0)
        {
            LOG4CPLUS_ERROR(logger, "Unable to request output for ADC power line: " << ::strerror(errno));
            return NULL;
        }

        analogReader = new Lpc3250AnalogReader(powerLine, *adc8344, *inputMux);

        if (Lpc3250AnalogInput::buildAnalogInputsFromPartition(partitions[3], *analogReader, hardwareIoMap) == NULL)
            return NULL;

        //
        // Now build RTDs
        //
        powerLine = ::gpiod_line_find(LPC3250_RTD_POWER_LINE_NAME);
        if (powerLine == NULL)
        {
            LOG4CPLUS_ERROR(logger, "RTD power line not found!");
            return NULL;
        }

        if (::gpiod_line_request_output(powerLine, "zios", 0) != 0)
        {
            LOG4CPLUS_ERROR(logger, "Unable to request output for ADC power line: " << ::strerror(errno));
            return NULL;
        }

        if (Lpc3250RTD::buildRtdsFromPartition(partitions[4], hardwareIoMap, powerLine, *analogReader) == NULL)
            return NULL;

        //
        // Turn on SENS_MUX for DOs
        //
        hardware_io_map_t::iterator iter = hardwareIoMap.find("SENS_MUX.1");
        assert(iter != hardwareIoMap.end());
        iter->second->writeBool(true);

        model = FG600;
    }

    return new Lpc3250HardwareManager(model,
                                      system_info,
                                      chipMap,
                                      hardwareIoMap,
                                      powerModules,
                                      *voltageReader,
                                      notificationCenter,
                                      analogReader);
}

}
}

