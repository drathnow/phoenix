-include ../build/Makeenv.mk

#define outputLIB file
OUT_EXE = TestRunner

# modify includes for local includes
INCLUDES += -I../AppIncludes
INCLUDES += -I../lpc3250
INCLUDES += -I../ZediCommon
INCLUDES += -I../zhal

#define Libraries needed by this build here (linking)
LIBS += -llog4cplus 
LIBS += -llpc3250 
LIBS += -lZediCommon 
LIBS += -lgmock
LIBS += -lgtest
LIBS += -lgpiod
LIBS += -lpthread
LIBS += -lrt
LIBS += -lcjson

# All Target (default)
all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_EXE)

# Add inputs and outputs from these tool invocations to the build variables 

dependents: 
	cd ../ZediCommon && $(MAKE)
	cd ../lpc3250 && $(MAKE)

# make sure the output dir exists.
$(OUT_DIR):
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
	-cd ../lpc3250 && $(MAKE) clean

runtest: $(OUT_DIR)/$(OUT_EXE)
	LD_LIBRARY_PATH="../sharedlibs/output" ./output/$(OUT_EXE)

clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_EXE)
	-@echo ' '

# no post build for this project
post-build:

.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build
