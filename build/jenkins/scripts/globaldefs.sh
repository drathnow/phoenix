#
# These are the current projects for which we will run our test
#
ZIOS_PROJECTS=(ZediCommon \
       BaseClasses \
       Session \
       ConnectionManager \
       CommandHandler \
       MQWrapper \
       EventHandler \
       Modbus \
       RealTimeEngine \
       SchemaPersistenceManager \
       Reporting \
       ZiosWeb \
       Ports \
       PassThrough \
       zioslib \
       Scheduling \
       LE910B1ModemDriver \
       NetworkManagerCommon \
       NetworkMonitorLib \
       WatchdogLib \
       lpc3250 \
       OTAD)


#
# runTestsForProject - Run tests for individual project
#
#     $1 - Project root. Root directory where the all project source code lives
#
#   $2 - Project name.  This should be the name of the project, without the Test suffix
#        or prefix.  This function will find the test project by prefixing(or suffixing) the
#        project name with "Test" and searching for the directory in PROJECT_ROOT.
#        if that project is not found, it will suffix the name with "Test" and search
#        for that project
#
#   $3 - Environment name (Relase or Debug)
#
#    $4 - Report directory where gtest reports will be written. This function will append "gtest_results"
#          onto this argument and will first delete the directory before recreating it.
#
runTestsForProject() {
    PROJECT_ROOT=$1
    PROJECT_NAME=$2
    ENV=$3
    
    if [ -e "${PROJECT_ROOT}/Test${PROJECT_NAME}" ]; then
        TEST_PROJECT_NAME="Test${PROJECT_NAME}"
    elif [ -e "${PROJECT_ROOT}/${PROJECT_NAME}Test" ]; then
        TEST_PROJECT_NAME="${PROJECT_NAME}Test"
    else
        echo "No test directory found for project ${PROJECT_NAME}"
        return 1
    fi

    if [ "$4" = "" ]; then
        echo "ERROR: No report directory specified"
        return 1
    fi     
    
    if [ ! -d "$4" ]; then
        echo "ERROR: Report directory does not exist"
        return 1
    fi
        
    RPT_FILE="$4/${TEST_PROJECT_NAME}.xml"
	if [ -e "$RPT_FILE" ]; then
		rm $RPT_FILE
	fi
                
    export LD_LIBRARY_PATH=${PROJECT_ROOT}/sharedlibs/${ENV}:/usr/local/lib
    
    echo ""
    echo "******************************************************************************"
    echo "Running tests for project ${PROJECT_NAME}"
    echo "******************************************************************************"
    echo ""
	pushd ${PROJECT_ROOT}/${TEST_PROJECT_NAME};./${ENV}/${TEST_PROJECT_NAME} --gtest_output=xml:${RPT_FILE};STATUS=$?; popd
    if [ $STATUS -ne 0 ]; then
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "${PROJECT_NAME} Tests FAILED"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        return 1
    else
        echo ""
        echo "******************************************************************************"
        echo "${PROJECT_NAME} Tests Completed Successfully"
        echo "******************************************************************************"
    fi
    echo ""
    return 0
}

#
# Runs valgind on a project
#
#     $1 - Project root. Root directory where the all project source code lives
#
#   $2 - Project name.  This should be the name of the project, without the Test suffix
#        or prefix.  This function will find the test project by prefixing(or suffixing) the
#        project name with "Test" and searching for the directory in PROJECT_ROOT.
#        if that project is not found, it will suffix the name with "Test" and search
#        for that project
#
#    $3 - Report directory
#
runValgrindForProject() {    
    PROJECT_ROOT=$1
    PROJECT_NAME=$2
    
    if [ ! -d "${PROJECT_ROOT}" ]; then
        echo "Project directory does not exist or was not specified"
        return 1;
    fi
                
    if [ -e "${PROJECT_ROOT}/Test${PROJECT_NAME}" ]; then
        TEST_PROJECT_NAME="Test${PROJECT_NAME}"
    elif [ -e "${PROJECT_ROOT}/${PROJECT_NAME}Test" ]; then
        TEST_PROJECT_NAME="${PROJECT_NAME}Test"
    else
        echo "No test directory found for project ${PROJECT_NAME} in project root ${PROJECT_ROOT}"
        return 1
    fi
    

    if [ "$3" = "" ]; then
        echo "ERROR: No report directory specified"
        return 1
    fi     
    
    if [ ! -d $3 ]; then
        echo "ERROR: Report directory $3 does not exist"
        return 1
    fi
    RPT_DIR="$3"
    
    LD_LIBRARY_PATH=${PROJECT_ROOT}/sharedlibs/Debug:/usr/local/lib
    cd ${PROJECT_ROOT}/${TEST_PROJECT_NAME}; valgrind --tool=memcheck \
            --child-silent-after-fork=yes \
            --quiet \
            --leak-check=full \
            --xml=yes \
            --xml-file=${RPT_DIR}/${PROJECT_NAME}_memcheck.xml \
            ${PROJECT_ROOT}/${TEST_PROJECT_NAME}/Debug/${TEST_PROJECT_NAME}; $STATUS=$? 
    if [ $STATUS -ne 0 ]; then
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "${PROJECT_NAME} Tests FAILED"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        return 1
    else
        echo ""
        echo "******************************************************************************"
        echo "${PROJECT_NAME} Tests Completed Successfully"
        echo "******************************************************************************"
    fi
    echo ""
    return 0
}