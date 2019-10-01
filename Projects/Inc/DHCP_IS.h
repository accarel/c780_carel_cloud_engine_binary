/**
 * @file DHCP_IS.h
 * @author USR
 * @date 1 Oct 2019
 * @brief  functions implementations specific related to the management of DNS client
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHCP_IS_H
#define __DHCP_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"      
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_RES DHCP_Client_Init(void);

#ifdef __cplusplus
}
#endif

#endif
