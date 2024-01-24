O_SRCS := 
CPP_SRCS := 
C_UPPER_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
CXX_SRCS := 
C++_SRCS := 
CC_SRCS := 
OBJS := 
C++_DEPS := 
C_DEPS := 
CC_DEPS := 
LIBRARIES := 
CPP_DEPS := 
CXX_DEPS := 
C_UPPER_DEPS := 

# Every subdirectory with source files must be described here
SUBDIRS := \
. \
#  
# Build the list of source to compile. We exclude all files *Ex.c and *Ex.cpp for release
# builds.  These are experimental files that need not be bundled with a release build.
#
ifeq ($(PLT),$(filter $(PLT),Release ReleaseARM))
CPP_SRCS = $(filter-out *Ex.cpp, $(wildcard *.cpp))
C_SRCS = $(wildcard *.c)
else
CPP_SRCS = $(filter-out quicktest.cpp, $(wildcard *.cpp))
C_SRCS = $(wildcard *.c)
endif

OBJS = $(patsubst %.cpp, $(OUT_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.o, $(C_SRCS))

CPP_DEPS = $(patsubst %.cpp,$(OUT_DIR)/%.d, $(CPP_SRCS))
C_DEPS = $(patsubst %.c, $(OUT_DIR)/%.d, $(C_SRCS))


