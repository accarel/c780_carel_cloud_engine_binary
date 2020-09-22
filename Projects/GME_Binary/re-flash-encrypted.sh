#!/bin/bash

if [ ! -d ~/esp/GME_Binary/_crypt_build ]; then
  mkdir -p ~/esp/GME_Binary/_crypt_build ;
fi

#copy the build file into the new folder
rm gme-enc-app.bin
rm bootloader_enc.bin
rm partition-table_enc.bin

cp -f ~/esp/GME_Binary/build/gme_carel.bin ~/esp/GME_Binary/_crypt_build 
cp -f ~/esp/GME_Binary/build/bootloader/bootloader.bin ~/esp/GME_Binary/_crypt_build 
cp -f ~/esp/GME_Binary/build/partition_table/partition-table.bin  ~/esp/GME_Binary/_crypt_build 

#encrypt the application
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x10000 -o _crypt_build/gme-enc-app.bin _crypt_build/gme_carel.bin

#encrypt the bootloader
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x1000 -o _crypt_build/bootloader_enc.bin _crypt_build/bootloader.bin

#encrypt the part tab
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x8000 -o _crypt_build/partition-table_enc.bin _crypt_build/partition-table.bin

#flash of three part 
python ~/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x1000 _crypt_build/bootloader_enc.bin 0x8000 _crypt_build/partition-table_enc.bin 0x10000 _crypt_build/gme-enc-app.bin

idf.py -p /dev/ttyUSB0 monitor


