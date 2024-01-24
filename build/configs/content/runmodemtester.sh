#!/bin/sh
# the start up script for modemtester
#set up so path:
export LD_LIBRARY_PATH=/zioshome/zios:/usr/local/bin:$LD_LIBRARY_PATH

#change to executable directory:
cd /zioshome/zios

# make sure required logging subdirectories exist:
mkdir -p log
# if tmplog doesn't exist, make a symlink to /tmp folder
# on embedded device, /tmp is a ram drive so keep files small
# on shared Linux dev, create tmplog as a regular directory manually before running this....
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

# start tester
./modemtester

echo "result: name=Main Board Serial; tag=mfg.modem.mainserial;value=`hostname`;state=pass"
echo "ZEDIDONE."
