svn checkout http://svncarel.carel.com/svn/c780_carel_cloud_engine_binary/Projects/GME_Binary
cd esp-idf
patch components/freemodbus/common/esp_modbus_master.c ~/esp/GME_Binary/patches/0001-esp-modbus-master.patch
patch components/freemodbus/modbus/functions/mbfuncother.c ~/esp/GME_Binary/patches/0002_manage_cmd17.patch
patch components/freemodbus/modbus/include/mb_m.h ~/esp/GME_Binary/patches/0003_manage_cmd17.patch


