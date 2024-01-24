#!/bin/sh

# @file initdb.sh
#
# @copyright
# Copyright 2015, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
#  this file is the script that initializes the db file at the end
#   of the install cycle. It is in a separate script to 
#   isolate functionality
#
#
#
# @HISTORY
# Created Sep 10, 2015 by eganb:
# @ENDHISTORY
#


if [ $# -ne 2 ]; then
    echo "Usage: `basename $0` <db image dir> <deployment package>" >&2
    exit 1
fi

DB_DIR=$1
PKG=$2

#verify our working dir is defined in environement
if [ "$WD" = "" ] ; then
    echo "ERROR: Working Dir not defined"
    exit 2
fi

#verify our db was in our install....
if [ ! -d $WD/$DB_DIR ] ; then
    echo "ERROR: DB dir $WD/$DB_DIR was not found"
    exit 3
fi

#verify our package installed.....
if [ ! -d $WD/$PKG ] ; then
    echo "ERROR: PKG dir $PKG is not present"
    exit 5
fi

#for safety, remove the db dir (shouldn't be there but....)
if [ -d $WD/$PKG/db ] ; then
    rm -rf $WD/$PKG/db
fi

# Create the db directory. ZIOS will create the db file if it doesn't exist
mkdir -p $WD/$PKG/db

# we done!!!


