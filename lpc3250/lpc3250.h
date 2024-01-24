/** @file lpc3250.h 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Mar. 29, 2019 by daver:
 * @endhistory
 */
#ifndef LPC3250_H_
#define LPC3250_H_

#include <map>
#include <string>
#include <gpiod.h>
#include <pthread.h>
#include <Mutex.h>
#include <NotificationCenter.h>

#define LPC325_CONSUMER_NAME                    "zios"
#define LPC3250_LOGGER_NAME                     "zios.lpc3250"

#define LPC3250_TEMP_FILE                       "/sys/class/hwmon/hwmon0/temp1_input" //!< Path to the system temperature file.

//
// The LPC3250 has a number of gpio chips. The exact number depends on whether the
// the IO board is present (FG600) or not (FG400).  These chips are found under
// /sys/bus/gpio/devices and called "gpiochipX" where X is a chip number. However,
// the numbers are not consistent between the FG400 and FG600, which can pose a problem
// when trying to write common code between these to devices.  For that reason,
// we chose to use the chip labels, which are consistent across both devices.  Below is
// a list of the chip names and labels:
//
//              FG400
//       Chip Name   Label
//       -------------------
//       gpiochip0   gpio_p0
//       gpiochip1   gpio_p1
//       gpiochip2   gpio_p2
//       gpiochip3   gpio_p3
//       gpiochip4   gpi_p3
//       gpiochip5   gpo_p3
//
//              FG600
//       Chip Name   Label
//       -------------------
//       gpiochip0   2-0021
//       gpiochip1   gpio_p0
//       gpiochip2   gpio_p1
//       gpiochip3   gpio_p2
//       gpiochip4   gpio_p3
//       gpiochip5   gpi_p3
//       gpiochip6   gpo_p3
//
//
#define LPC3250_PCA6416A_CHIP_LABEL             "2-0021"
#define LPC3250_GPO_P2_LABEL                    "gpo_p2"
#define LPC3250_GPO_P3_LABEL                    "gpo_p3"
#define LPC3250_GPIO_P2_LABEL                   "gpio_p2"

#define LPC3250_GPIO_RS232_FORCEON_LINE         "gpo15"     //!< gpo15 = RS323 FORCEON line (high)
#define LPC3250_GPIO_RS232_FORCEOF0F_N_LINE     "gpo14"     //!< gpo14 = FORCEOFF_N line (low)
#define LPC3250_GPIO_RS485_RCV_LINE_N           "gpo18"     //!< gpo18 = Receive enable (low) RS485_RE_N
#define LPC3250_GPIO_RS485_TRX_LINE             "gpo19"     //!< gpo19 = Transmit Enable (high) RS485_TE
#define LPC3250_LOOP_POWER_LINE_NAME            "p2.8"
#define LPC3250_LOOP_SELECT_LINE_NAME           "p2.9"
#define LPC3250_V5_POWER_LINE_NAME              "p2.10"
#define LPC3250_RTD_POWER_LINE_NAME             "p2.11"
#define LPC3250_ANA_POWER_LINE_NAME             "p2.12"

//
// These define the select lines used to select channels on the SN74LV4051APWR
// Multiplexer.
//
#define LPC3250_MUX_SELECT_LINE1_NAME           "gpo05"
#define LPC3250_MUX_SELECT_LINE2_NAME           "gpo06"
#define LPC3250_MUX_SELECT_LINE3_NAME           "gpo07"

//
// The Analog Circuit has an optional 250 ohm resistor that can be enabled
// in the cicuit to give current readings.
//
#define LPC3250_LOOP_RESISTOR_VALUE             250.0

//
// The ADC8344 has 8 channels but are only using 4. Channel 0 and 1 are used to read analog pins.
// Channel 0 is an unfiltered channel while channel 1 is filtered.
//
// Channels 2 and 3 are use to read RTDs, with channel 2 being unfiltered and channel 4 filtered.
//
#define LPC3250_UNF_ANALOG_CHANNEL_NUMBER       0
#define LPC3250_FILT_ANALOG_CHANNEL_NUMBER      1
#define LPC3250_UNF_RTD_CHANNEL_NUMBER          2
#define LPC3250_FILT_RTD_CHANNEL_NUMBER         3

