cd ~/esp
# this will create a folder esp-idf with version 4.0-beta2
git clone -b v4.0-beta2  https://github.com/espressif/esp-idf.git
cd ~/esp/esp-idf
git submodule update --init --recursive
