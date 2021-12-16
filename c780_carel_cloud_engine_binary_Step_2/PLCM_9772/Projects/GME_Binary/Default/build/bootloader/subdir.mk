################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../build/bootloader/project_elf_src_esp32.c 

S_UPPER_SRCS += \
../build/bootloader/signature_verification_key.bin.S 

OBJS += \
./build/bootloader/project_elf_src_esp32.o \
./build/bootloader/signature_verification_key.bin.o 

C_DEPS += \
./build/bootloader/project_elf_src_esp32.d 


# Each subdirectory must supply rules for building sources it contributes
build/bootloader/%.o: ../build/bootloader/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/bootloader/%.o: ../build/bootloader/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


