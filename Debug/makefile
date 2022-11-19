################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM :=  -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := mqtt_heater_control
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: mqtt_heater_control.elf

# Tool invocations
mqtt_heater_control.elf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	arm-linux-gnueabihf-g++ -L"C:\Users\bdosd\git\mqtt_heater_control\lib" -o "mqtt_heater_control.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lpthread -lmosquitto
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) mqtt_heater_control.elf
	-@echo ' '

post-build:
	-@echo 'C:\Users\Bertrand\workspace\HomeControl'
	-C:/Users/bdosd/git/mqtt_heater_control\utils\deploy.bat  "C:/Users/bdosd/git/mqtt_heater_control/Debug/mqtt_heater_control.elf" /home/pi/projects/HomeControlRemote.a
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
