/** @file Lpc3250PowerModuleEx.h
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
#ifndef LPC3250POWERMODULE_H_
#define LPC3250POWERMODULE_H_

#include "lpc3250.h"

#include <zhal.h>
#include <MonotonicTime.h>
#include <string>

namespace zios {
namespace lpc3250 {

class Lpc3250Chip;
class Lpc3250GPIO;

typedef struct {
    std::string name;
    std::string gpiochipName;
    std::string enableDioName;
    std::string faultDioName;
    uint16_t enableDelayMilliseconds;
    std::string voltageRanges;
    std::string selectPinNumbers;
    bool isInternal;
} power_module_config_t ;

class Lpc3250PowerModule : public zios::hal::IPowerModule
{
public:
    Lpc3250PowerModule(const char* name,
                         Lpc3250GPIO& enableIo,
                         bool isInternal,
                         int enableDelayMilliseconds,
                         std::vector<Lpc3250GPIO*>& selectorIos,
                         std::vector<Lpc3250GPIO*>& voltageRangeIos,
                         Lpc3250GPIO* faultIo = NULL,
                         zios::common::MonotonicTime& monotonicTime = zios::common::MonotonicTime::globalSystemTime);
    ~Lpc3250PowerModule();

    const char* name() const;
    const char* label() const;
    bool isFaulted() const;
    bool isInternalPowerSupply() const;
    int32_t enableDelayMilliseconds() const;
    void setEnableDelayMilliseconds(int32_t delayMilliseconds);
    size_t rangeCount() const;
    const std::vector<uint32_t>& rangeList() const;
    uint32_t setSelectRange(uint32_t range);
    uint32_t selectedRange() const;
    uint32_t currentRangeValue() const;
    void setModeId(uint32_t id);
    uint32_t modeId() const;
    void setFaultId(uint32_t id);
    uint32_t faultId() const;
    void setSelectId(uint32_t id);
    uint32_t selectId() const;
    POWER_STATE turnOn();
    POWER_STATE turnOff();
    bool isOn();
    POWER_STATE powerState();


    static Lpc3250PowerModule* buildPowerModuleFromPartitionLine(const std::string& partitionLine, const chip_map_t& chips, const hardware_io_map_t& hardwareIoMap);

    static power_module_map_t* buildPowerModulesFromPartition(power_module_map_t& powerModules, const std::string &partition, const chip_map_t &chipMap, const hardware_io_map_t &hardwareIoMap);

private:
    char* _name;
    Lpc3250GPIO& _enableIo;
    Lpc3250GPIO* _faultIo;
    uint16_t _enableDelayMs;
    uint8_t _selectedVoltage;
    bool _internal;
    uint32_t _modeId;
    uint32_t _faultId;
    uint32_t _selectId;
    zios::hal::IPowerModule::POWER_STATE _currentPowerState;
    std::vector<Lpc3250GPIO*> _selectorIos;
    std::vector<Lpc3250GPIO*> _voltageRangesIos;
    zios::common::MonotonicTime& _monotonicTime;
    time_t _powerWillBeOnTime;
    uint8_t _selectCount;
    uint8_t _currentSelection;
};

} /* namespace lpc3250 */
} /* namespace zios */

#endif /* LPC3250POWERMODULE_H_ */
