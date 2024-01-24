#!/bin/sh
# IOBoardTester.sh - Test IO Board functions
#
#
#@history
# Created Aug 16, 2019 by Mattias Park
#$endhistory

setADCMux() {
    local ADC=$1
    return 0
}

#
# Sets a line on the IO board.
#
#   $1 - The chip name
#   $2 - Line number
#   $3 -true  (on), false (off)
#
setIOBoardLine() {
    local CHIP=$1
    local LINE=$2
    local VALUE=`echo $3 | sed -e 's/\(.*\)/\L\1/'`
    if [ "$VALUE" = "true" ];
    then
        VALUE=1
    else
        VALUE=0 
    fi
        
    gpioset $CHIP $LINE=$VALUE
    
    if [ $VALUE = $(gpioget $CHIP $LINE) ];
    then
        return 0
    else
        return 1
    fi
}

setIOBoardBit() {
    local PORT=$1
    local BIT=$2
    local VALUE=$3
    return 0
}

getIOBoardBit() {
    local PORT=$1
    local BIT=$2
    DATA="true"
}

setSensorMux() {
    local SOMETHING=$1
    return 0
}

IOBoardConvert() {
    local CHANNEL=$1
    local SOMETHING_ELSE=$2
    local VALUE=$3
    return 0
}

setMainDOPort() {
    return 0
}

i2cWrite() {
    # This is some kind of i2c command... i dunno
    return 0
}

