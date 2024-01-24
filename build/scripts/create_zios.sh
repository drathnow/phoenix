#!/bin/sh
# : sanity checks
#set -x
if [ $# -ne 9 ]; then
# Strip is optional, so 7 is also OK.
if [ $# -ne 8 ]; then
    echo "Usage: `basename $0` <zios version> <networkManager ver> <watchdogmanager ver> <deployment dir> <deployment package> <db dir> <ZIOS HOME>  <TAR file> [<STRIP CMD>]" >&2
    exit 1
fi
fi

#
# This var contains pattern that will be excluded when copying lib* files
EXCLUDE_PATTERN="(dummy)"

#extract the version # for zios executable from command line
ZIOS_VER=$1
shift
NET_VER=$1
shift
WD_VER=$1
shift

#extract other information from command line
PKGDIR=$1					# product package dir 
PKG=$2
PKGSUB=$PKGDIR/$2
DBDIR=$3
# pass through ENV variables to be set...

export ZIOS_HOME=$4
export TARFILE=$5
# if STRIP not there then define it as ""
if [ $# -ne 8 ]; then
  export STRIP=$6
else
  export STRIP=""
fi

#echo "********************************"
#echo PKG=$PKG
#echo PKGDIR=$PKGDIR
#echo PKGSUB=$PKGSUB
#echo ZIOS_VER=$ZIOS_VER
#echo NET_VER=$NET_VER
#echo WD_VER=$WD_VER
#echo "********************************"


# sanity checks on env variables
if [ -z "$ZIOS_HOME" ]; then
    echo "Environment variable ZIOS_HOME is undefined." >&2
    exit 1
fi
if [ ! -d "$ZIOS_HOME" ]; then
    echo "Environment variable ZIOS_HOME points to a non-existant directory." >&2
    exit 1
fi

echo "********************************"
echo "Creating ZIOS package in $PKGDIR"
echo "********************************"
 
# : initialization (load up helper files)
set -e
. $ZIOS_HOME/build/scripts/createImageFuncs.sh

# : create any required sub - directories
# none yet

# copy over the zios application
copySrc zios/$PLT/zios.$ZIOS_VER $PKGSUB      #application 
copySrc zios/$PLT/zios $PKGSUB                #application link

# copy over the netmanager application
copySrc NetworkManager/$PLT/networkmanager.$NET_VER $PKGSUB       #application 
copySrc NetworkManager/$PLT/networkmanager $PKGSUB                #application link

# copy over the watchdog application
copySrc Watchdog/$PLT/watchdog.$WD_VER $PKGSUB              #application 
copySrc Watchdog/$PLT/watchdog $PKGSUB                      #application link

# copy over the modemtester application
copySrc ModemTester/$PLT/modemtester.$NET_VER $PKGSUB       #application 
copySrc ModemTester/$PLT/modemtester $PKGSUB                #application link

# copy over the IOBoardtester application
copySrc IOBoardTester/$PLT/IOBoardtester.$ZIOS_VER $PKGSUB       #application 
copySrc IOBoardTester/$PLT/IOBoardtester $PKGSUB                #application link

# Copy over ALL *.libs, excluding files that match EXCLUDE_PATTERN
for fl in `ls sharedlibs/$PLT/lib*`; do
    if [ ! $(echo $fl | grep -E "($EXCLUDE_PATTERN)") ];
    then
        copySrc $fl $PKGSUB
    fi
done

# : strip out debug info from binaries if requested
if [ "" != "$STRIP" ]; then
  stripBinaries $PKGSUB
fi

#copy over web content
mkdir -p $PKGSUB/www/html               #webserver dir
echo Copying ZiosWeb/$PLT/WebDeploy to $PKGSUB/www/html
cp -a  $ZIOS_HOME/ZiosWeb/$PLT/WebDeploy/* $PKGSUB/www/html


echo "Copying support..."
copySrc build/content/start.sh $PKGSUB
copySrc build/content/config.json $PKGSUB
copySrc build/content/zios.properties $PKGSUB
copySrc build/content/networkmanager.properties $PKGSUB
copySrc build/content/networkmanager.conf $PKGSUB
copySrc build/content/networkproviders.conf $PKGSUB
copySrc build/content/watchdog.properties $PKGSUB
copySrc build/content/watchdog.conf $PKGSUB
copySrc build/content/modemtester.properties $PKGSUB
copySrc build/content/runmodemtester.sh $PKGSUB
copySrc build/content/IOBoardtester.properties $PKGSUB
copySrc build/content/runIOBoardtester.sh $PKGSUB
chmod 775 $PKGSUB/start.sh

#temporary stuff:
mkdir -p $PKGSUB/devscripts
copySrc build/content/devscripts/zstart.sh $PKGSUB/devscripts             # backdoor shell to start zios only
copySrc build/content/devscripts/netstart.sh $PKGSUB/devscripts           # backdoor shell to start networkmanager only
copySrc build/content/devscripts/wdstart.sh $PKGSUB/devscripts            # backdoor shell to start watchdog 
chmod 775 $PKGSUB/devscripts/netstart.sh
chmod 775 $PKGSUB/devscripts/wdstart.sh
chmod 775 $PKGSUB/devscripts/zstart.sh

# alter the install script(s) (otad and factory 1st time install) and copy over...
alterScript build/scripts/otad.tmpl.sh $PKGDIR/otad.sh
alterScript build/scripts/zios_install.tmpl.sh $PKGDIR/zios_install.sh
chmod 775 $PKGDIR/otad.sh
chmod 775 $PKGDIR/zios_install.sh
copySrc build/scripts/passwordcopy.sh $PKGDIR/passwordcopy.sh
chmod 775 $PKGDIR/passwordcopy.sh


#create the OTAD properties file
echo "zios.ver=$ZIOS_VER" > $PKGDIR/otad.properties
echo "net.ver=$NET_VER" >> $PKGDIR/otad.properties
echo "wd.ver=$WD_VER" >> $PKGDIR/otad.properties
echo "pkg.dir=$PKG" >> $PKGDIR/otad.properties

#copy over initial db dir
mkdir -p $PKGDIR/$DBDIR
chmod g+w $PKGDIR/$DBDIR            # alter so figadmin can delete it and files.
copySrc build/content/dbinit/initdb.sh $PKGDIR/$DBDIR
chmod 775 $PKGDIR/$DBDIR/initdb.sh

# add group write to all files/dir
chmod -R g+w $PKGSUB              


