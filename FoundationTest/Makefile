BUILD_DIR=../build

include ../defconfig
include $(BUILD_DIR)/scripts/makeenv.mk
include $(BUILD_DIR)/scripts/makerules.mk
include $(BUILD_DIR)/scripts/makesource.mk

OUT_EXE = FoundationTest

INCLUDES += -I../Foundation
INCLUDES += -I../TestUtil

LIB_PATH += -L../sharedlibs/$(CONFIG)

LIBS += -lpthread
LIBS += -lFoundation
LIBS += -lgtest 
LIBS += -lgmock
LIBS += -llog4cplus

all: $(OUT_DIR) dependents $(OUT_DIR)/$(OUT_EXE)

dependents:
	cd ../Foundation && $(MAKE) all

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

