#!/bin/sh

if [ $# -ne 0 ]; then
  if [ -f $1 ]; then
     cat $1 | dd bs=1000 count=1 of=/sys/class/i2c-adapter/i2c-0/0-0050/eeprom
  fi
fi

dd bs=400 count=1 if=/sys/class/i2c-adapter/i2c-0/0-0050/eeprom
