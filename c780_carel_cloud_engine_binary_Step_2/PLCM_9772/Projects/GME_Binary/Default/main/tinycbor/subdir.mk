################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main/tinycbor/cborencoder.c \
../main/tinycbor/cborencoder_close_container_checked.c \
../main/tinycbor/cborerrorstrings.c \
../main/tinycbor/cborparser.c 

OBJS += \
./main/tinycbor/cborencoder.o \
./main/tinycbor/cborencoder_close_container_checked.o \
./main/tinycbor/cborerrorstrings.o \
./main/tinycbor/cborparser.o 

C_DEPS += \
./main/tinycbor/cborencoder.d \
./main/tinycbor/cborencoder_close_container_checked.d \
./main/tinycbor/cborerrorstrings.d \
./main/tinycbor/cborparser.d 


# Each subdirectory must supply rules for building sources it contributes
main/tinycbor/%.o: ../main/tinycbor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


