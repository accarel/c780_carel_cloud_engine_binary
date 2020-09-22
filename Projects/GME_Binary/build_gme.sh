#!/bin/bash
test -n "${1}" || echo "Missing option"

rm sdkconfig
rm sdkconfig.old

case ${1} in
 DEBUG) MODE=1;;	
 RELEASE) MODE=0;;
 *) MODE=1;;
esac


if [ "$MODE" = "0" ]; then
  echo "===== RELEASE selected =====" 
  cp sdkconfig.release sdkconfig
else
  echo "===== DEBUG selected =====" 
  cp sdkconfig.debug sdkconfig     
fi


~/esp/esp-idf/tools/idf.py clean
~/esp/esp-idf/tools/idf.py build -DDEBUG_MODE=$MODE
