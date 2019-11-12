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

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */


/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */


/* ========================================================================== */
/* Functions prototypes                                                       */
/* ========================================================================== */

C_INT16 uart_read_bytes_IS(C_BYTE uart_num, C_BYTE *buf, C_UINT32 length, C_UINT32 ticks_to_wait);
C_INT16 uart_write_bytes_IS(C_BYTE uart_num, const C_BYTE* src, C_INT16 size);
C_RES uart_flush_IS(C_BYTE uart_num);
C_RES uart_flush_input_IS(C_BYTE uart_num);


#endif  //__OTA_IS
