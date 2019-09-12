/**
 * @file ccl_manager_carel.c
 * @author carel
 * @date 9 Sep 2019
 * @brief   this file contains some management routine of
 *          the Carel Cloud Library
 */


/* Includes ------------------------------------------------------------------*/


/* Varaibles -----------------------------------------------------------------*/
static C_BYTE polling_running;  

   

/* Functions Implementation --------------------------------------------------*/

/**
 * @brief Set_polling_Status
 *        Manage the satus of the polling engine 
 * @param C_BYTE value C_START/C_STOP
 * @return none
 * @warning set to C_START only after modbus initialization
 */
void Set_polling_Status(C_BYTE value)
{
	polling_running = value;	
}


/**
 * @brief Get_polling_Status
 *        return the status of the polling engine
 * 
 * @param none C_BYTE value C_START/C_STOP
 * @return C_BYTE C_START or C_STOP
 */
C_BYTE Get_polling_Status(void)
{
	return polling_running;	
}

