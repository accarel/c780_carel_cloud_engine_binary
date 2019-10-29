#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
#
#	FW Ver 90.00.00
#
#COMPONENT_EMBED_FILES := favicon.ico
#COMPONENT_EMBED_FILES += xlogin.html
#COMPONENT_EMBED_FILES += xconfig_page_test01.html
#COMPONENT_EMBED_FILES += xstyle.css


COMPONENT_SRCDIRS += ../../esp-idf/components/freemodbus
COMPONENT_ADD_INCLUDEDIRS := ../../esp-idf/components/freemodbus/modbus/include
COMPONENT_ADD_INCLUDEDIRS += ../../esp-idf/components/freemodbus/port

#COMPONENT_EMBED_TXTFILES := carel_mqtt_broker_cert.crt
#COMPONENT_EMBED_TXTFILES += howsmyssl_com_root_cert.pem
#COMPONENT_EMBED_TXTFILES += iot_eclipse_org.pem
