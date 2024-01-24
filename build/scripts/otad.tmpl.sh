#!/bin/sh
# @file otad.tmpl.sh
#
# @copyright
# Copyright 2015, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Template file used in creation of a script to facitilate otad of new version.
#
# The template employs token substiution: the token %%PKG%% is replaced with the actual package 
#   name and %%TAR%% with tar file name when the install.sh is created.
#
# script usage: otad.sh {install|cleanup}
#
# It has 2 phases:
#  1) called with 'install'
#       this sets up the new app and 'archives' old app
#  2) called with cleanup (after reboot)
#       this cleans up the old app and removes the otad artifacts
#
# This script provides the following exit statuses:
#
#   0 - Successful completion
#   2 - OTAD tar file not found
#   3 - OTAD version already installed
#   4 - Unable to unpack tarfile
#   5 - New app directory not found after unpacking tarfile
#   101 - Unrecognized option (argument to script)
#   
#
# Note: it is two phase because you can't remove the old app while it is still executing so
#        a reboot is needed between the phases
#
# @HISTORY
# Created Mar 12, 2015 by eganb: based on otad.sh from the zedPAC
# IncrDev Jul 24, 2015 by eganb: changed to TAR from PKG for tar file
# IncrDev Sep 08, 2015 by eganb: added log file/ check for tar files and copy if generic, remove tbz2 file on all failures.
# IncrDev Sep 10, 2015 by eganb: added removal of db dir (for install pkg)
# IncrDev Apr 15, 2016 by eganb: new sub script that brings over passwords from old file.
# @ENDHISTORY
#
#set -x

#
# First and foremost, this version CANNOT be installed on any kernel version less than 5.
#
KERNEL_VERSION=$(uname -r | awk '{split($0,a,"."); print a[1]}')
if [ $KERNEL_VERSION -lt 5 ];
then
    echo "ZIOS version 2 or higher cannot be installed on this Linux version" 
    return 1
fi

