-include ../build/Makeenv.mk

#define outputLIB file
OUT_SO_LINK = libziosdomain.so
OUT_SO_NAME = $(OUT_SO_LINK).2
OUT_SO = $(OUT_SO_NAME).0.0
MAP_NAME = $(subst .so,,$(OUT_SO)).map

INCLUDES += -I../AppIncludes
INCLUDES += -I../ZediCommon

#define Libraries needed by this build here (linking)
LIBS += -llog4cplus
LIBS += -lcjson 

# All Target (default)
all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO)

# Add inputs and outputs from these tool invocations to the build variables 

#no dependents for this project :)
dependents: 
	cd ../ZediCommon && $(MAKE)

# make sure the output dir exists.
$(OUT_DIR):
	@echo "OBJS: $(OBJS)"
	$(MKDIR) $(OUT_DIR)

#  make sure shared lib exists
$(SHARED_DIR):
	$(MKDIR) $(SHARED_DIR)

# Tool invocations
$(OUT_DIR)/$(OUT_SO): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -shared -Wl,-soname,$(OUT_SO_NAME) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS) -Xlinker -Map=$(OUT_DIR)/$(MAP_NAME)
	@echo 'Finished building target: $@'

$(SHARED_DIR)/$(OUT_SO): $(OUT_DIR)/$(OUT_SO)
	@$(MAKE) --no-print-directory post-build

# Other Targets
cleanall: clean
	-cd ../ZediCommon && $(MAKE) clean
	-cd ../SchemaPersistenceManager && $(MAKE) clean
	
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_SO) $(OUT_DIR)/$(OUT_SO_LINK) $(OUT_DIR)/$(OUT_SO_NAME) $(OUT_DIR)/$(MAP_NAME)
	-$(RM) $(SHARED_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO_LINK) $(SHARED_DIR)/$(OUT_SO_NAME) $(SHARED_DIR)/$(MAP_NAME)
	-@echo ' '

post-build: $(SHARED_DIR)
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO_NAME) $(OUT_SO_LINK) 
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO) $(OUT_SO_NAME) 
	-$(CP) -d $(OUT_DIR)/lib* $(SHARED_DIR)/
	-@echo ' '

.PHONY: all clean dependents cleanall outdir
.SECONDARY: post-build

deploy: all
	@echo "-----------------"
	@echo "DEPLOY_DIR: $(DEPLOY_DIR)"
	@echo "-----------------"
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO_NAME) $(OUT_SO_LINK) 
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO) $(OUT_SO_NAME) 
	-$(CP) -d $(OUT_DIR)/lib* $(DEPLOY_DIR)/
	-$(RM) $(DEPLOY_DIR)/$(MAP_NAME)
