/** @file IOBoardEEPromCreator.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct. 21, 2019 by daver:
 * @endhistory
 */
#include "EEPromCreator.h"

namespace zios {
namespace lpc3250 {

IOBoardEEPromCreator::IOBoardEEPromCreator()
{
}

IOBoardEEPromCreator::~IOBoardEEPromCreator()
{
}


void IOBoardEEPromCreator::createMainBoardEEprom(std::ostream& ostream)
{
    ostream << "some test stuff" << (char)0x0a;
    ostream << "eeprom.version=1.0" << (char)0x0a;
    ostream << "brd.hwrev=0.0" << (char)0x0a;
    ostream << "brd.serial=x86test" << (char)0x0a;
    ostream << "brd.pcbserial=simulator" << (char)0x0a;
    ostream << "brd.key=AAAAAGFzO2ZkbGtqcHdlb2luYjtlb2luYjtza2xqZGY7YXNnams7c2FkbGtnajtna2xq" << (char)0x0a;
    ostream << (char)0x04;
    ostream << "COM1=/dev/ttyS1,9600:8N1,232,0" << (char)0x0a;
    ostream << "COM2= /dev/ttyS2,14400:8O1,232,0" << (char)0x0a;
    ostream << "COM3= /dev/ttyS3,56700:7N1,232FC,0" << (char)0x0a;
}


void IOBoardEEPromCreator::createIOBordEEProm(std::ostream& os)
{
    os << "# File version 1.8" << (char)0x0a;
    os << "EEPROM.version=1" << (char)0x0a;
    os << "brd.type=IO-1" << (char)0x0a;
    os << "brd.hwrev=2.0" << (char)0x0a;
    os << "brd.pcbserial=IOBxxxxx" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Digital Ports" << (char)0x0a;
    os << "# These are driver objects meant to bridge abstract digital IO model" << (char)0x0a;
    os << "# to concrete implementations, typically hardware specific" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# Naming convention:" << (char)0x0a;
    os << "#   When refering to a specific digital value, rules are:" << (char)0x0a;
    os << "#     variable=<name of Digital Port>-<Bit#>[*]" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "#   The * indicates that the polarity of the signal is inverted and is optional." << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# LPC3250 implementation:" << (char)0x0a;
    os << "# DP.0 = P0 (0-7)" << (char)0x0a;
    os << "# DP.1 = P1 (16-23)" << (char)0x0a;
    os << "# DP.2 = P2 (0-12)" << (char)0x0a;
    os << "# DP.3 = GPI, GPO, GPIO pins, various assignments" << (char)0x0a;
    os << "# ZIBRD refers to the zedi_board kernel driver, which has IOCTL commands" << (char)0x0a;
    os << "# for the internal IO points, implemented as 32-bit registers.  This driver" << (char)0x0a;
    os << "# requires a port number to be supplied, mapped here as the address" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# DP.10= I2C port, (0-15), address=0x21" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# dev=device name, used by factory methods" << (char)0x0a;
    os << "# ad=address of an instance" << (char)0x0a;
    os << "# val=initial value for output" << (char)0x0a;
    os << "# dir=initial pin direction (0=in, 1=out)" << (char)0x0a;
    os << "# ZIBRD uses IOPort IOCTL on zedi_board driver" << (char)0x0a;
    os << "DP.0=dev=ZIBRD.IOPort;ad=0;val=0xFF;dir=0x00" << (char)0x0a;
    os << "DP.1=dev=ZIBRD.IOPort;ad=1;val=0x0;dir=0xFF0000" << (char)0x0a;
    os << "DP.2=dev=ZIBRD.IOPort;ad=2;val=0x0;dir=0x1F00" << (char)0x0a;
    os << "DP.3=dev=ZIBRD.IOPort;ad=3;val=0x0;dir=0x0" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# ADCMUX uses ADC MUX IOCTL on zedi_board driver" << (char)0x0a;
    os << "# ad=0 (ignored), dir is always 1's as this is a GPO (on all active pins) that means mux range 0-7" << (char)0x0a;
    os << "DP.4=dev=ZIBRD.ADCMUX;ad=0;val=0;dir=0x7" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# SENSMUX uses SENSOR MUX IOCTL on zedi_board driver" << (char)0x0a;
    os << "# ad=0 (ignored), dir is always 1's as this is a GPO (on all active pins) that means mux range 0-3" << (char)0x0a;
    os << "DP.5=dev=ZIBRD.SENSMUX;ad=0;val=0;dir=0x3" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# i2c 16 bit port extender" << (char)0x0a;
    os << "# dev = driver device used for this" << (char)0x0a;
    os << "# ad = address on i2c (0x21)" << (char)0x0a;
    os << "# val = initial value on lines at boot" << (char)0x0a;
    os << "# dir = initial direction on lines at boot (0 = input, 1 = output )" << (char)0x0a;
    os << "DP.10=dev=PCA6416;ad=33;val=00;dir=0xFFFF" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Digital Inputs" << (char)0x0a;
    os << "# label= board specific label on silkscreen" << (char)0x0a;
    os << "# dev = Digital Port driver with channel ( note * means invert)" << (char)0x0a;
    os << "# DI 1-8 are on AI/DI pins" << (char)0x0a;
    os << "DI.1=label=AIDI1;dev=DP.0-0*" << (char)0x0a;
    os << "DI.2=label=AIDI2;dev=DP.0-1*" << (char)0x0a;
    os << "DI.3=label=AIDI3;dev=DP.0-2*" << (char)0x0a;
    os << "DI.4=label=AIDI4;dev=DP.0-3*" << (char)0x0a;
    os << "DI.5=label=AIDI5;dev=DP.0-4*" << (char)0x0a;
    os << "DI.6=label=AIDI6;dev=DP.0-5*" << (char)0x0a;
    os << "DI.7=label=AIDI7;dev=DP.0-6*" << (char)0x0a;
    os << "DI.8=label=AIDI8;dev=DP.0-7*" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# DI 9-16 are on DIO pins" << (char)0x0a;
    os << "# Note DI 15-16 break pattern" << (char)0x0a;
    os << "DI.9=label=DIO9;dev=DP.2-0*" << (char)0x0a;
    os << "DI.10=label=DIO10;dev=DP.2-1*" << (char)0x0a;
    os << "DI.11=label=DIO11;dev=DP.2-2*" << (char)0x0a;
    os << "DI.12=label=DIO12;dev=DP.2-3*" << (char)0x0a;
    os << "DI.13=label=DIO13;dev=DP.2-4*" << (char)0x0a;
    os << "DI.14=label=DIO14;dev=DP.2-5*" << (char)0x0a;
    os << "DI.15=label=DIO15;dev=DP.3-2*" << (char)0x0a;
    os << "DI.16=label=DIO16;dev=DP.3-7*" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Digital Outputs" << (char)0x0a;
    os << "# label= board specific label on silkscreen" << (char)0x0a;
    os << "# dev = Digital Port driver" << (char)0x0a;
    os << "# ch  = channel (bit) on the DP" << (char)0x0a;
    os << "# type= OD --> open drain, high value turns on current sink" << (char)0x0a;
    os << "#       PP --> push-pull, non-inverting buffer" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# DO 9-16 are on DIO pins" << (char)0x0a;
    os << "# Setting value high turns ON current sink, output is pulled to GND" << (char)0x0a;
    os << "# Setting value low turns OFF current sink, output will float, typically to ext pull-up" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# type=" << (char)0x0a;
    os << "DO.9=label=DIO9;dev=DP.1-16;type=OD" << (char)0x0a;
    os << "DO.10=label=DIO10;dev=DP.1-17;type=OD" << (char)0x0a;
    os << "DO.11=label=DIO11;dev=DP.1-18;type=OD" << (char)0x0a;
    os << "DO.12=label=DIO12;dev=DP.1-19;type=OD" << (char)0x0a;
    os << "DO.13=label=DIO13;dev=DP.1-20;type=OD" << (char)0x0a;
    os << "DO.14=label=DIO14;dev=DP.1-21;type=OD" << (char)0x0a;
    os << "DO.15=label=DIO15;dev=DP.1-22;type=OD" << (char)0x0a;
    os << "DO.16=label=DIO16;dev=DP.1-23;type=OD" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Power Supplies" << (char)0x0a;
    os << "# \"POW\" objects model a software enableable power supply" << (char)0x0a;
    os << "# label=(optional) name of the supply, typically the external label visible to the user" << (char)0x0a;
    os << "# en=digital port output to turn on the supply (required)" << (char)0x0a;
    os << "# endelay = time in usec to allow for powersupply to stabilize before using ... eg: RTD / ANA power" << (char)0x0a;
    os << "# fault=digital port input to indicate if the supply has faulted (optional)" << (char)0x0a;
    os << "#       If a supply has faulted, it must be disabled and re-enabled to clear the fault" << (char)0x0a;
    os << "# faultdelay=min time (msec) that the supply must be disabled to clear a fault (optional, default=0)" << (char)0x0a;
    os << "# delay=min time (msec) for a supply to be stable after enabling (optional, default=0)" << (char)0x0a;
    os << "# selX=? 0 or more select lines to form an index of software selectable voltage ranges(optional)" << (char)0x0a;
    os << "# range=enumeration of valid voltages based on selX codes (1 or more) (optional)" << (char)0x0a;
    os << "# internal=(optional) if 1 then is not displayed to user or exposed, is for internal use only." << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# an example of a power supply with 3 valid values" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "#  sel1 sel0  Voltage" << (char)0x0a;
    os << "#  ------------------" << (char)0x0a;
    os << "#     0    0  5V" << (char)0x0a;
    os << "#     0    1  invalid" << (char)0x0a;
    os << "#     1    0  24V" << (char)0x0a;
    os << "#     1    1  12V" << (char)0x0a;
    os << "#  POW.LOOP.2=label=LOOP2;en=DP.2-8;fault=DP.2-6;sel0=P2.9;sel1=P2.10;range={5,,24,12}" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Dry contact current scan" << (char)0x0a;
    os << "# When enabled, provides a weak current drive to digital input pins, allowing direct" << (char)0x0a;
    os << "# connection to a on/off switch that is connected between the input and GND." << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "POW.SWSCAN=label=;en=DP.5-1;internal=1;range={5}" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Analog loop power supply" << (char)0x0a;
    os << "POW.LOOP=label=LOOP;en=DP.2-8;fault=DP.2-6;sel0=DP.2-9;range={24,12}" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# 5V Nominal power supply" << (char)0x0a;
    os << "POW.5V=label=5V;en=DP.2-10;fault=DP.2-7;range={5}" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# RTD power enable" << (char)0x0a;
    os << "# no fault or label" << (char)0x0a;
    os << "# internal=1 means internal function, may not be interesting to user" << (char)0x0a;
    os << "POW.RTD=label=;en=DP.2-11;endelay=2000;internal=1;" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Analog power enable" << (char)0x0a;
    os << "# no fault or label" << (char)0x0a;
    os << "POW.ANA=label=;en=DP.2-12;endelay=2000;internal=1" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# ADC" << (char)0x0a;
    os << "# ADC block models an ability to read an integer value from a converter using a" << (char)0x0a;
    os << "# logical channel.  The ADC can interpret the channel freely.  e.g. it could drive" << (char)0x0a;
    os << "# external MUXes as well as internal channel select to create large numbers of inputs." << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# driver = A/D driver module - contains things like max, min, settle time?" << (char)0x0a;
    os << "# scale = scaling factor used by A/D (Vadc = CODEadc/scale * ref) (e.g. 65536 for ad58344)" << (char)0x0a;
    os << "#        ADJ - scaling factor can be provided by ADC driver as it knows the physical properties of the device" << (char)0x0a;
    os << "# ref = reference voltage applied to A/D" << (char)0x0a;
    os << "# add = spi \"address\" of A/D" << (char)0x0a;
    os << "# muxdev=device to supply additional mux select lines" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# In this file, the channel numbers are assumed to be 6 bits:" << (char)0x0a;
    os << "# bits: 5-3 --> internal MUX" << (char)0x0a;
    os << "# bits: 2-0 --> external MUX" << (char)0x0a;
    os << "# Example channel encoding:" << (char)0x0a;
    os << "# Also consider parameters defining channel settling times" << (char)0x0a;
    os << "# ?? - link to analog power selection, min delay after turning it on" << (char)0x0a;
    os << "ADC.1=driver=ads8344;ref=2.5;add=0;muxdev=DP.4;pow=POW.ANA" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Analog Inputs (I - non filtered, immediate)" << (char)0x0a;
    os << "# label = board specific label on silkscreen" << (char)0x0a;
    os << "# dev = A/D device and channel that input uses (see channel encoding above)" << (char)0x0a;
    os << "# scale = scaling factor from front end (if applicable) (Vai = Vadc * scale) + offset" << (char)0x0a;
    os << "#          Iai=Vai/Rterm" << (char)0x0a;
    os << "# offset = offset of front end (if applicable)" << (char)0x0a;
    os << "# chandelay = min # of usec to wait after changing channel before conversion is valid (hint to ADC driver)" << (char)0x0a;
    os << "AI.1.I=label=AIDI1;dev=ADC.1-0;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.2.I=label=AIDI2;dev=ADC.1-1;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.3.I=label=AIDI3;dev=ADC.1-2;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.4.I=label=AIDI4;dev=ADC.1-3;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.5.I=label=AIDI5;dev=ADC.1-4;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.6.I=label=AIDI6;dev=ADC.1-5;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.7.I=label=AIDI7;dev=ADC.1-6;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "AI.8.I=label=AIDI8;dev=ADC.1-7;scale=2.0;offset=0;chandelay=0" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "AI.1=label=AIDI1;dev=ADC.1-8;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.2=label=AIDI2;dev=ADC.1-9;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.3=label=AIDI3;dev=ADC.1-10;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.4=label=AIDI4;dev=ADC.1-11;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.5=label=AIDI5;dev=ADC.1-12;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.6=label=AIDI6;dev=ADC.1-13;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.7=label=AIDI7;dev=ADC.1-14;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "AI.8=label=AIDI8;dev=ADC.1-15;scale=2.0;offset=0;chandelay=80" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# RTD devices (I non  filtered)" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# Assume 1 ADC must be used for all channels" << (char)0x0a;
    os << "# label=silkscreen display label" << (char)0x0a;
    os << "# type=method to measure RTD.  \"4w\" only support right now" << (char)0x0a;
    os << "# dev=analog converter used by RTD" << (char)0x0a;
    os << "# sense=channel to measure diff voltage from RTD" << (char)0x0a;
    os << "# ref=channel to measure diff voltage across reference resistor" << (char)0x0a;
    os << "# gnd=channel to measure \"zero\" diff voltage, for compensating ADC offset errors" << (char)0x0a;
    os << "# rtdpow=\"POW\" object to enable drive current to the RTD , note has intrinsic settling delay" << (char)0x0a;
    os << "# chandelay=usec to wait after changing channels before reading ADC (default is 0)" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "# Circuit assumptions:" << (char)0x0a;
    os << "#   - constant voltage or current is supplied through a precision ref. resistor" << (char)0x0a;
    os << "#   - \"ref\" channel measures voltage drop across ref resistor" << (char)0x0a;
    os << "#   - \"sense\" channel measures voltage drop across target resistor" << (char)0x0a;
    os << "#   - \"gnd\" channel measures \"zero\" voltage drop from muxes (should be ~zero)" << (char)0x0a;
    os << "#   - need to be supplied the value (in ohms) of ref resistor" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "RTD.1.I=label=RTD1;type=4w;dev=ADC.1;sense=16;ref=17;gnd=18;refres=150.0;rtdpow=POW.RTD" << (char)0x0a;
    os << "RTD.2.I=label=RTD2;type=4w;dev=ADC.1;sense=19;ref=20;gnd=21;refres=150.0;rtdpow=POW.RTD" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# NOTE - Kyle thinks we really should use the LPF version" << (char)0x0a;
    os << "RTD.1=label=RTD1;type=4w;dev=ADC.1;sense=24;ref=25;gnd=26;refres=150.0;rtdpow=POW.RTD;chandelay=80" << (char)0x0a;
    os << "RTD.2=label=RTD2;type=4w;dev=ADC.1;sense=27;ref=28;gnd=29;refres=150.0;rtdpow=POW.RTD;chandelay=80" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Pulse counters" << (char)0x0a;
    os << "# NOTE: Yes, the channel order IS 3,2,4" << (char)0x0a;
    os << "# label = board specific label on silkscreen" << (char)0x0a;
    os << "# dev = driver used to handle pulse counter" << (char)0x0a;
    os << "# ad = \"address\" of pulse counter in driver (id used by driver)" << (char)0x0a;
    os << "PULSE.1=label=DIO14;dev=ZIBRD.Pulse;ad=3" << (char)0x0a;
    os << "PULSE.2=label=DIO15;dev=ZIBRD.Pulse;ad=2" << (char)0x0a;
    os << "PULSE.3=label=DIO16;dev=ZIBRD.Pulse;ad=4" << (char)0x0a;
    os << "" << (char)0x0a;
    os << "# Pin objects" << (char)0x0a;
    os << "# Model the physical connection to an external Pin" << (char)0x0a;
    os << "# Supports typical end-point configurations, e.g. current loop terminals, labelling, etc." << (char)0x0a;
    os << "#  label=physical label of the pin" << (char)0x0a;
    os << "#  order=integer, used to sort display to match external form factor" << (char)0x0a;
    os << "#  loopen=digital output, enables current loop termination resisitor (optional)" << (char)0x0a;
    os << "#  loopres=resistance in ohms of the current loop termination resistor (optional, require if loopen present)" << (char)0x0a;
    os << "#  swscanen=digital output, enables injection of switch scanning current (optional)" << (char)0x0a;
    os << "#" << (char)0x0a;
    os << "#Power" << (char)0x0a;
    os << "PIN.1=label=LOOP;order=1" << (char)0x0a;
    os << "PIN.2=label=5V;order=2" << (char)0x0a;
    os << "# AIDI" << (char)0x0a;
    os << "PIN.3=label=AIDI1;order=3;loopen=DP.10-0;loopres=250.0;swscanen=DP.10-8" << (char)0x0a;
    os << "PIN.4=label=AIDI2;order=4;loopen=DP.10-1;loopres=250.0;swscanen=DP.10-9" << (char)0x0a;
    os << "PIN.5=label=AIDI3;order=5;loopen=DP.10-2;loopres=250.0;swscanen=DP.10-10" << (char)0x0a;
    os << "PIN.6=label=AIDI4;order=6;loopen=DP.10-3;loopres=250.0;swscanen=DP.10-11" << (char)0x0a;
    os << "PIN.7=label=AIDI5;order=7;loopen=DP.10-4;loopres=250.0;swscanen=DP.10-12" << (char)0x0a;
    os << "PIN.8=label=AIDI6;order=8;loopen=DP.10-5;loopres=250.0;swscanen=DP.10-13" << (char)0x0a;
    os << "PIN.9=label=AIDI7;order=9;loopen=DP.10-6;loopres=250.0;swscanen=DP.10-14" << (char)0x0a;
    os << "PIN.10=label=AIDI8;order=10;loopen=DP.10-7;loopres=250.0;swscanen=DP.10-15" << (char)0x0a;
    os << "#RTD" << (char)0x0a;
    os << "PIN.11=label=RTD1;order=11" << (char)0x0a;
    os << "PIN.12=label=RTD2;order=12" << (char)0x0a;
    os << "# DIO" << (char)0x0a;
    os << "PIN.13=label=DIO9;order=13" << (char)0x0a;
    os << "PIN.14=label=DIO10;order=14" << (char)0x0a;
    os << "PIN.15=label=DIO11;order=15" << (char)0x0a;
    os << "PIN.16=label=DIO12;order=16" << (char)0x0a;
    os << "PIN.17=label=DIO13;order=17" << (char)0x0a;
    os << "PIN.18=label=DIO14;order=18" << (char)0x0a;
    os << "PIN.19=label=DIO15;order=19" << (char)0x0a;
    os << "PIN.20=label=DIO16;order=20" << (char)0x0a;
    os << "" << (char)0x0a;
}

} /* namespace orm */
} /* namespace zios */
