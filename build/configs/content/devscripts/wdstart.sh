#!/bin/sh
# QA start up script for watchdog 
# !!! use only to start watchdog if start.sh wasn't used to start watchdog
#currently only starts up zios but will eventually start Watchdog manager to start others
export LD_LIBRARY_PATH=/zioshome/zios:/usr/local/bin:$LD_LIBRARY_PATH
cd /zioshome/zios

# make sure required subdirectories exist
mkdir -p log
mkdir -p report

# if tmplog doesn't exist, make a symlink to /tmp folder
# on embedded device, /tmp is a ram drive so keep files small
# on shared Linux dev, create tmplog as a regular directory manually before running this

# check and remove broken link
if [ -h tmplog ]; then
  if [ ! -e tmplog ]; then
    rm tmplog
  fi
fi
# create tmplog if missing
if [ ! -d tmplog ]; then
  mkdir -p /tmp/tmplog
  ln -s /tmp/tmplog tmplog
fi;
killall wdtkicker
/zioshome/zios/watchdog $@ &
