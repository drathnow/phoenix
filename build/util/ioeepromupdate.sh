#!/bin/sh

if [ $# -ne 0 ]; then
  if [ -f $1 ]; then
     cat $1 | dd bs=1024 count=12 of=/sys/class/i2c-adapter/i2c-1/1-0052/eeprom
  fi
fi

dd bs=1024 count=11 if=/sys/class/i2c-adapter/i2c-1/1-0052/eeprom