#define LPC3250_POW_DETECT_EN_LINE              20          //!< Chip line that enables voltage reading
#define LPC3250_LED_POLL_LINE                   13
#define LPC3250_LED_SRV_LINE                    12
#define LPC3250_LED_POW_LINE                    1
#define LPC3250_WAKE_BUTTON_LINE                0
#define LPC3250_ADC8344_ENABLE_LINE             12
#define LPC3250_WAKE_BUTTON_LINE_NAME           "gpi00"     //!< Line 0 on gpio_p3 chip

#define LCP3250_LOOP_POWERMODULE_NAME           "POW.LOOP"
#define LCP3250_5V_POWERMODULE_NAME             "POW.5V"
#define LCP3250_RTD_POWERMODULE_NAME            "POW.RTD"
#define LCP3250_ADC_POWERMODULE_NAME            "POW.ANA"
#define LCP3250_LPC_POWERMODULE_NAME            "POW.LPC3250"

/**
 * These are keys we use for the LPC3250 eeprom
 */
#define LPC3250_MAINBOARD_SERIAL_NO_KEY         "brd.serial"
#define LPC3250_BOARD_PCB_SERIAL_NO_KEY         "brd.pcbserial"
#define LPC3250_MAINBOARD_KEY_KEY               "brd.key"
#define LPC3250_IOBOARD_TYPE_KEY                "brd.type"
#define LPC3250_BOARD_HW_VERSION_KEY            "brd.hwrev"


#ifndef _ONARM
#define LPC3250_MAC_ADDRESS_PATH                "./address"
#else
#define LPC3250_MAC_ADDRESS_PATH                "/sys/class/net/eth0/address"
#endif

#define LPC3250_RS232                           1
#define LPC3250_RS485                           2

#ifdef __cplusplus
extern "C" {
#endif

enum lpc3250_model {
    FG400, FG600
};

namespace zios {
namespace common {
    class Mutex;
}
namespace hal {
    class IPowerModule;
    class IHardwareIO;
}
namespace lpc3250 {

class Lpc3250Chip;
class Lpc3250AnalogReader;
class Lpc3250RS485Transceiver;
class Lpc3250RS232Transceiver;
class Lpc3250HardwareManager;


typedef std::map<std::string, zios::hal::IPowerModule*>     power_module_map_t;
typedef std::map<std::string, gpiod_chip*>                  chip_map_t;
typedef std::map<std::string, zios::hal::IHardwareIO*>      hardware_io_map_t;

typedef struct _main_board_info
{
    const char* hwrev;
    const char* serial_number;
    const char* pcb_serial;
    const char* key;
} main_board_info_t;

typedef struct
{
    const char* type;
    const char* hwrev;
    const char* pcb_serial;

} ioboard_info_t;

typedef struct _system_info
{
    main_board_info_t main_board_info;
    ioboard_info_t* ioboard_info;
} system_info_t;

Lpc3250HardwareManager* lpc3250_init_board(zios::common::NotificationCenter& notificationCenter);

/**
 * Builds a map of chip structures.  Each structure contains a gpiod_chip, a mutex, and mutex attributes.
 *
 * @param returnMap
 *
 * @return The number of chips returned in returnMap. 0 means no chips where found.  -1 if an error occurred
 * trying to open gpiod_chip.  If -1 is returned, returnMap may contain valid lpc325_chip objects. They should
 * be closed by calling lpc325_free_chip;
 */
int lpc3250_build_chip_map(chip_map_t& returnMap);

void lpc3250_free_chip_map(chip_map_t& chipMap);

Lpc3250RS232Transceiver* lpc3250_build_rs232_transceiver(chip_map_t& chipMap);

Lpc3250RS485Transceiver* lpc3250_build_rs485_transceiver(chip_map_t& chipMap);

void lpc3250_free_system_info(system_info_t *system_info);

}
}

}
#endif /* LPC3250_H_ */
