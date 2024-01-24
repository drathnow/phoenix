#!/bin/sh
# @file
#
# @copyright
# Copyright 2008, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Script to run Klocwork static analysis on the Connect source code.
# Usage: kwConnect.sh [--force] [buildname]
#
# @HISTORY
# Created Apr 01, 2014 by eb: based on script from connect project
# @ENDHISTORY
#

# : define kw variables
KW_HOME=/opt/klocwork
KW_LIC_ADDR=esbuild1.zedisolutions.com
KW_LIC_PORT=27000
KW_PROJ_NAME=ZIOS
KW_PROJ_DIR=$KW_HOME/kw-project-tables/$KW_PROJ_NAME
KW_CONF_FILE=$ZIOS_HOME/zios.out # ZIOS_HOME defined in kw project properties
KW_SERVER_URL=http://zi1ssled2.zedisolutions.com:8070

# : define kw analysis options
KW_OPT_WHOLE="-f"		# overwrite existing cached data (engage with '--force')
KW_OPT_INCREMENTAL="-I"		# incremental build (default)
KW_ANALYSIS_OPTS="--license-host $KW_LIC_ADDR \
                  --license-port $KW_LIC_PORT \
                  --tables-directory $KW_PROJ_DIR \
                  --project $KW_PROJ_NAME \
                  -j 2"

# : sanity checks
if [ -z "$ZIOS_HOME" ]; then
    echo "Environment variable 'ZIOS_HOME' is not defined." >&2
    exit 1
fi
if [ ! -d "$ZIOS_HOME" ]; then
    echo "Directory not found: ZIOS_HOME=$ZIOS_HOME" >&2
    exit 1
fi

if [ ! -d "$KW_HOME" ]; then
    echo "Directory not found: $KW_HOME" >&2
    exit 1
fi
if [ ! -d "$KW_PROJ_DIR" ]; then
    echo "Directory not found: $KW_PROJ_DIR" >&2
    exit 1
fi

# : initialization
set -e				# fail on first error

# : parse command line options
if [ "$1" = "--force" ]; then	# run an incremental build
    shift
    KW_ANALYSIS_OPTS="$KW_ANALYSIS_OPTS $KW_OPT_WHOLE"
else
    KW_ANALYSIS_OPTS="$KW_ANALYSIS_OPTS $KW_OPT_INCREMENTAL"
fi
if [ -n "$1" ]; then		# use given build name
    BUILD_NAME="$1"
fi

if [ "$2" = "--report-only" ]; then 
#we are done...
exit 0
fi

cd $ZIOS_HOME

# : prepare the source code
hg update				# update source
hg status				# verify clean tree

# : get hg revision number for build name
HG_REVISION=$(hg summary | grep ^parent: | cut -d: -f3 | awk '{print $1}')
if [ -z "$BUILD_NAME" ]; then
    BUILD_NAME=$HG_REVISION
fi

# : verify build name
BUILD_NAME="build_$BUILD_NAME"	# kw needs "build_" prefix
if $KW_HOME/bin/kwadmin list-builds $KW_PROJ_NAME | grep -qe "$BUILD_NAME"; then
    echo "Build '$BUILD_NAME' already exists." >&2
    exit 0
fi

# : create kw configuration file for Connect (rebuild in case files changed)
rm -f $KW_CONF_FILE
# remove Release output directories 
find . -name Release* |grep -v sharedlibs |xargs rm -rf

# run kw analysis
nice $KW_HOME/bin/kwinject -o $KW_CONF_FILE $ZIOS_HOME/kwbuild.sh $ZIOS_HOME # run shell under kw
nice $KW_HOME/bin/kwbuildproject $KW_ANALYSIS_OPTS $KW_CONF_FILE --url $KW_SERVER_URL/$KW_PROJ_NAME
nice $KW_HOME/bin/kwadmin load $KW_PROJ_NAME $KW_PROJ_DIR --name $BUILD_NAME

# : cleanup and return
#use exit status of last command
