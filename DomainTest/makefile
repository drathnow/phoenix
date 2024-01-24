-include ../build/Makeenv.mk

#define outputLIB file

OUT_EXE = TestRunner

INCLUDES += -I../AppIncludes
INCLUDES += -I../Domain
INCLUDES += -I../ZediCommon
INCLUDES += -I../ZediCommonTest
INCLUDES += -I../zhal
INCLUDES += -I../zhaltest

#define Libraries needed by this build here (linking)
LIBS += -lziosdomain
LIBS += -lZediCommon
LIBS += -llog4cplus 
LIBS += -lgtest 
LIBS += -lgmock 
LIBS += -lpthread
LIBS += -ldl

# All Target (default)
all: $(OUT_DIR) dependents ./output/TestRunner

# Add inputs and outputs from these tool invocations to the build variables 

dependents: 
	cd ../Domain && $(MAKE)
	cd ../ZediCommon && $(MAKE)

# make sure the output dir exists.
$(OUT_DIR):
	echo "OBJS: $(OBJS)"
	$(MKDIR) $(OUT_DIR)

# Tool invocations
$(OUT_DIR)/$(OUT_EXE): $(OBJS) $(USER_OBJS) dependents
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS)
	@echo 'Finished building target: $@'

# Other Targets
cleanall: clean
	-cd ../ZediCommon && $(MAKE) clean
	-cd ../Domain && $(MAKE) clean

runtest: $(OUT_DIR)/$(OUT_EXE)
	LD_LIBRARY_PATH="../sharedlibs/output" ./output/$(OUT_EXE)

clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_EXE)
	-@echo ' '

# no post build for this project
post-build:

.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build
