#!/bin/sh
# @file  copy_jenkins_configs.sh
#
# @copyright
# Copyright 2008, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Script to compare and copy the JENKINS Continuous Integration Server
# configuration files between JENKINS_HOME and the current directory.
# See also http://hudson.dev.java.net/
#
# Do not edit configuration files here unless you know what you are doing.
# Otherwise edit them in JENKINS, copy them here, and check them in.
#
# @HISTORY
# Created Apr 02, 2014 by eb: new file for ZIOS from original script
# IncrDev Oct 30, 2009 by fl: expand func to find conf files to allow plugin config
# BugFix_ May 11, 2009 by fl: don't track secret.key as it changes for each machine
# BugFix_ Feb 03, 2009 by fl: diff now shows files not common to both directories
# IncrDev Oct 07, 2008 by farrenl: update paths
# BugFix_ Aug 29, 2008 by farrenl: create target dir before copying files
# Created Jul 15, 2008 by farrenl: 
# @ENDHISTORY
#

# : function to find configuration files
function findconfig() {
#    find $1 -maxdepth 3 -name 'config.xml' -o -name 'jenkins.*.xml' | sort
    find $1 -maxdepth 3 -name '*.xml' | grep -v /war/WEB-INF/ | sort
}

# : initialization
JENKINS_HOME=/extra/var/jenkins
JENKINS_CONFIG=$JENKINS_HOME
LOCAL_CONFIG=`pwd`/config
echo local $LOCAL_CONFIG
exit_status=0
case "$1" in
    "here")
	SRC=$JENKINS_CONFIG
	TRG=$LOCAL_CONFIG
	echo "Copying config files from $JENKINS_HOME ..."
	;;
    "there")
	SRC=$LOCAL_CONFIG
	TRG=$JENKINS_CONFIG
	echo "Copying config files to $JENKINS_HOME ..."
	;;
    "diff")
	for src in `findconfig $LOCAL_CONFIG`; do
	    trg=$JENKINS_CONFIG/`echo $src | sed "s#$LOCAL_CONFIG/##"`
	    diff -u $src $trg
	    rv=$?
	    if [ $rv -ne 0 ]; then
		exit_status=$rv
	    fi
	done
	localFiles=`findconfig $LOCAL_CONFIG | sed "s#$LOCAL_CONFIG/##"`
	remoteFiles=`findconfig $JENKINS_CONFIG | sed "s#$JENKINS_CONFIG/##"`
	for file in `echo $localFiles $remoteFiles | tr ' ' '\n' | sort | uniq -c | awk '$1 != "2" {print $2}'`; do
	    if [ -f "$LOCAL_CONFIG/$file" ]; then
		echo "Only in $LOCAL_CONFIG: $file"
	    elif [ -f "$JENKINS_CONFIG/$file" ]; then
		echo "Only in $JENKINS_CONFIG: $file"
	    else
		echo "Only: $file"
	    fi
	done
	exit $exit_status
	;;
    *)
	echo
        echo "Copy JENKINS CI config files to and from this directory and JENKINS HOME."
	echo "Usage: $0 here            # pull config from JENKINS_HOME"
	echo "       $0 there           # push config to JENKINS_HOME"
	echo "       $0 diff            # compare changes in JENKINS_HOME"
	echo
	echo "Found these JENKINS CI config files:"
	findconfig $JENKINS_HOME
	echo
	exit 1
	;;
esac

# : diff all JENKINS configuration files
for src in `findconfig $SRC`; do
    if [ "$SRC" = "$LOCAL_CONFIG" ]; then
	trg=$JENKINS_CONFIG/`echo $src | sed "s#$LOCAL_CONFIG/##"`
    else
	trg=`echo $src | sed "s#$JENKINS_CONFIG/#$LOCAL_CONFIG/#"`
    fi
    mkdir -p `dirname $trg`
    cp -v $src $trg
    rv=$?
    if [ $rv -ne 0 ]; then
	exit_status=$rv
    fi
done

# : cleanup and return
if [ "$SRC" = "$LOCAL_CONFIG" ]; then
    echo
    echo "** JENKINS must be restarted or the config reloaded for changes to take effect."
    echo
fi
exit $exit_status