echo "0:OK, epic"
while [ "$OPT" != "x" ]
do
    DATA=""
    read -sn1 OPT
    case $OPT in
    
        '')  # ignore extra line feeds
            ;;
    
        'x')
            ;;
    
        'c')
            setADCMux $1
            if [ $? -eq 0 ]; then
                echo "0:AMUX set to 0"
            else
                echo "1:AMUX not set to 0"
            fi
            ;;
    
        # VLoop
        'V')
            setIOBoardLine gpio_p2 8 true
            if [ $? -eq 0 ]; then
                echo "0:loop Power on"
            else
                echo "1:loop Power on Fail"
            fi
            ;;
                    
        'v')
            setIOBoardLine gpio_p2 8 false
            if [ $? -eq 0 ]; then
                echo "0:loop Power off"
            else
                echo "1:loop Power off Fail"
            fi
            ;;
    
        # Voltage High (24/12)
        'H')
            setIOBoardLine gpio_p2 9 false
            if [ $? -eq 0 ]; then
                echo "0:loop Power 24V"
            else
                echo "1:loop Power 24V Fail"
            fi
            ;;  # low is 24V
            
        'h')
            setIOBoardLine gpio_p2 9 true
            if [ $? -eq 0 ]; then
                echo "0:loop Power 12V"
            else
                echo "1:loop Power 12V Fail"
            fi
            ;;  # high is 12V
    
            # 5V on
        'F')
            setIOBoardLine gpio_p2 10 true
            if [ $? -eq 0 ]; then
                echo "0:5V Power on"
            else
                echo "1:5V Power on Fail"
            fi
            ;;
    
        # 5V off
        'f')
            setIOBoardLine gpio_p2 10 false
            if [ $? -eq 0 ]; then
                echo "0:5V Power off"
            else
                echo "1:5V Power off Fail"
            fi
            ;;
    
            # RTD
        'R')
            setIOBoardLine gpio_p2 11 true
            if [ $? -eq 0 ]; then
                echo "0:RTD Power on"
            else
                echo "1:RTD Power on Fail"
            fi
            ;;
        'r')
            setIOBoardLine gpio_p2 11 false
            if [ $? -eq 0 ]; then
                echo "0:RTD Power off"
            else
                echo "1:RTD Power off Fail"
            fi
            ;;
    
            # ANA
        'A')
            setIOBoardLine gpio_p2 12 true
            if [ $? -eq 0 ]; then
                echo "0:Analog Power on"
            else
                echo "1:Analog Power on Fail"
            fi
            ;;
            
        'a')
            setIOBoardLine gpio_p2 12 false
            if [ $? -eq 0 ]; then
                echo "0:Analog Power off"
            else
                echo "1:Analog Power off Fail"
            fi
            ;;
    
        # Switch Scan
        'S')
            setBoardLine gpi_p3 3 true
            if [ $? -eq 0 ]; then
                echo "0:DI9-16 switch scan Power on"
            else
                echo "1:DI9-16 switch scan Power on Fail"
            fi
            ;;  
            
        # SENS_MUX.1 == bit 1 == 2 to turn on
        's')
            setBoardLine gpi_p3 3 false
            if [ $? -eq 0 ]; then
                echo "0:DI9-16 switch scan Power off"
            else
                echo "1:DI9-16 switch scan Power off Fail"
            fi
            ;;
    
        # Outputs (first usable output is at bit 16)
        'O')
            setBoardLine gpi_p3 0 true 
            if [ $? -eq 0 ]; then
                echo "0:DO1-8 grounded"
            else
                echo "1:DO1-8 grounded Fail"
            fi
            ;;
            
        'o')
            setBoardLine gpi_p3 0 false 
            if [ $? -eq 0 ]; then
                echo "0:DO1-8 open cct"
            else
                echo "1:DO1-8 open cct FAIL"
            fi
            ;;
    
        #
        # Current loop termination
        # each high bit enables termination for one AIN
        #
        'T')
            i2cWrite PCA6416Driver::PCA6416_ADDR, PCA6416Driver::PCA6416_REG_OUT0, 0xFF
            if [ $? -eq 0 ]; then
                echo "0:Termination resistors Enabled"
            else
                echo "1:Termination resistors Enabled Fail"
            fi
            ;;
            
        't')
            i2cWrite PCA6416Driver::PCA6416_ADDR, PCA6416Driver::PCA6416_REG_OUT0, 0x00
            if0x00
            if [ $? -eq 0 ]; then
                echo "0:Termination resistors disabled"
            else
                echo "1:Termination resistors disabled Fail"
            fi
            ;;
    
            # analog switch scan drive
            # each high bit enables injection of +5V through 10K when
            # switch scan is enabled
        'D')
            i2cWrite PCA6416Driver::PCA6416_ADDR, PCA6416Driver::PCA6416_REG_OUT1, 0xFF
            if [ $? -eq 0 ]; then
                echo "0:DI1-DI8 Switch Scan Power on"
            else
                echo "1:DI1-DI8 Switch Scan Power on Fail"
            fi
            ;;
            
        'd')
            i2cWrite PCA6416Driver::PCA6416_ADDR, PCA6416Driver::PCA6416_REG_OUT1, 0x00
            if [ $? -eq 0 ]; then
                echo "0:DI1-DI8 Switch Scan Power off"
            else
                echo "1:DI1-DI8 Switch Scan Power off Fail"
            fi
            ;;
    
            # ACD MUX sets
        'l')
            setADCMux 0
            if [ $? -eq 0 ]; then
                echo "0:AMUX1"
            else
                echo "1:AMUX1 Fail"
            fi
            ;;
            
        'm')
            setADCMux 1
            if [ $? -eq 0 ]; then
                echo "0:AMUX2"
            else
                echo "1:AMUX2 Fail"
            fi
            ;;
            
        'n')
            setADCMux 2
            if [ $? -eq 0 ]; then
                echo "0:AMUX3"
            else
                echo "1:AMUX3 Fail"
            fi
            ;;
            
        'p')
            setADCMux 3
            if [ $? -eq 0 ]; then
                echo "0:AMUX4"
            else
                echo "1:AMUX4 Fail"
            fi
            ;;
            
        'u')
            setADCMux 4
            if [ $? -eq 0 ]; then
                echo "0:AMUX5"
            else
                echo "1:AMUX5 Fail"
            fi
            ;;
            
        'w')
            setADCMux 5
            if [ $? -eq 0 ]; then
                echo "0:AMUX6"
            else
                echo "1:AMUX6 Fail"
            fi
            ;;
            
        'y')
            setADCMux 6
            if [ $? -eq 0 ]; then
                echo "0:AMUX7"
            else
                echo "1:AMUX7 Fail"
            fi
            ;;
            
        'z')
            setADCMux 7
            if [ $? -eq 0 ]; then
                echo "0:AMUX8"
            else
                echo "1:AMUX8 Fail"
            fi
            ;;
    
    
        # ADC reads
        
        '1'|'2'|'3'|'4'|'5'|'6'|'7'|'8')
            let "CHANNEL = $1 - 1"
            let "VALUE = 0" # I don't know what value is supposed to represent
            # $value * 2.5 / 65536.0 * 2
            IOBoardConvert $CHANNEL _aMuxCh $value
            if [ $? -eq 0 ]; then
                echo "0:Read AI $CHAN $RESULT"
            else
                echo "1:Read AI $CHAN Fail"
    
            fi
            ;;
    
            # AIDI1-8 reads
        '!')
            getIOBoardBit Port0 0
            if [ $? -eq 0 ]; then
                # I think $DATA might show the opposite of the original C++ script. I dunno lol
                echo "0:Read DI1, $DATA"
            else
                echo "1:Read DI1 Fail"
            fi
            ;;
    
        '@')
            getIOBoardBit Port0 1
            if [ $? -eq 0 ]; then
                echo "0:Read DI2, $DATA"
            else
                echo "1:Read DI2 Fail"
            fi
            ;;
            
        '$')
            getIOBoardBit Port0 2
            if [ $? -eq 0 ]; then
                echo "0:Read DI3, $DATA"
            else
                echo "1:Read DI3 Fail"
            fi
            ;;
            
        '%')
            getIOBoardBit Port0 3
            if [ $? -eq 0 ]; then
                echo "0:Read DI4, $DATA"
            else
                echo "1:Read DI4 Fail"
            fi
            ;;
        '^')
            getIOBoardBit Port0 4
            if [ $? -eq 0 ]; then
                echo "0:Read DI5, $DATA"
            else
                echo "1:Read DI5 Fail"
            fi
            ;;
        '&')
            getIOBoardBit Port0 5
            if [ $? -eq 0 ]; then
                echo "0:Read DI6, $DATA"
            else
                echo "1:Read DI6 Fail"
            fi
            ;;
        '*')
            getIOBoardBit Port0 6
            if [ $? -eq 0 ]; then
                echo "0:Read DI7, $DATA"
            else
                echo "1:Read DI7 Fail"
            fi
            ;;
            
        '-')
            getIOBoardBit Port0 7
            if [ $? -eq 0 ]; then
                echo "0:Read DI8, $DATA"
            else
                echo "1:Read DI8 Fail"
            fi
            ;;
    
        # DIO9-16 reads
        '=')
            getIOBoardBit Port2 0
            if [ $? -eq 0 ]; then
                echo "0:Read DI9, $DATA"
            else
                echo "1:Read DI9 Fail"
            fi
            ;;
            
        '+')
            getIOBoardBit Port2 1
            if [ $? -eq 0 ]; then
                echo "0:Read DI10, $DATA"
            else
                echo "1:Read DI10 Fail"
            fi
            ;;
            
        '(')
            getIOBoardBit Port2 2
            if [ $? -eq 0 ]; then
                echo "0:Read DI11, $DATA"
            else
                echo "1:Read DI11 Fail"
            fi
            ;;
            
        ')')
            getIOBoardBit Port2 3
            if [ $? -eq 0 ]; then
                echo "0:Read DI12, $DATA"
            else
                echo "1:Read DI12 Fail"
            fi
            ;;
            
        '[')
            getIOBoardBit Port2 4
            if [ $? -eq 0 ]; then
                echo "0:Read DI13, $DATA"
            else
                echo "1:Read DI13 Fail"
            fi
            ;;
            
        ']')
            getIOBoardBit Port2 5
            if [ $? -eq 0 ]; then
                echo "0:Read DI14, $DATA"
            else
                echo "1:Read DI14 Fail"
            fi
            ;;
            
        '9')
            getIOBoardBit Port3 2
            if [ $? -eq 0 ]; then
                echo "0:Read DI15, $DATA"
            else
                echo "1:Read DI15 Fail"
            fi
            ;;
            
        '0')
            getIOBoardBit Port3 7
            if [ $? -eq 0 ]; then
                echo "0:Read DI16, $DATA"
            else
                echo "1:Read DI16 Fail"
            fi
            ;;
            
        *)
            echo "2:Unknown command - '$OPT'."
    
    esac
done
