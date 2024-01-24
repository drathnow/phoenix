#
#@history
# Created Sep 01, 2013 by eganb: maybe
# IncrDev Oct 08, 2013 by eganb: changed script to build all and remove unbuildable parts instead of multiple build arguments
# IncrDev Jan 23, 2014 by eganb: added in removal of output stuff
# IncrDev Mar 31, 2014 by eb:	refactored using manual managed build files...
# IncrDev oct 23, 2014 by eb: removed depenancy on ZIOS dir
# IncrDev Nov 12, 2014 by eb: change NetManager to NetworkManager
#@endhistory

#set -x
if [ "$1" = "" ] ; then
  echo "ERROR: projects root directory not specified"
  echo "      usage: headlessBuild.sh <projdir>"
  exit 1   
fi

# remove all shared library files
rm -rf $1/sharedlibs

# remove all output directories (start with Debug or Release)
find $1 -name Debug* |grep -v sharedlibs
find $1 -name Release* |grep -v sharedlibs
find $1 -name Debug* |grep -v sharedlibs |xargs rm -rf
find $1 -name Release* |grep -v sharedlibs |xargs rm -rf

# Build does not yet include projects:
# so:
# OTAD
# PassThrough
# WatchDogManager
#----------------
# Unit Tests:
# All
#---------------
# Exe:
# WDogManager


#build all platform builds with exit on errors of each make.
(cd $1/zios; make PLT=Debug all)
if [ $? -ne 0 ]; then
  echo "*************** ZIOS Debug build FAIL  ******************************************"
  exit 1
fi
(cd $1/NetworkManager; make PLT=Debug all)
if [ $? -ne 0 ]; then
  echo "*************** NetworkManager Debug build FAIL  ******************************************"
  exit 1
fi
(cd $1/zios; make PLT=DebugARM all)
if [ $? -ne 0 ]; then
  echo "*************** ZIOS DebugARM build FAIL  ******************************************"
  exit 1
fi
(cd $1/NetworkManager; make PLT=DebugARM all)
if [ $? -ne 0 ]; then
  echo "*************** NetworkManager DebugARM build FAIL  ******************************************"
  exit 1
fi
(cd $1/zios; make PLT=Release all)
if [ $? -ne 0 ]; then
  echo "*************** ZIOS Release build FAIL  ******************************************"
  exit 1
fi
(cd $1/NetworkManager; make PLT=Release all)
if [ $? -ne 0 ]; then
  echo "*************** NetworkManager Release build FAIL  ******************************************"
  exit 1
fi
(cd $1/zios; make PLT=ReleaseARM all)
if [ $? -ne 0 ]; then
  echo "*************** ZIOS ReleaseARM build FAIL  ******************************************"
  exit 1
fi
(cd $1/NetworkManager; make PLT=ReleaseARM all)
if [ $? -ne 0 ]; then
  echo "*************** NetworkManager ReleaseARM build FAIL  ******************************************"
  exit 1
fi

