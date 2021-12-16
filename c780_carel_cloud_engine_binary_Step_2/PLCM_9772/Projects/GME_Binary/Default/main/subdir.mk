################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main/CBOR_CAREL.c \
../main/File_System_CAREL.c \
../main/File_System_IS.c \
../main/GSM_Miscellaneous_IS.c \
../main/IO_Port_IS.c \
../main/Led_Manager_IS.c \
../main/MQTT_Interface_CAREL.c \
../main/MQTT_Interface_IS.c \
../main/RTC_IS.c \
../main/SoftWDT.c \
../main/WebDebug.c \
../main/binary_model.c \
../main/filelog_CAREL.c \
../main/gme_https_ota.c \
../main/http_server_CAREL.c \
../main/http_server_IS.c \
../main/https_client_CAREL.c \
../main/https_client_IS.c \
../main/main_CAREL.c \
../main/main_IS.c \
../main/mobile.c \
../main/modbus_IS.c \
../main/nvm_CAREL.c \
../main/nvm_IS.c \
../main/ota_CAREL.c \
../main/ota_IS.c \
../main/polling_CAREL.c \
../main/polling_IS.c \
../main/radio.c \
../main/sys_CAREL.c \
../main/sys_IS.c \
../main/test_hw_CAREL.c \
../main/unlock_CAREL.c \
../main/utilities_CAREL.c \
../main/wifi.c 

OBJS += \
./main/CBOR_CAREL.o \
./main/File_System_CAREL.o \
./main/File_System_IS.o \
./main/GSM_Miscellaneous_IS.o \
./main/IO_Port_IS.o \
./main/Led_Manager_IS.o \
./main/MQTT_Interface_CAREL.o \
./main/MQTT_Interface_IS.o \
./main/RTC_IS.o \
./main/SoftWDT.o \
./main/WebDebug.o \
./main/binary_model.o \
./main/filelog_CAREL.o \
./main/gme_https_ota.o \
./main/http_server_CAREL.o \
./main/http_server_IS.o \
./main/https_client_CAREL.o \
./main/https_client_IS.o \
./main/main_CAREL.o \
./main/main_IS.o \
./main/mobile.o \
./main/modbus_IS.o \
./main/nvm_CAREL.o \
./main/nvm_IS.o \
./main/ota_CAREL.o \
./main/ota_IS.o \
./main/polling_CAREL.o \
./main/polling_IS.o \
./main/radio.o \
./main/sys_CAREL.o \
./main/sys_IS.o \
./main/test_hw_CAREL.o \
./main/unlock_CAREL.o \
./main/utilities_CAREL.o \
./main/wifi.o 

C_DEPS += \
./main/CBOR_CAREL.d \
./main/File_System_CAREL.d \
./main/File_System_IS.d \
./main/GSM_Miscellaneous_IS.d \
./main/IO_Port_IS.d \
./main/Led_Manager_IS.d \
./main/MQTT_Interface_CAREL.d \
./main/MQTT_Interface_IS.d \
./main/RTC_IS.d \
./main/SoftWDT.d \
./main/WebDebug.d \
./main/binary_model.d \
./main/filelog_CAREL.d \
./main/gme_https_ota.d \
./main/http_server_CAREL.d \
./main/http_server_IS.d \
./main/https_client_CAREL.d \
./main/https_client_IS.d \
./main/main_CAREL.d \
./main/main_IS.d \
./main/mobile.d \
./main/modbus_IS.d \
./main/nvm_CAREL.d \
./main/nvm_IS.d \
./main/ota_CAREL.d \
./main/ota_IS.d \
./main/polling_CAREL.d \
./main/polling_IS.d \
./main/radio.d \
./main/sys_CAREL.d \
./main/sys_IS.d \
./main/test_hw_CAREL.d \
./main/unlock_CAREL.d \
./main/utilities_CAREL.d \
./main/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
main/%.o: ../main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


