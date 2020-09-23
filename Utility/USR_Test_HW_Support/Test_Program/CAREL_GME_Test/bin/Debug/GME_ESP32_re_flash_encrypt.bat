

::if exist post_build rd /s /q post_build

::if not exist "post_build" mkdir post_build

::Xcopy /i gme_carel.bin post_build
::Xcopy /i bootloader.bin post_build
::Xcopy /i partition-table.bin post_build

::python espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin --address 0x10000 -o post_build/gme-enc-app.bin post_build/gme_carel.bin > prgout.txt

::python espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin --address 0x1000 -o post_build/bootloader_enc.bin post_build/bootloader.bin > prgout.txt

::python espsecure.py encrypt_flash_data --keyfile carel_encryption_key.bin --address 0x8000 -o post_build/partition-table-enc.bin post_build/partition-table.bin > prgout.txt


python esptool.py --chip esp32 --port %1 --baud %2 write_flash -z 0x1000 post_build/bootloader_enc.bin 0x8000 post_build/gme-enc-tab.bin 0x10000 post_build/gme-enc-app.bin 0x310000 post_build/gme_spiffs.bin > prgout.txt

