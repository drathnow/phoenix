#
# Makefile to buld all Test Projects
#
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

# All Target (default)
all:  dependents
	@echo 'Done Building Test targets'


# Add inputs and outputs from these tool invocations to the build variables 

dependents: 
	@echo 'Building Test targets:'
	cd BaseClassesTest && $(MAKE) PLT=$(PLT) all
	cd ZediCommonTest && $(MAKE) PLT=$(PLT) all
	cd ConnectionManagerTest && $(MAKE) PLT=$(PLT) all
	cd SessionTest && $(MAKE) PLT=$(PLT) all
	cd ReportingTest && $(MAKE) PLT=$(PLT) all
	cd CommandHandlerTest && $(MAKE) PLT=$(PLT) all
	cd lpc3250Test && $(MAKE) PLT=$(PLT) all
	cd PortsTest && $(MAKE) PLT=$(PLT) all
	cd PassThroughTest && $(MAKE) PLT=$(PLT) all
	cd SchedulingTest && $(MAKE) PLT=$(PLT) all
	cd RealTimeEngineTest && $(MAKE) PLT=$(PLT) all
	cd SchemaPersistenceManagerTest && $(MAKE) PLT=$(PLT) all
	cd ModbusTest && $(MAKE) PLT=$(PLT) all
	cd MQWrapperTest && $(MAKE) PLT=$(PLT) all
	cd EventHandlerTest && $(MAKE) PLT=$(PLT) all
	cd ZiosWebTest && $(MAKE) PLT=$(PLT) all
	cd NetworkManagerCommonTest && $(MAKE) PLT=$(PLT) all
	cd NetworkMonitorLibTest && $(MAKE) PLT=$(PLT) all
	cd Fakepppd && $(MAKE) PLT=$(PLT) all
	cd zioslibTest && $(MAKE) PLT=$(PLT) all
	cd OTADTest && $(MAKE) PLT=$(PLT) all
	cd WatchdogLibTest && $(MAKE) PLT=$(PLT) all

.PHONY: build
build: 
	@echo 'Building Test targets:'
	cd BaseClassesTest && $(MAKE) PLT=$(PLT)
	cd ZediCommonTest && $(MAKE) PLT=$(PLT)
	cd ConnectionManagerTest && $(MAKE) PLT=$(PLT)
	cd SessionTest && $(MAKE) PLT=$(PLT)
	cd ReportingTest && $(MAKE) PLT=$(PLT)
	cd CommandHandlerTest && $(MAKE) PLT=$(PLT)
	cd lpc3250Test && $(MAKE) PLT=$(PLT)
	cd PortsTest && $(MAKE) PLT=$(PLT)
	cd PassThroughTest && $(MAKE) PLT=$(PLT)
	cd SchedulingTest && $(MAKE) PLT=$(PLT)
	cd RealTimeEngineTest && $(MAKE) PLT=$(PLT)
	cd SchemaPersistenceManagerTest && $(MAKE) PLT=$(PLT)
	cd ModbusTest && $(MAKE) PLT=$(PLT)
	cd MQWrapperTest && $(MAKE) PLT=$(PLT)
	cd EventHandlerTest && $(MAKE) PLT=$(PLT)
	cd ZiosWebTest && $(MAKE) PLT=$(PLT)
	cd NetworkManagerCommonTest && $(MAKE) PLT=$(PLT)
	cd NetworkMonitorLibTest && $(MAKE) PLT=$(PLT)
	cd Fakepppd && $(MAKE) PLT=$(PLT)
	cd zioslibTest && $(MAKE) PLT=$(PLT)
	cd OTADTest && $(MAKE) PLT=$(PLT)
	cd WatchdogLibTest && $(MAKE) PLT=$(PLT)

tests:
	./build/scripts/testrunner.sh $(PWD) $(PLT) test

# Other Targets
cleanall: 
	-cd BaseClassesTest && $(MAKE) PLT=$(PLT) cleanall
	-cd ZediCommonTest && $(MAKE) PLT=$(PLT) cleanall
	-cd ConnectionManagerTest && $(MAKE) PLT=$(PLT) cleanall
	-cd SessionTest && $(MAKE) PLT=$(PLT) cleanall
	-cd ReportingTest && $(MAKE) PLT=$(PLT) cleanall
	-cd CommandHandlerTest && $(MAKE) PLT=$(PLT) cleanall
	-cd lpc3250Test && $(MAKE) PLT=$(PLT) cleanall
	-cd PortsTest && $(MAKE) PLT=$(PLT) cleanall
	-cd PassThroughTest && $(MAKE) PLT=$(PLT) cleanall
	-cd SchedulingTest && $(MAKE) PLT=$(PLT) cleanall
	-cd ModbusTest && $(MAKE) PLT=$(PLT) cleanall
	-cd RealTimeEngineTest && $(MAKE) PLT=$(PLT) cleanall
	-cd SchemaPersistenceManagerTest && $(MAKE) PLT=$(PLT) cleanall
	-cd MQWrapperTest && $(MAKE) PLT=$(PLT) cleanall
	-cd EventHandlerv && $(MAKE) PLT=$(PLT) cleanall
	-cd ZiosWebTest && $(MAKE) PLT=$(PLT) cleanall
	-cd NetworkManagerCommonTest && $(MAKE) PLT=$(PLT) cleanall
	-cd NetworkMonitorLibTest && $(MAKE) PLT=$(PLT) cleanall
	-cd Fakepppd && $(MAKE) PLT=$(PLT) cleanall
	-cd zioslibTest && $(MAKE) PLT=$(PLT) cleanall
	-cd OTADTest && $(MAKE) PLT=$(PLT) cleanall
	-cd WatchdogLibTest && $(MAKE) PLT=$(PLT) cleanall

runtests:
	./build/scripts/runprojecttest.sh $(PLT)

.PHONY: all  dependents cleanall 


