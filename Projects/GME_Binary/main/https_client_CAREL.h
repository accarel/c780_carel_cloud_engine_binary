/**
 * @file   https_client_CAREL.h
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the download of some files via HTTPS
 */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_HTTPS_CLIENT_C_
#define MAIN_HTTPS_CLIENT_C_



/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */

#include "common.h"
#include "esp_tls.h"
#include "MQTT_Interface_CAREL.h"

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */


/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */

typedef enum https_conn_err_s{
	CONN_OK = 0,
	CONN_FAIL,
	FILE_NOT_SAVED,
	NO_HEAP_MEMORY,
}https_conn_err_t;





#ifndef INCLUDE_PLATFORM_DEPENDENT
void HTTPClient__TestTask(void *pvParameters);
#endif

https_conn_err_t HttpsClient__DownloadFile(req_download_devs_config_t *download_devs_config, uint8_t cert_num, const char *filename);
https_conn_err_t HttpsClient__UpdateCertificate(c_cborrequpdatecacert *update_ca_cert, C_BYTE cert_num);

#endif /* MAIN_HTTPS_CLIENT_C_ */

