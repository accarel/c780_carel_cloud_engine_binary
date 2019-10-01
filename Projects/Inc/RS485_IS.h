/**
 * @file RS485_IS.h
 * @author USR
 * @date 1 Oct 2019
 * @brief  functions implementations specific related to the managment of RS485
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS485_IS_H
#define __RS485_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"      
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_RES RS485_Init(C_BYTE rs485_port, C_UINT32 rs485_baud_rate, C_BYTE rs485_stop_bits, C_BYTE rs485_parity);

#ifdef __cplusplus
}
#endif

#endif
