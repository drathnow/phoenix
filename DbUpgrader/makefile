################################################################################
# based on managed file, modified to be generic
# zios makefile
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

-include ../zios.currentRelease.Versions

#define output executable file
OUT_LNK = dbupgrader
OUT_EXE = $(OUT_LNK).$(DBUPGRD_VER)

# modify includes for local includes
INCLUDES += -I../AppIncludes
INCLUDES += -I../BaseClasses
INCLUDES += -I../ZediCommon
INCLUDES += -I../SchemaPersistenceManager

#modify LIBPATH... with local needed paths
LIB_PATH += -L../sharedlibs/$(PLT)

#define Libraries needed by this build here (linking)
LIBS := -lSchemaPersistenceManager
LIBS += -lBaseClasses
LIBS += -lZediCommon 
LIBS += -llog4cplus
LIBS += -lpthread
LIBS += -lcurl
LIBS += -lsqlite3
LIBS += -lfcgi 
LIBS += -ldl
LIBS += -lrt
LIBS += -lz
LIBS += -lcjson

ifneq (,$(filter $(PLT),DebugARM ReleaseARM))
LIBS += -llpc3250
else
LIBS += -ldummy
endif

# All Target (default)
all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_EXE)

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
	cd ../ZediCommon && $(MAKE) PLT=$(PLT) all
	cd ../BaseClasses && $(MAKE) PLT=$(PLT) all
	cd ../SchemaPersistenceManager && $(MAKE) PLT=$(PLT) all

# make sure the output dir exists.
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

# Tool invocations
$(OUT_DIR)/$(OUT_EXE): $(OBJS) $(USER_OBJS) dependents ../$(zios.Versions)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS)
	@echo 'Finished building target: $@'
	@$(MAKE) --no-print-directory post-build

# Other Targets
cleanall: clean
	-cd ../ZediCommon && $(MAKE) PLT=$(PLT) clean
	-cd ../BaseClasses && $(MAKE) PLT=$(PLT) clean
	-cd ../SchemaPersistenceManager && $(MAKE) PLT=$(PLT) clean
	
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_EXE)
	-@echo ' '

post-build:
	ln -sf $(OUT_EXE) $(OUT_DIR)/$(OUT_LNK)
	echo linking $(OUT_LNK) to $(OUT_EXE)

install:
	-$(CP) -d $(PLT)/$(OUT_EXE) $(SHARED_DIR)/
	-$(CP) -d config.json $(SHARED_DIR)/
	-$(CD) $(SHARED_DIR); $(LNS) $(OUT_EXE) zios
	
.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build

deploy: all
	-$(CP) -d $(PLT)/$(OUT_EXE) $(DEPLOY_DIR)/
	-$(CP) -d config.json $(DEPLOY_DIR)/
	
copyarm:
ifeq ($(PLT), $(filter $(PLT),DebugARM ReleaseARM))
ifndef REMOTE_ZIOS_HOME
	@echo '*** ERROR: Unable to copy: REMOTE_ZIOS_HOME is not set'
else
	@echo 'Copying: $(PLT)/$(OUT_EXE) to $(REMOTE_ZIOS_HOME)/zios/$(OUT_EXE)'
	@SSH_AUTH_SOCK=0 scp $(PLT)/$(OUT_EXE)  $(REMOTE_ZIOS_HOME)/zios/$(OUT_EXE)
endif
else
	@echo 'Target copyarm only works with DebugARM'
endif
