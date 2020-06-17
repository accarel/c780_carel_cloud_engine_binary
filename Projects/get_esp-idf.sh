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