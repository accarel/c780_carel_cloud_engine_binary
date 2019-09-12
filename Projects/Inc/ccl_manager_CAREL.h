/**
 * @file ccl_manager_carel.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   this file contains some management routine of
 *          the Carel Cloud Library
 */


#ifndef __CCL_MANAGER_CAREL_H
#define __CCL_MANAGER_CAREL_H


/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"



/* Varaibles -----------------------------------------------------------------*/
static C_BYTE polling_running;
   

/* Functions Implementation --------------------------------------------------*/

void Set_polling_Status(C_BYTE value);
C_BYTE Set_polling_Status(void);

#endif   /* #ifndef __CCL_MANAGER_CAREL_H */

