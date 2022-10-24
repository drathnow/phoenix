CONFIGS_DIR := $(BUILD_DIR)/configs
DEF_CONFIG := $(shell if [ -e ./defconfig ]; then echo $$(readlink -f ./defconfig); else echo ; fi;)

ifeq ($(DEF_CONFIG),)
  $(error defconfig not found.  Run make with configuration)
else
  include $(DEF_CONFIG)
endif