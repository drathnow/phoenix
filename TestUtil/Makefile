BUILD_DIR=../build

include ../defconfig
include $(BUILD_DIR)/scripts/makeenv.mk
include $(BUILD_DIR)/scripts/makerules.mk
include $(BUILD_DIR)/scripts/makesource.mk

OUT_SO_LINK = libtestutil.so
OUT_SO_NAME = $(OUT_SO_LINK).1
OUT_SO = $(OUT_SO_NAME).0.0
MAP_NAME = $(subst .so,,$(OUT_SO)).map

INCLUDES := -I../Foundation

LIBS += -llog4cplus

all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

dependents:

$(OUT_DIR):
	@echo "Creating output directory '$(OUT_DIR)'"
	mkdir $(OUT_DIR)
	
$(SHARED_DIR):
	@echo "Creating shared library directory '$(SHARED_DIR)'"
	$(MKDIR) $(SHARED_DIR)
	
$(OUT_DIR)/$(OUT_SO): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -shared -Wl,-soname,$(OUT_SO_NAME) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS) $(L_FLAGS) -Xlinker -Map=$(OUT_DIR)/$(MAP_NAME)
	@echo 'Finished building target: $@'

$(SHARED_DIR)/$(OUT_SO): $(OUT_DIR)/$(OUT_SO)
	@$(MAKE) --no-print-directory post-build
	
post-build: $(SHARED_DIR)
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO_NAME) $(OUT_SO_LINK) 
	-$(CD) $(OUT_DIR); $(LNS) $(OUT_SO) $(OUT_SO_NAME) 
	-$(CP) -d $(OUT_DIR)/lib*.so* $(SHARED_DIR)/
	-@echo ' '
	
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_SO) $(OUT_DIR)/$(OUT_SO_LINK) $(OUT_DIR)/$(OUT_SO_NAME) $(OUT_DIR)/$(MAP_NAME)
	#remove the shared dir copy of the so.
	-$(RM) $(SHARED_DIR)/$(OUT_SO) $(SHARED_DIR)/$(OUT_SO_LINK) $(SHARED_DIR)/$(OUT_SO_NAME)
