
FILES_WITH_FOO := $(shell ls build/configs/*_defconfig)
FILES := $(patsubst %_defconfig,%,$(notdir $(FILES_WITH_FOO)))


all: cleanall buildall


buildall:
	@echo "using versions from file $(VERSIONS)"
	cd zios && $(MAKE)
	cd LE910B1ModemDriver && $(MAKE)
	cd NetworkManager && $(MAKE)
	cd NetworkMonitorLib && $(MAKE)
	cd Watchdog && $(MAKE)
	cd ModemTester && $(MAKE)
	cd lpc3250 && $(MAKE)
	cd IOBoardTester && $(MAKE)

cleanall:
	cd zios && $(MAKE) cleanall
	cd LE910B1ModemDriver && $(MAKE) cleanall
	cd NetworkManager && $(MAKE) cleanall
	cd NetworkMonitorLib && $(MAKE) cleanall
	cd Watchdog && $(MAKE) cleanall
	cd ModemTester && $(MAKE) cleanall
	cd lpc3250 && $(MAKE) cleanall
	cd IOBoardTester && $(MAKE) clean cleanall

buildtests:
	cd BaseClassesTest && $(MAKE)
	cd CommandHandlerTest && $(MAKE)
	cd ConnectionManagerTest && $(MAKE)
	cd EventHandlerTest && $(MAKE)
	cd lpc3250Test && $(MAKE)
	cd ModbusTest && $(MAKE)
	cd MQWrapperTest && $(MAKE)
	cd NetworkManagerCommonTest && $(MAKE)
	cd NetworkMonitorLibTest && $(MAKE)
	cd OTADTest && $(MAKE)
	cd TestSupport && $(MAKE)
	cd PassThroughTest && $(MAKE)
	cd PortsTest && $(MAKE)
	cd RealTimeEngineTest && $(MAKE)
	cd ReportingTest && $(MAKE)
	cd SchedulingTest && $(MAKE)
	cd SchemaPersistenceManagerTest && $(MAKE)
	cd SessionTest && $(MAKE)
	cd WatchdogLibTest && $(MAKE)
	cd WatchdogTest && $(MAKE)
	cd ZediCommonTest && $(MAKE)
	cd zioslibTest && $(MAKE)
	cd ZiosWebTest && $(MAKE)

runtests:
	cd BaseClassesTest && $(MAKE) runtest
	cd CommandHandlerTest && $(MAKE) runtest
	cd ConnectionManagerTest && $(MAKE) runtest
	cd EventHandlerTest && $(MAKE) runtest
	cd lpc3250Test && $(MAKE) runtest
	cd ModbusTest && $(MAKE) runtest
	cd MQWrapperTest && $(MAKE) runtest
	cd NetworkManagerCommonTest && $(MAKE) runtest
	cd NetworkMonitorLibTest && $(MAKE) runtest
	cd OTADTest && $(MAKE) runtest
	cd TestSupport && $(MAKE) runtest
	cd PassThroughTest && $(MAKE) runtest
	cd PortsTest && $(MAKE) runtest
	cd RealTimeEngineTest && $(MAKE) runtest
	cd ReportingTest && $(MAKE) runtest
	cd SchedulingTest && $(MAKE) runtest
	cd SchemaPersistenceManagerTest && $(MAKE) runtest
	cd SessionTest && $(MAKE) runtest
	cd WatchdogLibTest && $(MAKE) runtest
	cd WatchdogTest && $(MAKE) runtest
	cd ZediCommonTest && $(MAKE) runtest
	cd zioslibTest && $(MAKE) runtest
	cd ZiosWebTest && $(MAKE) runtest
	
.PHONY: $(FILES)
$(FILES):
	@cp build/configs/$@_defconfig ./defconfig
	@echo "Build configuration set to $@"

