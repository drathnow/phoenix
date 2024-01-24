#!/bin/sh
# @file
#
# @copyright
# Copyright 2015, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Image creation functions for generating a release image; should be sourced
# from the creation script.
#
# Assume we are already in the release image directory.
#
# DO NOT REFERENCE SOURCE FILES BY ABSOLUTE PATHS OR RELATIVE TO THIS DIRECTORY
# ALWAYS REFERENCE RELATIVE TO $ZIOS_HOME
#
# @HISTORY
# BugFix_ Nov 09, 2009 by fl: make stripBinaries not fail if nothing to strip
# Created Feb 23, 2009 by farrenl: abstracted from Egan's create_image.sh scripts
# @ENDHISTORY
#

# : initialization
set -e					# fail on first error
OBJDIR=$PLT		         	# 

# : sanity checks
if [ -z "$ZIOS_HOME" ]; then
    echo "Environment variable ZIOS_HOME is undefined." >&2
    exit 1
fi
if [ ! -d "$ZIOS_HOME" ]; then
    echo "Environment variable ZIOS_HOME points to a non-existant directory." >&2
    exit 1
fi

# :
report() {
    echo "createImage: $*"
}

# : start copying files to the release image from the output obj directory
copyObj() {
    sPath=`dirname $1`
    sFile=`basename $1`
    dest=$2
    cp -dr $ZIOS_HOME/$sPath/$OBJDIR/$sFile $dest
    report "copyobj --> $dest/$sFile"
}

# : start copying files to the release image from the actual source dir (used for things like scripts)
copySrc() {
    sPath=`dirname $1`
    sFile=`basename $1`
    dest=$2
    cp -dr $ZIOS_HOME/$sPath/$sFile $dest
    report "copysrc --> $dest/$sFile"
}

#  ############### generic functions ###################################################
# : used to do variable susbtituion in shell file templates
# : $1 is the source template
# : $2 is the script file to create
#
#   replaces:
#     %%PKG%% -> the package name with $PKG
#     %%TAR%% -> the TARFILE name with $TARFILE
#     %%ZIOS_CURRENT_CONF_FILE%% -> current configuration filename
#     %%ZIOS_CURRENT_MENU_CMD%% -> current conf product header and menuing command
alterScript() {
    if [ -z "$PKG" ]; then
	echo "ERROR: variable 'PKG' undefined" >&2
	test 1 -eq 0
	return 1
    fi
	if [ -z "$TARFILE" ]; then
	echo "ERROR: variable 'TARFILE' undefined" >&2
	test 1 -eq 0
	return 1
    fi
#    if [ -z "$ZIOS_CURRENT_CONF_FILE" ]; then
#	echo "ERROR: variable 'ZIOS_CURRENT_CONF_FILE' undefined" >&2
#	test 1 -eq 0
#	return 1
 #   fi
#    if [ -z "$ZIOS_CURRENT_MENU_CMD" ]; then
#	echo "ERROR: variable 'ZIOS_CURRENT_MENU_CMD' undefined" >&2
#	test 1 -eq 0
#	return 1
#    fi
    sPath=`dirname $1`
    sFile=`basename $1`
    dest=$2
    sed -e "s@%%PKG%%@$PKG@g"\
	-e "s@%%TAR%%@$TARFILE@g"\
#	-e "s@%%ZIOS_CURRENT_CONF_FILE%%@$ZIOS_CURRENT_CONF_FILE@g"\
#	-e "s@%%ZIOS_CURRENT_MENU_CMD%%@$ZIOS_CURRENT_MENU_CMD@g"\
        < $ZIOS_HOME/$sPath/$sFile > $dest
    report "createImage: creating --> $dest"
}

# : used to change permisions on file
changeMod() {
    mde=$1
    sPath=`dirname $2`
    sFile=`basename $2`
    chmod $1 $sPath/$sFile
    report "chmod --> $1 $sPath/$sFile"
}

# : used to create a symbolic link for a file
slinkFile() {
    sPath=`dirname $1`
    sFile=`basename $1`
    dPath=`dirname $2`
    dFile=`basename $2`
    if [ "$sPath" = "$dPath" ]
    then
       ( cd $sPath;ln -sf  $sFile $dFile )
       report "slink --> $sFile $dFile"
    else
       return 1 
    fi
}

# : used to create a symbolic link for a file
linkFile() {
    slinkFile $*
#    sPath=`dirname $1`
#    sFile=`basename $1`
#    dest=$2
#    ln -sf  ./$sPath/$sFile $dest
#    report "link --> $sFile $dest"
}

# : used to strip binary executables in the given directory
stripBinaries() {
    # strip all ELF binary files to save space
    files=`find $1 -perm /u=x -a -type f | xargs file | grep 'ELF .* not stripped' | cut -d: -f1`
    if [ -n "$files" ]; then
        $STRIP $files
    fi
}
