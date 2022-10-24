BUILD_DIR=./build

include ./defconfig
include $(BUILD_DIR)/scripts/makeenv.mk
include $(BUILD_DIR)/scripts/makerules.mk
include $(BUILD_DIR)/scripts/makesource.mk

dependents:
	cd Foundation && $(MAKE) all
	cd Domain && $(MAKE) all
	cd FoundationTest && $(MAKE) all
	cd DomainTest && $(MAKE) all

clean:
	cd Foundation && $(MAKE) clean
	cd Domain && $(MAKE) clean
	cd FoundationTest && $(MAKE) clean
	cd DomainTest && $(MAKE) clean
    
test:
	cd FoundationTest && $(MAKE) test
	cd DomainTest && $(MAKE) test