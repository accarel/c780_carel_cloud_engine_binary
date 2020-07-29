#!/bin/bash
test -n "${1}" || echo "Missing option"

case ${1} in
 DEBUG) MODE=1;;	
 RELEASE) MODE=0;;
 *) MODE=1;;
esac

~/esp/esp-idf/tools/idf.py build -DDEBUG_MODE=$MODE
