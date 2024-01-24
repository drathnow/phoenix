#!/bin/sh
#
# Startup script for zios
#
export LD_LIBRARY_PATH=/zioshome/zios:/usr/local/bin:$LD_LIBRARY_PATH

if [ -d "/zioshome/webtmp" ]
then
    rm -rf /zioshome/webtmp/*
else
    mkdir /zioshome/webtmp
    chmod 777 /zioshome/webtmp
fi

cd /zioshome/zios
mkdir -p log

#
# if tmplog doesn't exist, make a symlink to /tmp folder
# on embedded device, /tmp is a ram drive so keep files small
# on shared Linux dev, create tmplog as a regular directory manually before running this....
# check and remove broken link
#
if [ -h tmplog ]; then
  if [ ! -e tmplog ]; then
    rm tmplog
  fi
fi

#
# create tmplog if missing
#
if [ ! -d tmplog ]; then
  mkdir -p /tmp/tmplog
  ln -s /tmp/tmplog tmplog
fi;

#for now until kernel is changed for not starting wdkicker.
killall wdtkicker

# start watchdog Manager
./watchdog &

