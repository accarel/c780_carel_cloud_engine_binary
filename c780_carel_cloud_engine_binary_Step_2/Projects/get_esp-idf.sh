cd ~/esp
# this will create a folder esp-idf with version 4.0-beta2
git clone -b v4.0  https://github.com/espressif/esp-idf.git
cd ~/esp/esp-idf
git submodule update --init --recursive

# this will apply possible patches
# patch modbus implementation for esp32
patch components/freemodbus/common/esp_modbus_master.c ~/esp/GME_Binary/patches/0001-esp-modbus-master.patch
# manage cmd 17
patch components/freemodbus/modbus/functions/mbfuncother.c ~/esp/GME_Binary/patches/0002_manage_cmd17.patch
patch components/freemodbus/modbus/include/mb_m.h ~/esp/GME_Binary/patches/0003_manage_cmd17.patch
patch components/freemodbus/modbus/include/mbconfig.h ~/esp/GME_Binary/patches/0004_enable_rep_slaveid.patch
# collect messages longer than 120 bytes
patch components/freemodbus/port/portserial_m.c ~/esp/GME_Binary/patches/0005_receive_mesg_longer_than_120.patch
patch components/freemodbus/modbus/include/mbport.h ~/esp/GME_Binary/patches/0006_receive_mesg_longer_than_120.patch
# in ota process, erase in chunks to yield for a while in between (thus avoiding task starvation) 
patch components/app_update/esp_ota_ops.c ~/esp/GME_Binary/patches/0007_erase_ota_in_chunks_to_avoid_starvation.patch
# manage cmd 21
cd components/freemodbus/modbus
patch -p0 < ~/esp/GME_Binary/patches/0008_manage_cmd21.patch
patch -p0 < ~/esp/GME_Binary/patches/0009_manage_cmd21.patch
cd ../../..
# extend MQTT timeout before qos 1 publish is resent with puback received (from 1 to 2 seconds)
patch components/mqtt/esp-mqtt/mqtt_client.c ~/esp/GME_Binary/patches/0010_mqtt_extend_puback_timeout.patch

# manage read file transfer 

cd ~/esp/esp-idf

patch components/freemodbus/modbus/include/mb_m.h ~/esp/GME_Binary/patches/0011_add_read_filetransf_1.patch
patch components/freemodbus/modbus/mb_m.c ~/esp/GME_Binary/patches/0012_add_read_filetransf_2.patch
patch components/freemodbus/modbus/include/mbfunc.h ~/esp/GME_Binary/patches/0013_add_read_filetransf_3.patch
patch components/freemodbus/modbus/functions/mbfuncother.c ~/esp/GME_Binary/patches/0014_add_read_filetransf_4.patch
patch components/freemodbus/modbus/include/mbconfig.h ~/esp/GME_Binary/patches/0015_add_read_filetransf_5.patch
patch components/freemodbus/modbus/include/mbproto.h ~/esp/GME_Binary/patches/0016_add_read_filetransf_6.patch
patch components/esp_https_ota/src/esp_https_ota.c ~/esp/GME_Binary/patches/0017_fix_ota_2G_pt1.patch
patch components/freemodbus/common/esp_modbus_master.c ~/esp/GME_Binary/patches/0019_fix_di_coil_read.patch

