python esptool.py --chip esp32 --port %1 --baud %2 write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0xd000 ota_data_initial.bin 0x1000 bootloader.bin 0x8000 gme_part_tab.bin 0x10000 gme_carel.bin 0x310000 gme_spiffs.bin > prgout.txt

