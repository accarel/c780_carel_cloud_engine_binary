The program ..\CustumCfgGenerator.exe put the information you provide 
into the files 
.\dir_image\cfgdef.bin
.\dir_image\cfgusr.bin

the other files present in the .\dir_image are customizable if needed
but take care that the current FW recognize now only the ones listed below

- cert1.crt   
security certificate to connect to the cloud must be update later via OTA

- cert2.crt
copy of security certificate to connect to the cloud must be update later via OTA

- chcred.html
page to set the login data for the first time

- config.html
page to set the WiFi configuration parameters 

- fav.ico
logo of the HTML page default is the CAREL logo

- login.html
HTML page of login 

- model.bin
optional default model use by Modbus it will be update later via OTA

- style.css
the CSS of the HTML pages listed above


When you launch CustumCfgGenerator.exe the result file is placed 
in this folder in the file  .\gme_spiffs.bin

This file MUST be flashed in ESP32 via the batch files
go_spiffs.sh
a simple linux bash file that flash the spiffs into an ESP32-4MB flash memory
Note that is an easy task to port this script into a .bat Windows one. 







==========================================================================
Other files
==========================================================================

go_make_spiffs.bat
this batch file generate a spiffs binary image of the content found on 
dir_image directory is used by CustumCfgGenerator.exe to assemble the 
gme_spiffs.bin



.\ESP_example
in this folder 2 file of an example to use the image inside an application
