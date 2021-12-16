################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../components/modem/src/bg96.c \
../components/modem/src/esp_modem.c \
../components/modem/src/esp_modem_dce_service.c \
../components/modem/src/sim800.c 

OBJS += \
./components/modem/src/bg96.o \
./components/modem/src/esp_modem.o \
./components/modem/src/esp_modem_dce_service.o \
./components/modem/src/sim800.o 

C_DEPS += \
./components/modem/src/bg96.d \
./components/modem/src/esp_modem.d \
./components/modem/src/esp_modem_dce_service.d \
./components/modem/src/sim800.d 


# Each subdirectory must supply rules for building sources it contributes
components/modem/src/%.o: ../components/modem/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


