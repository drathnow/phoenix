# Rules for building files
# for c++ rules
$(OUT_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) $(DEFINES) $(INCLUDES) $(GCC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#for c rules
$(OUT_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	$(CC)  $(DEFINES) $(INCLUDES) $(CC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
