DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source $DIR/../jenkins/scripts/globaldefs.sh

PLT=$1
PROJECT_NAME=$2
ZIOS_ROOT=$DIR/../..
REPORT_DIR=${ZIOS_ROOT}/Reports
LD_LIBRARY_PATH=${ZIOS_ROOT}/sharelibs/$PLT

if [ ! -d "Reports" ]; then
	mkdir $REPORT_DIR
fi        

if [ "$PROJECT_NAME" = "" ] ; then
	for NEXT_PROJECT in "${ZIOS_PROJECTS[@]}"  
	do
		runTestsForProject "${ZIOS_ROOT}" "$NEXT_PROJECT" "$PLT" "$REPORT_DIR"
	done
else
	runTestsForProject "${ZIOS_ROOT}" "${PROJECT_NAME}" "${PLT}" "${REPORT_DIR}"
fi