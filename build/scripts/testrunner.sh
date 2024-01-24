#!/bin/bash
# testrunner.sh
#
#   $1 - ZIOS source root
#   $2 - Configuration to build and run tests for.  Options are:
#		Debug - (Default) Will build Default and tests 
#		Release - (Release) Will build Release run tests
#	$3 - If set to "test", will not delete existing object and shared library directories.
#        Will run build scripts and then tests
#
#@history
# Created Sep 01, 2013 by eb: maybe
# IncrDev Apr 01, 2014 by eb: added in test make
# IncrDev jul 31, 2014 by eb: added in more test calls
# IncrDev oct 23, 2014 by eb: removed depenancy on ZIOS dir
# IncrDev jul 07, 2015 by aj: added WatchdogLibTest
# IncrDev nov 25, 2015 by eb: added check for non full erase/build
# IncrDev Feb 16, 2016 by aj: added OTADTest
#$endhistory



#
# Function to run tests for a spacific project.
#
#   $1 - Project name.  This should be the name of the project, without the Test suffix
#        or prefix.  This function will find the test project by prefixing(or suffixing) the
#        project name with "Test" and searching for the directory in PROJECT_ROOT.
#        if that project is not found, it will suffix the name with "Test" and search
#        for that project
#
runTestsForProject() {
    projectName=$1

    if [ -e "${PROJECT_ROOT}/${projectName}Test" ]; then
        testProjectName="${projectName}Test"
    else
        echo "No test directory found for project ${projectName}"
        return 0
    fi

    echo ""
    echo "******************************************************************************"
    echo "Running tests for project ${projectName}"
    echo "******************************************************************************"
    echo ""
    cd ${PROJECT_ROOT}/${testProjectName};./${CONFIG}/${testProjectName} --gtest_output=xml:./${CONFIG}/${testProjectName}.xml
    echo ""
    echo "******************************************************************************"
    if [ $? -ne 0 ]; then
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "${projectName} Tests FAILED"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        return 1
    else
        echo "${projectName} Tests Completed Successfully"
    fi
    echo "******************************************************************************"
    echo ""
    return 0
}

if [ "$1" = "" ] ; then
	echo "ERROR: ZIOS Projects root directory not specified"
	exit 1
fi

PROJECT_ROOT=$1

if [ "$2" = "" ] ; then
	CONFIG="Debug"
else
	CONFIG="$(tr [A-Z] [a-z] <<< "$2")"
	case ${CONFIG} in 
	    release) CONFIG="Release";;
	    debug) CONFIG="Debug";;
	    *) CONFIG="";
	esac
fi

if [ "CONFIG" = "" ] ; then
	echo "ERROR: Invalid configuration.  Must be Debug or Release"
	exit 1
fi

#
# Delete all the shared libraries so we start with a clean slate
#
TEST_ONLY="$(tr [A-Z] [a-z] <<< "$3")"
if [ "$3" != "test" ]; then
	rm -rf ${PROJECT_ROOT}/sharedlibs
	find ${PROJECT_ROOT} -name ${CONFIG} |grep -v sharedlibs |xargs rm -rf

    #
    # Build representive Test platform builds with exit on errors of each make.
    #
    cd ${PROJECT_ROOT}; make -f maketest.mk PLT=${CONFIG} all
    if [ $? -ne 0 ]; then
        echo "******************************************************************************"
        echo "*************** ${CONFIG} build FAILED *******************************************"
        echo "******************************************************************************"
        exit 1
    fi

fi

# check for adding sharedlibs more than once.
envtest=$(env|grep sharedlibs)
if [ -z $envtest ] ; then
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PROJECT_ROOT}/sharedlibs/${CONFIG}:/usr/local/lib
fi

#run the tool to create shortcuts in the sharedlibs dir...
cd ${PROJECT_ROOT}/sharedlibs/${CONFIG}; /sbin/ldconfig -n .

rc=0

if runTestsForProject "ZediCommon" &&
   runTestsForProject "BaseClasses" &&
   runTestsForProject "Session" &&
   runTestsForProject "Session" &&
   runTestsForProject "CommandHandler" &&
   runTestsForProject "ConnectionManager" &&
   runTestsForProject "MQWrapper" &&
   runTestsForProject "EventHandler" &&
   runTestsForProject "Modbus" &&
   runTestsForProject "RealTimeEngine" &&
   runTestsForProject "SchemaPersistenceManager" &&
   runTestsForProject "Reporting" &&
   runTestsForProject "ZiosWeb" &&
   runTestsForProject "Ports" &&
   runTestsForProject "PassThrough" &&
   runTestsForProject "zioslib" &&
   runTestsForProject "Scheduling" &&
   runTestsForProject "NetworkManagerCommon" &&
   runTestsForProject "NetworkMonitorLib" &&
   runTestsForProject "WatchdogLib" &&
   runTestsForProject "Hardware" &&
   runTestsForProject "OTAD"; then
    echo ""
    echo "******************************************************************************"
    echo "******************* All ZIOS Tests Completed Successfully ********************"
    echo "******************************************************************************"
    echo ""
else
    echo ""
    echo "******************************************************************************"
    echo "********************** ZIOS Tests Completed With Errors **********************"
    echo "******************************************************************************"
    echo ""
fi
