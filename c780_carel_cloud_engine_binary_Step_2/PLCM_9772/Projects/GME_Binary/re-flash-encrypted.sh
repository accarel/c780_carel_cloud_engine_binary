

if [ ! -d ~/esp/GME_Binary/_crypt_build ]; then
  mkdir -p ~/esp/GME_Binary/_crypt_build ;
fi

#copy the build file into the new folder
cp -i ~/esp/GME_Binary/build/gme_carel.bin ~/esp/GME_Binary/_crypt_build 
cp -i ~/esp/GME_Binary/build/bootloader/bootloader.bin ~/esp/GME_Binary/_crypt_build 
cp -i ~/esp/GME_Binary/gme_part_tab.bin ~/esp/GME_Binary/_crypt_build 

#encrypt the application
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x10000 -o ~/esp/GME_Binary/_crypt_build/gme-enc-app.bin ~/esp/GME_Binary/_crypt_build/gme_carel.bin

#encrypt the bootloader
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x1000 -o ~/esp/GME_Binary/_crypt_build/bootloader_enc.bin ~/esp/GME_Binary/_crypt_build/bootloader.bin

#encrypt the part tab
python ~/esp/esp-idf/components/esptool_py/esptool/espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin  --address 0x8000 -o ~/esp/GME_Binary/_crypt_build/gme-enc-tab.bin ~/esp/GME_Binary/_crypt_build/gme_part_tab.bin

#flash of three part 
python ~/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x1000 ~/esp/GME_Binary/_crypt_build/bootloader_enc.bin 0x8000 ~/esp/GME_Binary/_crypt_build/gme-enc-tab.bin 0x10000 ~/esp/GME_Binary/_crypt_build/gme-enc-app.bin
