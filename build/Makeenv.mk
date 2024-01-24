ifeq ("$(wildcard ../defconfig)","")
  $(error defconfig file was not found)
endif

-include ../defconfig

MV = mv -f
CP = cp
LNS = ln -snf
CD = cd
LS = ls
MKDIR = mkdir -p
RM = rm -f
RMREC = rm -f -r
RMDIR = rmdir
MAKE = make

OUT_DIR = ./output
SHARED_DIR = ../sharedlibs/output

LIB_PATH += -L$(SHARED_DIR)

# Every subdirectory with source files must be described here
SUBDIRS := \
. \

CPP_SRCS = $(filter-out quicktest.cpp, $(wildcard *.cpp))
C_SRCS = $(wildcard *.c)

OBJS = $(patsubst %.cpp, $(OUT_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.o, $(C_SRCS))

CPP_DEPS = $(patsubst %.cpp,$(OUT_DIR)/%.d, $(CPP_SRCS))
C_DEPS = $(patsubst %.c, $(OUT_DIR)/%.d, $(C_SRCS))

# Rules for building files 
# for c++ rules
$(OUT_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) $(DEFINES) $(INCLUDES) $(GCC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#for c rules
$(OUT_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	$(CC)  $(DEFINES) $(INCLUDES) $(CC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
