################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/bootloader/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp 

OBJS += \
./build/bootloader/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.o 

CPP_DEPS += \
./build/bootloader/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
build/bootloader/CMakeFiles/3.16.3/CompilerIdCXX/%.o: ../build/bootloader/CMakeFiles/3.16.3/CompilerIdCXX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


