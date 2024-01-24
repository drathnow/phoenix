#rem "source" file: ". ./setup.sh" 
export LD_LIBRARY_PATH=$PWD/sharedlibs/Debug:/usr/local/bin
#add sharedlibs to path to allow simulator tools to run without fully qualifying path
export PATH=$PWD/sharedlibs/Debug:$PATH
