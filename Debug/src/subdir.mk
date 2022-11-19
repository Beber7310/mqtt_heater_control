################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/client.cpp \
../src/main.cpp 

C_SRCS += \
../src/SerialManagement.c 

CPP_DEPS += \
./src/client.d \
./src/main.d 

C_DEPS += \
./src/SerialManagement.d 

OBJS += \
./src/SerialManagement.o \
./src/client.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"C:\SysGCC\raspberry\arm-linux-gnueabihf\include\arm-linux-gnueabihf" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"C:\SysGCC\raspberry\arm-linux-gnueabihf\include\arm-linux-gnueabihf" -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/SerialManagement.d ./src/SerialManagement.o ./src/client.d ./src/client.o ./src/main.d ./src/main.o

.PHONY: clean-src

