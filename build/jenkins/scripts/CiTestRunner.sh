#!/bin/sh
# CiTestRunner.sh - Continuous Integration Test Runner
#
#   $1 - ZIOS source root
#
#	$2 - Report directory 
#

#set -x
source ./globaldefs.sh

#
# Make sure a project root was provided and it really exists
#
if [ "$1" = "" ] ; then
	echo "ERROR: ZIOS Projects root directory not specified"
	echo "Usage: testrunner.sh <ZIOS_ROOT> <REPORT_DIR>"
	exit 1
fi
PROJECT_ROOT=$1

if [ ! -d $PROJECT_ROOT ]; then
	echo "ERROR: ZIOS Projects root directory $PROJECT_ROOT does not exist"
	exit 1	
fi

if [ "$1" = "" ] ; then
	echo "ERROR: Reporting  directory not specified"
	echo "Usage: testrunner.sh <ZIOS_ROOT> <REPORT_DIR>"
	exit 1
fi

REPORT_DIR=$2/gtest
if [ -d "${REPORT_DIR}" ]; then
	if [ -e "${REPORT_DIR}/*.*" ]; then
		rm ${REPORT_DIR}/*.*
	fi
else
	mkdir -p ${REPORT_DIR}
fi

#
# First thing to do is delete all the shared libraries
#
find $1 -name sharelibs |xargs rm -rf

#
# Build the Test projects for the given configuration
#
cd ${PROJECT_ROOT}; make -f maketest.mk PLT=Release all; STATUS=$?
if [ $STATUS -ne 0 ]; then
	echo "******************************************************************************"
	echo "*************** Release build FAILED. No tests will be run"
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
	runTestsForProject "$PROJECT_ROOT" $NEXT_PROJECT Release "$REPORT_DIR"
	if [ $? = 1 ]; then
	    echo ""
	    echo "******************************************************************************"
	    echo "****************** ZIOS Release Tests Completed With Errors ******************"
	    echo "******************************************************************************"
	    echo ""
		exit 1
	fi
done

echo ""
echo "******************************************************************************"
echo "****************** ZIOS Release Tests Completed Sucessfully ******************"
echo "******************************************************************************"
echo ""

echo ""
echo "******************************************************************************"
echo "************************* Building DebugARM **********************************"
echo "******************************************************************************"
echo ""
		
cd ${PROJECT_ROOT}; make PLT=DebugARM; STATUS=$?
if [ $STATUS -ne 0 ]; then
	echo "******************************************************************************"
	echo "*************** ReleaseARM build FAILED "
	echo "******************************************************************************"
	exit 1
fi

echo ""
echo "******************************************************************************"
echo "************************* DebugARM Build Successful **************************"
echo "******************************************************************************"
echo ""
