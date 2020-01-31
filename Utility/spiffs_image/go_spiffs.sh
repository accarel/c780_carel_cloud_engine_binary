<<<<<<< .mine
python esptool.py --chip esp32 --port /dev/ttyUSB1 --baud 921600 write_flash -z 0x210000 bcu_spiffs.bin||||||| .r206
python esptool.py --chip esp32 --port /dev/ttyUSB1 --baud 921600 write_flash -z 0x210000 bcu_spiffs.bin
=======
python2 esptool.py --port COM13 --baud 921600 write_flash -z 0x310000 bcu_spiffs.bin
>>>>>>> .r216
