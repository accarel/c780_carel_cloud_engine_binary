#!/bin/bash

#flash of three part 
python ~/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x1000 build/bootloader/bootloader.bin 0x8000 build/gme_part_tab.bin 0x10000 build/gme_carel.bin