# must have at least 1 parameter specifying whether this is a install pass or a cleanup pass.
if [ $# -lt 1 ]
then
    echo "missing parameter, usage: otad.sh <install|cleanup>" >&2
    exit 1
fi
if [ "$ZIOS_HOME" == "" ]
then
    export WD=/zioshome
else
    export WD=$ZIOS_HOME
fi
OUT_FILE=$WD/ZIOS.tbz2
OTAD_LOG="$WD/otad.log"
DB_DIR=dbinit           # only for install, here so it can be removed.

#do the install portion of the script... ##########
if [ $1 = "install" ]
then
    echo "Installing"

    # check for either the generic file (run from root-otad.sh) or specific file (just run by self)
    if [ ! -f $OUT_FILE ]; then
       if [ ! -f $WD/%%TAR%% ]; then
          echo "ERROR: could not find generic TAR file ($OUT_FILE) or specific TAR FILE ($WD/%%TAR%%) "
          exit 2
       fi
    else
        # rename generic file back to otad tar name :)
        echo "renaming $OUT_FILE to $WD/%%TAR%%..."
        mv $OUT_FILE $WD/%%TAR%%
    fi

    #ensure the zios directory exists else olddir will point to /zios - NOT A GOOD THING (will delete /
    #   zios later) paranoia...
    if [ -d $WD/zios ]
    then
        # get the actual absolute path of the 'app' directory --> olddir
        olddir="`cd $WD/zios; pwd -P`"
        echo "old dir --> $olddir"
    else
        olddir=/zioshome/nodir
    fi

    #protect against old dir pointing to 'new' app directory (re-install)
    # this  would allow "cleanup" to remove the active application from under our feet!
    if [ $olddir = $WD/%%PKG%% ]
    then
        # notice and exit (with error)
        echo "ERROR: %%PKG%% already directory exists. Aborting" >&2
        #remove the installer artifacts.
        rm -f $WD/zios_installer.*
        rm -f $WD/%%TAR%%
        rm -r $WD/otad.sh
        rm -rf $WD/$DB_DIR
        exit 3;
    fi
      
    # safety... if otad install ran twice before cleaned up...
    if [ -d $WD/old_zios ]
    then
        # get the actual absolute path of the 'app' directory --> olddir
        existdir="`cd $WD/old_zios; pwd -P`"
        echo "existing dir --> $existdir"
        # make sure that old_zios DOESN'T point to zios!!!
        if [ $olddir !=  $existdir ]
        then
            rm -rf $existdir
        fi
    fi

    # If the otad.properties file exist, we won't override it.
    if [ -f $WD/otad.properties ]
    then
        EXCLUDE="--exclude=otad.properties"
    fi

    #extract new app to it's directory under $WD (bringing all install artifacts to $WD also)
    echo "Extracting new zios package...." >> $OTAD_LOG
    tar -xjvf $WD/%%TAR%% -C $WD --no-same-owner $EXCLUDE
    if [ $? -ne 0 ]; then
        echo >&2 "Extraction failed"  >> $OTAD_LOG
        rm -f $WD/%%TAR%%
        rm -rf $WD/$DB_DIR  # just to be safe :)
        exit 4;
    fi

    #remove unneeded script and dir
    rm -f $WD/zios_install.sh
    rm -rf $WD/$DB_DIR  # and install stuff for db

    # safety, if new dir does not exist it is an error at this point.
    if [ ! -d $WD/%%PKG%% ]; then
        echo >&2 "$WD/%%PKG%% not found.... error." >> $OTAD_LOG
        rm -f $WD/%%TAR%%
        exit 5
    fi

    # synch up the database(and message) data from previous install (if they exist)
    if [ `ls -1 $WD/zios/db |wc -l` -gt 0 ]
    then
        mkdir -p $WD/%%PKG%%/db
        cp -f $WD/zios/db/* $WD/%%PKG%%/db/
    fi

    if [ `ls -1 $WD/zios/report |wc -l` -gt 0 ]
    then
        mkdir -p $WD/%%PKG%%/report
    fi
	 
    #ensure all data group writable :)
    chmod g+w -R $WD/%%PKG%%    # to ensure new dir has group write.

    #and otad script is group executable
    chmod 774 $WD/*.sh

    #do password update for config.json
    if [ -f $WD/passwordcopy.sh ]; then
	    $WD/passwordcopy.sh $WD/zios $WD/%%PKG%%
	    rm -f $WD/passwordcopy.sh
    fi

    #sym link new app to 'app' with -snf to not dereference symlink and force overwrite.
    #Note this allows 1 point of 'switchover' from old to new.
    ln -snf $WD/%%PKG%% $WD/zios

    #link previous app to 'old_zios' with -snf to not dereference symlink and force overwrite.
    #Note: this will be used for cleanup...
    ln -snf $olddir $WD/old_zios

    # print messages for success....
    echo "reboot is required."
   
#or do the cleanup part of the script ######################
elif [ $1 = "cleanup" ]
then
    echo -n Cleanup...

    echo "in progress"

    # if the old dir exists.....
    if [  -e $WD/old_zios ]
    then
        #ok.. go get the absoute'real' path of the old_zios symlink and store in 'dir' variable
        currDir=$PWD
        cd $WD/old_zios
        pwd -P > input0.$$
        read dir < input0.$$
        rm -f input0.$$         # clean up our temp file
        ##echo "old_zios --> $dir"
        cd $WD/zios
        pwd -P > input0.$$
        read existdir < input0.$$
        rm -f input0.$$               # clean up temp file.
        cd $currDir
        # for safety - NEVER let it remove the working directory (incase old_zios linked to $WD)
        if [ "$dir" != "$WD" ];  then
             # or let it clean up "active" dir....
            if [ "$dir" != "$existdir" ]; then
                # remove the actual directory 
                rm -rf $dir
            fi
        fi
        # remove the actual link "old_zios
        rm -f $WD/old_zios
    fi

    # always do other clean up.....

    # cleanup artifacts from install portion of script
    rm -f $WD/zios_install.sh
    rm -f $WD/otad.sh
    rm -f $WD/%%TAR%%
    rm -rf $WD/$DB_DIR
    rm -f $WD/passwordcopy.sh
  
else
    # was neither install or cleanup...
    echo "bad parmameter, usage: otad.sh <install|cleanup>" >&2
    exit 101
fi

exit 0
