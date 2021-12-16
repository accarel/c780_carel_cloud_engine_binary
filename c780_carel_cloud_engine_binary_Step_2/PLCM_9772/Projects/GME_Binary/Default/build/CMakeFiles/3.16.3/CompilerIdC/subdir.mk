################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c 

OBJS += \
./build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.o 

C_DEPS += \
./build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
build/CMakeFiles/3.16.3/CompilerIdC/%.o: ../build/CMakeFiles/3.16.3/CompilerIdC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


