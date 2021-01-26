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
#include "MQTT_Interface_CAREL.h"

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE
/**
 * @brief _DEBUG_HTTPS_CLIENT_CAREL
 *        if defined print on the debug console some debugger message but only if
 *        global __CCL_DEBUG_MODE is defined
 */
#define _DEBUG_HTTPS_CLIENT_CAREL
#endif

/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */

typedef enum https_conn_err_s{
	CONN_OK = 0,
	CONN_FAIL,
	FILE_NOT_SAVED,
	NO_HEAP_MEMORY,
	WRONG_CRC,
	WRONG_FILE,
}https_conn_err_t;



#ifndef INCLUDE_PLATFORM_DEPENDENT
void HTTPClient__TestTask(void *pvParameters);
#endif

C_RES HttpsClient__DownloadFile(c_cborreqdwldevsconfig *download_devs_config, uint8_t cert_num, const char *filename);
C_RES HttpsClient__UpdateCertificate(c_cborrequpdatecacert *update_ca_cert);

#endif /* MAIN_HTTPS_CLIENT_C_ */

