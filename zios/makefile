-include ../build/Makeenv.mk

-include ../zios.currentRelease.Versions

#define output executable file
OUT_LNK = zios
OUT_EXE = $(OUT_LNK).$(ZIOS_VER)

# modify includes for local includes
INCLUDES += -I../AppIncludes
INCLUDES += -I../RealTimeEngine
INCLUDES += -I../Reporting
INCLUDES += -I../ConnectionManager
INCLUDES += -I../BaseClasses
INCLUDES += -I../ZediCommon
INCLUDES += -I../EventHandler
INCLUDES += -I../Scheduling
INCLUDES += -I../ZiosWeb
INCLUDES += -I../CommandHandler
INCLUDES += -I../Ports
INCLUDES += -I../SchemaPersistenceManager
INCLUDES += -I../PassThrough
INCLUDES += -I../zioslib
INCLUDES += -I../MQWrapper
INCLUDES += -I../Modbus
INCLUDES += -I../WatchdogLib
INCLUDES += -I../MQWrapper
INCLUDES += -I../zhal
INCLUDES += -I../OTAD
INCLUDES += -I../Session

#define Libraries needed by this build here (linking)
LIBS := -lSchemaPersistenceManager
LIBS += -lzioslib
LIBS += -lEventHandler 
LIBS += -lCommandHandler
LIBS += -lConnectionManager
LIBS += -lRealTimeEngine
LIBS += -lPassThrough
LIBS += -lBaseClasses
LIBS += -lWatchdogLib
LIBS += -lScheduling
LIBS += -lZediCommon 
LIBS += -lReporting
LIBS += -lMQWrapper
LIBS += -lZiosWeb
LIBS += -lSession
LIBS += -llog4cplus
LIBS += -lModbus
LIBS += -lPorts
LIBS += -lOTAD
LIBS += -lpthread
LIBS += -lcurl
LIBS += -lsqlite3
LIBS += -lfcgi 
LIBS += -ldl
LIBS += -lrt
LIBS += -lz
LIBS += -lcjson
LIBS += -l$(PLATFORM_LIBRARY)

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
	cd ../dummy && $(MAKE)
	cd ../lpc3250 && $(MAKE)
	cd ../ZediCommon && $(MAKE)
	cd ../BaseClasses && $(MAKE)
	cd ../SchemaPersistenceManager && $(MAKE)
	cd ../Ports && $(MAKE)
	cd ../PassThrough && $(MAKE)
	cd ../ConnectionManager && $(MAKE)
	cd ../Session && $(MAKE)
	cd ../Reporting && $(MAKE)
	cd ../MQWrapper && $(MAKE)
	cd ../RealTimeEngine && $(MAKE)
	cd ../EventHandler && $(MAKE)
	cd ../Scheduling && $(MAKE)
	cd ../ZiosWeb && $(MAKE)
	cd ../CommandHandler && $(MAKE)
	cd ../WatchdogLib && $(MAKE)
	cd ../OTAD && $(MAKE)
	cd ../Modbus && $(MAKE)
	cd ../zioslib && $(MAKE)

# make sure the output dir exists.
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

# Tool invocations
$(OUT_DIR)/$(OUT_EXE): $(OBJS) $(USER_OBJS) dependents ../$(zios.Versions)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS) -Wl,-Map,zios.map
	@echo 'Finished building target: $@'
	@$(MAKE) --no-print-directory post-build

# Other Targets
cleanall: clean
	-cd ../lpc3250 && $(MAKE) clean
	-cd ../ZediCommon && $(MAKE) clean
	-cd ../BaseClasses && $(MAKE) clean
	-cd ../ConnectionManager && $(MAKE) clean
	-cd ../Session && $(MAKE) clean
	-cd ../Reporting && $(MAKE) clean
	-cd ../Ports && $(MAKE) clean
	-cd ../PassThrough && $(MAKE) clean
	-cd ../SchemaPersistenceManager && $(MAKE) clean
	-cd ../RealTimeEngine && $(MAKE) clean
	-cd ../EventHandler && $(MAKE) clean
	-cd ../Scheduling && $(MAKE) clean
	-cd ../ZiosWeb && $(MAKE) clean
	-cd ../CommandHandler && $(MAKE) clean
	-cd ../MQWrapper && $(MAKE) clean
	-cd ../WatchdogLib && $(MAKE) clean
	-cd ../OTAD && $(MAKE) clean
	-cd ../Modbus && $(MAKE) clean
	-cd ../zioslib && $(MAKE) clean

clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_EXE)
	-@echo ' '

post-build:
	ln -sf $(OUT_EXE) $(OUT_DIR)/$(OUT_LNK)
	@echo linking $(OUT_LNK) to $(OUT_EXE)
	-$(CP) -d $(OUT_DIR)/$(OUT_EXE) $(SHARED_DIR)/
	-$(CP) -d config.json $(SHARED_DIR)/
	-$(CP) -d zios.properties $(SHARED_DIR)/
	-$(CD) $(SHARED_DIR); $(LNS) $(OUT_EXE) zios
	
.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build

deploy: all
	-$(CP) -d $(OUT_DIR)/$(OUT_EXE) $(DEPLOY_DIR)/
	-$(CP) -d config.json $(DEPLOY_DIR)/
