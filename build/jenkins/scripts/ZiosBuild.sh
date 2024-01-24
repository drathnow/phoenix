#!/bin/sh
# ZiosBuild.sh - Build, Test, and Deploy ZIOS 
#
# This script will perform a Release build of ZIOS and run all tests.  If the tests
# pass, a deployment kit will be created from a ReleaseARM build.  The name of the kit 
# will depend on which environment this script is run under.  That name will be determined 
# by the underlying make files that do the actual work of creating the deployment kit.
#
#   $1 - ZIOS source root
#
#	#2 - Report diretory 
#
#@history
#$endhistory

#set -x
source ./globaldefs.sh

#
# Make sure a project root was provided and it really exists
#
if [ "$1" = "" ] ; then
	echo "ERROR: ZIOS Projects root directory not specified"
	echo "Usage: testrunner.sh <ZIOS_ROOT> [Release|Debug] [deploy]"
	exit 1
fi
PROJECT_ROOT=$1

if [ ! -d $PROJECT_ROOT ]; then
	echo "ERROR: ZIOS Projects root directory $PROJECT_ROOT does not exist"
	exit 1	
fi

REPORT_DIR=$2
if [ ! -d "${REPORT_DIR}" ]; then
	mkdir -p "${REPORT_DIR}"
fi

if [ ! -d "${REPORT_DIR}/gtest" ]; then
	mkdir "${REPORT_DIR}/gtest"
fi

#
# Build the Test projects for the given configuration
#

cd ${PROJECT_ROOT}; make -f maketest.mk PLT=Release all; STATUS=$?
if [ $STATUS -ne 0 ]; then
	echo "******************************************************************************"
	echo "*************** Release build FAILED."
	echo "******************************************************************************"
	exit 1
fi

#run the tool to create shortcuts in the sharedlibs dir...
cd ${PROJECT_ROOT}/sharedlibs/Release; /sbin/ldconfig -n .

#
# Iterate over the list and run tests for each project.  A failure will scuttle the
# entire operation
#
for NEXT_PROJECT in "${ZIOS_PROJECTS[@]}"
do
	runTestsForProject "$PROJECT_ROOT" $NEXT_PROJECT Release "$REPORT_DIR/gtest"
	if [ $? != 0 ]; then
	    echo ""
	    echo "******************************************************************************"
	    echo "********************** ZIOS Tests Completed With Errors **********************"
	    echo "******************************************************************************"
	    echo ""
		exit 1
	fi
done

#
# Good so far. Now build and deploy ARM
#		
cd ${PROJECT_ROOT}; make PLT=ReleaseARM deploy; STATUS=$?
if [ $STATUS != 0 ]; then
	echo "******************************************************************************"
	echo "*************** ReleasARM deploy build FAILED "
	echo "******************************************************************************"
	exit 1
fi

