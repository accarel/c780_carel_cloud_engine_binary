set(COMPONENT_SRCS ".")
set(COMPONENT_ADD_INCLUDEDIRS ".")

idf_component_register(
                    SRCS "binary_model.c" "CBOR_CAREL.c" "File_System_CAREL.c" "File_System_IS.c" "GSM_Miscellaneous_IS.c" "https_client_CAREL.c" "https_client_IS.c" "http_server_CAREL.c" "http_server_IS.c" "IO_Port_IS.c" "Led_Manager_IS.c" "main_CAREL.c" "main_IS.c" "mobile.c" "modbus_IS.c" "MQTT_Interface_CAREL.c" "MQTT_Interface_IS.c""nvm_CAREL.c" "nvm_IS.c" "ota_CAREL.c" "ota_IS.c" "polling_CAREL.c" "polling_IS.c" "radio.c" "RTC_IS.c" "SoftWDT.c" "sys_CAREL.c" "sys_IS.c" "utilities_CAREL.c" "wifi.c" "./tinycbor/cborencoder.c" "./tinycbor/cborencoder_close_container_checked.c" "./tinycbor/cborerrorstrings.c" "./tinycbor/cborparser.c"
                     
                    INCLUDE_DIRS "." "include" "../../esp-idf/components/freemodbus/modbus/include" "../../esp-idf/components/freemodbus/port")
