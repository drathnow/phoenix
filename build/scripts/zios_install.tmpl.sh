#!/bin/sh
# @file zios_install.tmpl.sh
#
# @copyright
# Copyright 2015, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# Template used to generate a script  to facitilate install of cnct on device.
#
# The template employs token substiution: the token %%PKG%% is replaced with the actual package 
#   name and %%TAR%% with tar file name when the install.sh is created.
#
#  script usage: install {filename.tbz2}
#
#  General functionality: 
#     remove any existing application trace
#     install application 
#     install system configuration including etherenet setup and dropbear config
#
# @HISTORY
# Created Mar 12, 2015 by eganb: based on file from zedpac
# IncrDev Apr 23, 2015 by eganb: changed to TAR from PKG for tar file
# IncrDev Sep 10, 2015 by eganb: added initaliziation of db for ports script
# IncrDev Apr 15, 2016 by eganb: add remove of passwordcopy script
# @ENDHISTORY
#

#
# First and foremost, this version CANNOT be installed on any kernel version less than 5.
#
KERNEL_VERSION=$(uname -r | awk '{split($0,a,"."); print a[1]}')
if [ $KERNEL_VERSION -lt 5 ];
then
    echo "ZIOS version 2 or higher cannot be installed on this Linux version" 
    return 1
fi

#define internal variables (for boot hooks)
startLink=startupZIOS
stopLink=shutdownZIOS

#define our database subdir     
DB_DIR=dbinit

# set up the Working Directory
export WD=/zioshome

echo Install

#ensure the app is gone.
if [ -d $WD/zios ]
then
    rm -rf $WD/zios
fi

#clean up any oldapp stuff
if [ -d $WD/old_zios ]
then
  currDir=$PWD  #save the current pwd for later
  # get the actual absolute path of the 'app' directory --> olddir
  cd $WD/old_zios
  pwd -P > input0.$$
  read olddir < input0.$$
  rm -f input0.$$               # clean up temp file.
  ##echo "old dir --> $olddir"
  cd $currDir
  #toast any old application symbolic links.
  rm -f $WD/old_zios
  #remove the actual old dir if there
  rm -rf $WD/$olddir
fi 

#extract new app, link current app to old_app link, update state, and link new app to current directory
tar -xjvf $WD/%%TAR%% -C $WD  --no-same-owner


#cp -f $WD/linux-os/passwd /etc #copy over the passwd file (check with Farren)

#link new app to 'app' with -snf to not dereference symlink and force overwrite.
#Note this allows 1 point of 'switchover' from old to new.
ln -snf $WD/%%PKG%% $WD/zios


#install the boot hooks: (use double indirection to allow for easy development disabling)
ln -snf $WD/zios/start.sh /etc/$startLink
#ln -snf $WD/zios/shutdown.sh /etc/$stopLink
	 
# setup our mfg db file....
$WD/$DB_DIR/initdb.sh $DB_DIR %%PKG%%
INITDB=$?

#cleanup
rm $WD/otad.sh
rm $WD/%%TAR%%
rm $WD/zios_install.sh
rm -rf $WD/$DB_DIR
rm -f $WD/passwordcopy.sh

#make sure everything is group writable :)
chmod -R g+w $WD

#check our return
if [ ! "$INITDB" = "0" ] ; then
return 1
fi

exit 0
