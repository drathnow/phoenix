BUILD_DIR=../build

include ../defconfig
include $(BUILD_DIR)/scripts/makeenv.mk
include $(BUILD_DIR)/scripts/makerules.mk
include $(BUILD_DIR)/scripts/makesource.mk

OUT_EXE = bbbtest

INCLUDES += -I../Foundation
INCLUDES += -I../BeagleBoneBlack
INCLUDES += -I../TestUtil

LIB_PATH += -L../sharedlibs/$(CONFIG)

LIBS += -lpthread
LIBS += -lFoundation
LIBS += -lbbb
LIBS += -lgtest 
LIBS += -lgmock
LIBS += -lpthread
LIBS += -llog4cplus

all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_EXE)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
endif

dependents:
	cd ../Foundation && $(MAKE)
	cd ../BeagleBoneBlack && $(MAKE)

$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

$(OUT_DIR)/$(OUT_EXE): $(OBJS) $(USER_OBJS) dependents
	@echo 'Building target: $@'
	@echo 'LIB_PATH: $(LIB_PATH)'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) -o $@ $(OBJS) $(USER_OBJS) $(LIB_PATH) $(LIBS)
	@echo 'Finished building target: $@'

test:
	export LD_LIBRARY_PATH=../sharedlibs/$(CONFIG) && $(OUT_DIR)/$(OUT_EXE)

clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) $(OUT_DIR)/$(OUT_EXE)
	-@echo ' '

