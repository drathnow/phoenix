# kwbuild.sh
#
#@history
# Created Apr 01, 2014 by eganb: 
# IncrDev oct 23, 2014 by eb: added absolute dir
#@endhistory


if [ "$1" = "" ] ; then
  echo "ERROR: projects root directory not specified"
  echo "      usage: kwbuild.sh <projdir>"
  exit 1   
fi

#build release platform builds
(cd $1/zios;  make PLT=Release all)
if [ $? -ne 0 ]; then
  echo "*************** kw Release build FAIL  ******************************************"
  exit 1
fi

#remove analysis of tests to reduce "fluff"
# echo $PWD  "pwd...."
#now build tests....
# make -f maketest.mk PLT=Release all
# if [ $? -ne 0 ]; then
# echo "*************** kw Test Release build FAIL  ******************************************"
#  exit 1
# fi

