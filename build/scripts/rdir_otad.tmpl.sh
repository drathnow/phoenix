#!/bin/sh

# @file rdir_otad.tmpl.sh
#
# @copyright
# Copyright 2015, Zedi Inc.  All rights reserved.
# This file contains confidential intellectual property of Zedi Inc.
#
# @details
# this is the "OTAD" script to update the root dir
# it is meant to be used to sync up the root dir scripts
# invocation is root-otad.sh <URL of file>
#
# @HISTORY
# Created Sep 09, 2015 by eganb:
# @ENDHISTORY
#

export WD=/zioshome
OUT_FILE=$WD/ZIOS.tbz2
TAR_FILE=$WD/newroot.tbz2
OTAD_LOG="$WD/otad.log"


   echo Install | tee -a $OTAD_LOG

    # check for either the generic file (run from root-otad.sh) or specific file (just run by self)
    if [ ! -f $OUT_FILE ]; then
       if [ ! f $TAR_FILE ]; then
          echo "ERROR: could not find generic TAR file $OUT_FILE or specific TAR FILE $TAR_FILE "| tee -a $OTAD_LOG
          exit 2
       fi
    else
        # rename generic file back to otad tar name :)
        echo "renaming $OUT_FILE  to $TAR_FILE..." | tee -a $OTAD_LOG
        mv $OUT_FILE $TAR_FILE
    fi

  
   #extract files to /root
   echo "Extracting new root scripts...." >> $OTAD_LOG
   tar -xjvf $TAR_FILE -C /root --no-same-owner --exclude='otad.sh'        # extract files but don't take this file.
   if [ $? -ne 0 ]; then
    echo >&2 "Extraction failed"  >> $OTAD_LOG
    rm -f $WD/$TAR_FILE
    exit 4;
   else
     chmod -R g+w /root     # make sure all files are writable by group.
   fi

   rm -rf $WD/otad.sh       # remove this file
   rm -rf $TAR_FILE         # cleanup tar file


