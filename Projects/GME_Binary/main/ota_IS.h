/**
 * @file   ota_IS.h
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the update of the FW of the attached device 
 *         via HTTPS, this is the platform dependent part
 */
 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OTA_IS
#define __OTA_IS


/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "data_types_CAREL.h"
#include "https_client_IS.h"
/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */


/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */


/* ========================================================================== */
/* Functions prototypes                                                       */
/* ========================================================================== */

C_RES uart_flush_IS(C_BYTE uart_num);
C_RES uart_flush_input_IS(C_BYTE uart_num);
C_RES https_ota(c_http_client_config_t* c_config);
void OTA__GMEInit(c_cborrequpdgmefw update_gw_fw);
C_RES OTA_GMEWaitCompletion(void);
void OTA_End(EventGroupHandle_t s_ota_group);
void OTAGroup (bool ota_res);
void OTA__DEVInit(c_cborrequpddevfw update_dev_fw);
void OTADEVGroup (bool ota_res);
void OTA__CAInit(c_cborrequpdatecacert update_ca);
void OTA__ModelInit(c_cborreqdwldevsconfig download_devs_config);

#endif  //__OTA_IS
