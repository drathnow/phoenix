################################################################################
# based on managed file, modified to be generic
# ZIOA HAL makefile
################################################################################
ifeq ($(PLT),)
  $(error PLT not defined by make invocation)
endif


# pull in common make pieces
MAKE_DIR=../build/
-include $(MAKE_DIR)makedef.mk
-include $(MAKE_DIR)makerules.mk
-include $(MAKE_DIR)makeenv.mk
-include $(MAKE_DIR)makesource.mk
-include $(MAKE_DIR)makeplt.mk

#define outputLIB file
OUT_SO_LINK = libzhal.so
OUT_SO_NAME = $(OUT_SO_LINK).2
OUT_SO = $(OUT_SO_NAME).0.0

# modify includes for local includes
INCLUDES += -I../AppIncludes
INCLUDES += -I../ZediCommon 

#modify LIBPATH... with local needed paths
LIB_PATH += -L../sharedlibs/$(PLT) 

#define Libraries needed by this build here (linking)
LIBS := -llog4cplus


# All Target (default)
all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

dependents: 

# make sure the output dir exists.
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

#  make sure shared lib exists
$(SHARED_DIR):
	$(MKDIR) $(SHARED_DIR)

# Tool invocations
$(OUT_DIR)/$(OUT_SO): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -shared -Wl,-soname,$(OUT_SO_NAME) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS)
	@echo 'Finished building target: $@'

$(SHARED_DIR)/$(OUT_SO): $(OUT_DIR)/$(OUT_SO)
	@$(MAKE) --no-print-directory post-build

# Other Targets
cleanall: clean

clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_SO) $(OUT_DIR)/$(OUT_SO_LINK) $(OUT_DIR)/$(OUT_SO_NAME)
	#remove the shared dir copy of the so.
	-$(RM) $(SHARED_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO_LINK) $(SHARED_DIR)/$(OUT_SO_NAME)
	-@echo ' '

post-build: $(SHARED_DIR)
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO_NAME) $(OUT_SO_LINK) 
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO) $(OUT_SO_NAME) 
	-$(CP) -d $(OUT_DIR)/lib* $(SHARED_DIR)/
	-@echo ' '

.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build

copyarm:
ifeq ($(PLT), $(filter $(PLT),DebugARM ReleaseARM))
ifndef REMOTE_ZIOS_HOME
	@echo '*** ERROR: Unable to copy: REMOTE_ZIOS_HOME is not set'
else
	@echo 'Copying: $(PLT)/$(OUT_SO) to $(REMOTE_ZIOS_HOME)/zios/$(OUT_SO)'
	@SSH_AUTH_SOCK=0 scp $(PLT)/$(OUT_SO)  $(REMOTE_ZIOS_HOME)/zios
endif
else
	@echo 'Target copyarm only works with DebugARM'
endif


