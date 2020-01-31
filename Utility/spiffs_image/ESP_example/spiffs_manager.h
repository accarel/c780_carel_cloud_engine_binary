#ifndef _SPIFFS_MANAGER_H
#define _SPIFFS_MANAGER_H


/*
space for :
4 certificates
1 model
1 file for ...
*/
#define MAX_NUM_OF_FILES    6


/* file names
 * this is the named stored by the script that build the ffs
 * take care to upper/lower case
 * */
#define CERT_1_NAME "/spiffs/cert1.crt"
#define CERT_2_NAME "/spiffs/cert2.crt"
#define MODEL_NAME  "/spiffs/model.bin"


long filesize(const char *fname);
esp_err_t spiffs_init(void);


#endif


