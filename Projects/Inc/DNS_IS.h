/**
 * @file DNS_IS.h
 * @author USR
 * @date 1 Oct 2019
 * @brief  functions implementations specific related to the management of DNS client
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DNS_IS_H
#define __DNS_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"      
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_RES DNS_Client_Init(C_IPV4 dns_address);

#ifdef __cplusplus
}
#endif

#endif
