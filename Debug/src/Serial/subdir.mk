################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Serial/Serial_unix.cpp \
../src/Serial/Serial_windows.cpp 

OBJS += \
./src/Serial/Serial_unix.o \
./src/Serial/Serial_windows.o 

CPP_DEPS += \
./src/Serial/Serial_unix.d \
./src/Serial/Serial_windows.d 


# Each subdirectory must supply rules for building sources it contributes
src/Serial/%.o: ../src/Serial/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/libusbp-1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


