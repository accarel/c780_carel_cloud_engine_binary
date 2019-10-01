/**
 * @file RS485_IS.c
 * @author USR
 * @date 1 Oct 2019
 * @brief  contains functions related to RS485      
 *         
 */
#include "RS485_IS.h"

/* Functions implementation -------------------------------------------------------*/

/**
 * @brief RS485_Init
 *
 * Initialization of RS485
 * 
 * @param C_BYTE rs485_port
 * @param C_UINT32 rs485_baud_rate
 * @param C_BYTE rs485_stop_bits
 * @param C_BYTE rs485_parity
 * @return C_SUCCESS/C_FAIL 
 */
C_RES RS485_Init(C_BYTE rs485_port, C_UINT32 rs485_baud_rate, C_BYTE rs485_stop_bits, C_BYTE rs485_parity)
{
	return 1;
}


