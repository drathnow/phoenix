#!/bin/sh
# QA start up script for only zios (allows server address from config.json to be used)
export LD_LIBRARY_PATH=/zioshome/zios:/usr/local/bin:$LD_LIBRARY_PATH

cd /zioshome/zios

# make sure required subdirectories exist
mkdir -p log
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






if [ $# = 0 ]; then
  ./zios --config /$PWD/config.json  --serveraddress &
else
  ./zios --config /$PWD/config.json --menu --serveraddress  
fi
