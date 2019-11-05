/**
 * @file   https_client_IS.h
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the download of some files via HTTPS
 */
 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HTTP_CLIENT_IS
#define __HTTP_CLIENT_IS


/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "data_types_CAREL.h"


#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "wifi.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "file_system.h"
#include "esp_http_client.h"
#endif


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */


/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */

/**
 * @brief c_http_client_config_t this structure is used to pass the parameters need to connect
 *          
 * @param  url        < HTTP URL, the information on the URL is most important, it overrides the other fields below, if any 
 * @param *username   < Using for Http authentication  
 * @param *password   < Using for Http authentication 
 */
typedef struct {
    const char                  *url;                
    const char                  *username;           
    const char                  *password;           
	int                 	   cert_num;
} c_http_client_config_t;



/**
 * @brief http_client_handle_t this structure is the one returned when you are connected to the HTTPS server
 *                             is implementation specific and depends on the library you use
 */
#ifdef INCLUDE_PLATFORM_DEPENDENT 
typedef esp_http_client_handle_t http_client_handle_t;
#endif

#ifndef INCLUDE_PLATFORM_DEPENDENT
typedef  (YOU MUST INSERT HERE YOUR CLIENT STRUCTURE ACCORDING WITH YOUR LIBRARY) http_client_handle_t;
#endif


/* ========================================================================== */
/* Functions prototypes                                                       */
/* ========================================================================== */

http_client_handle_t http_client_init(c_http_client_config_t *config, C_BYTE cert_num);
C_INT32 http_client_open(http_client_handle_t client, C_INT32 write_len);
C_INT32 http_client_fetch_headers(http_client_handle_t client);
C_INT32 http_client_read(http_client_handle_t client, C_CHAR *buffer, C_INT32 len);
C_INT32 http_client_close(http_client_handle_t client);
C_INT32 http_client_cleanup(http_client_handle_t client);
 
#endif
