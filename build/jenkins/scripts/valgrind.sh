#!/bin/sh
source ./globaldefs.sh
# Make sure a project root was provided and it really exists
if [ "$1" = "" ] ; then
	echo "ERROR: ZIOS Projects root directory not specified"
	echo "Usage: valgrind.sh <ZIOS_ROOT> "
	exit 1
fi
PROJECT_ROOT=$1

if [ ! -d $PROJECT_ROOT ]; then
	echo "ERROR: ZIOS Projects root directory $PROJECT_ROOT does not exist"
	exit 1	
fi

PROJECT_ROOT=$1

cd ${PROJECT_ROOT}; make PLT=Debug -f maketest.mk; STATUS=$?
if [ $STATUS -ne 0 ]; then
	echo "******************************************************************************"
	echo "*************** Valgrind build FAILED"
	echo "******************************************************************************"
	exit 1
fi

REPORT_DIR=$2/valgrind
if [ -d "${REPORT_DIR}" ]; then
	if [ -e "${REPORT_DIR}/*.*" ]; then
		rm ${REPORT_DIR}/*.*
	fi
else
	mkdir -p ${REPORT_DIR}
fi

echo "************** REPORT DIR IS ********************"
ls -ald $REPORT_DIR
echo "+++++++++++++++++++++++++++++++++++++++++++++++++"


for NEXT_PROJECT in "${ZIOS_PROJECTS[@]}"
do
	runValgrindForProject "${PROJECT_ROOT}" "${NEXT_PROJECT}" "${REPORT_DIR}"
	if [ $? = 1 ]; then
	    echo ""
	    echo "******************************************************************************"
	    echo "****************** ZIOS Release Tests Completed With Errors ******************"
	    echo "******************************************************************************"
	    echo ""
		exit 1
	fi
done