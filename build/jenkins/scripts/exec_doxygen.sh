#!/bin/sh
# @file
#
# @copyright
# Copyright 2008, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Script to generate source code documentation via Doxygen from Hudson.
#
# @HISTORY
# Created Apr 02, 2014 by eb: created off original connect doc
# IncrDev May 11, 2009 by fl: move DOXY_DOC_DIR to within $HOME
# BugFix_ Oct 08, 2008 by farrenl: move DOXY_DOC_DIR to webroot to fix permissions bug
# Created Oct 07, 2008 by farrenl: based on Hudson config files
# @ENDHISTORY
#
# : sanity checks
if [ \( -z "$ZIOS_HOME" \) -o \( ! -d "$ZIOS_HOME" \) ]; then
    echo "ZIOS_HOME environment variable undefined or directory does not exist." >&2
    usage >&2
    exit 1
fi

# : initialization
DOXY_DOC_DIR=$HOME/public_html/ZIOS/doxygen
    
# execute doxygen
echo
echo "--------------------------------------------------------------------------------------"
echo "  DOXYGEN"
echo "--------------------------------------------------------------------------------------"
echo

cd /tmp				# run in /tmp
rm -rf $DOXY_DOC_DIR
mkdir $DOXY_DOC_DIR
$ZIOS_HOME/build/doxygen/genZIOSSrcDocs $DOXY_DOC_DIR

# : cleanup and return
#use exit code of last command
