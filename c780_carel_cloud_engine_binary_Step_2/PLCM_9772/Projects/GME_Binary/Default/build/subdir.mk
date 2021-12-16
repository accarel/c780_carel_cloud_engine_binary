################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../build/project_elf_src_esp32.c 

S_UPPER_SRCS += \
../build/signature_verification_key.bin.S 

OBJS += \
./build/project_elf_src_esp32.o \
./build/signature_verification_key.bin.o 

C_DEPS += \
./build/project_elf_src_esp32.d 


# Each subdirectory must supply rules for building sources it contributes
build/%.o: ../build/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/%.o: ../build/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


