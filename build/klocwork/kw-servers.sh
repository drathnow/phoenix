#!/bin/sh
# @file
#
# @copyright
# Copyright 2008, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Script to start, stop and check status of Klocwork servers.
# Usage: kw-servers.sh {start|stop|restart|status|help}
#
# @HISTORY
# INCRDEV Sep 02, 2014 by eb: changed to conform to new dir.... zi1ssled2.
# Created Oct 30, 2008 by alickq:
# @ENDHISTORY
#
set -x
# : define kw variables
KW_HOME=/opt/klocwork
KW_BIN=$KW_HOME/bin
KW_PROJ_ROOT=/opt/klocwork/projects_root

if [ ! -d "$KW_HOME" ]; then
    echo "Directory not found: $KW_HOME" >&2
    exit 1
fi

if [ ! -d "$KW_BIN" ]; then
    echo "Directory not found: $KW_BIN" >&2
    exit 1
fi

if [ ! -d "$KW_PROJ_ROOT" ]; then
    echo "Directory not found: $KW_PROJ_ROOT" >&2
    exit 1
fi

if [ "`whoami`" != "rdbuild" ]; then
    echo "User is not rdbuild: Please run as rdbuild" >&2
    exit 1
fi

# : initialization
set -e				# fail on first error

if [ "$1" = "help" -o -z "$1" ]; then
    echo
    echo "Usage:"
    echo "kw-servers.sh {start|stop|restart|status|help}"
    echo
fi

if [ "$1" = "stop" -o "$1" = "restart" ]; then	# stop the servers
    nice $KW_BIN/kwservice stop $KW_PROJ_ROOT database
    nice $KW_BIN/kwservice stop $KW_PROJ_ROOT project
    nice $KW_BIN/kwservice stop $KW_PROJ_ROOT web
fi

if [ "$1" = "start" -o "$1" = "restart" ]; then # start the servers
    nice $KW_BIN/kwservice start $KW_PROJ_ROOT database
    nice $KW_BIN/kwservice start $KW_PROJ_ROOT project
    nice $KW_BIN/kwservice start $KW_PROJ_ROOT web
fi

if [ "$1" = "status" ]; then # check the status of the servers
    nice $KW_BIN/kwservice check $KW_PROJ_ROOT
fi



