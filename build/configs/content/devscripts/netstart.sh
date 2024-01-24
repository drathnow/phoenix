#!/bin/sh
# QA start up script for just netManager
#currently only starts up zios but will eventually start Watchdog manager to start others
export LD_LIBRARY_PATH=/zioshome/zios:/usr/local/bin:$LD_LIBRARY_PATH

cd /zioshome/zios

./networkmanager $@ &

