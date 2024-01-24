#!/bin/sh
# : sanity checks
# set -x
if [ $# -ne 4 ]; then
    echo "Usage: `basename $0` <root files dir>  <deployment dir> <ZIOS HOME> <TAR FILE>" >&2
    exit 1
fi

#extract other information from command line
ROOTDIR=$1					# source root dir 
PKGDIR=$2   	  		    # target dir
PKGSUB=$PKGDIR              # image 
PKG=$2                      # to keep alterscript happy
export ZIOS_HOME=$3         # ZIOS base dir
TARFILE=$4                 # to keep alterscript happy

# : initialization (load up helper files)
set -e
. $ZIOS_HOME/build/scripts/createImageFuncs.sh

# : create any required sub - directories
# none yet


#check if pkgdir already exists
if [ -d $PKGSUB]; then
  rm -rf $PKGSUB/*
fi


# copy over ALL files
for fl in `ls $ROOTDIR/*`; do
copySrc $fl $PKGSUB	
done
copySrc $ROOTDIR/.profile $PKGSUB

#remove files not wanted:
rm -f $PKGSUB/copyrootfs.sh  # this is a one time install file.
rm -f $PKGSUB/finalize.sh   # this is a one time install file.
rm -f $PKGSUB/*~             # just to ensure no ~ files.....

# alter the install script(s) (otad and factory 1st time install) and copy over...
alterScript  build/scripts/rdir_otad.tmpl.sh $PKGSUB/otad.sh
chmod 755 $PKGDIR/otad.sh



