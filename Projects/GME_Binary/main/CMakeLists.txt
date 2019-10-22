set(COMPONENT_SRCS "main.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()


idf_component_register(SRCS "mb_sense_main.c" 
                            "mb_sense_modbus.c"
                            "mb_device_params.c"
                    INCLUDE_DIRS "." "include")
