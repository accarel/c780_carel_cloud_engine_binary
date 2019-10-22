/**
 * @file modbus_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  this is ONLY an example of the flow request to make a
 *         functional system based on the Carel Cloud Library.
 *         Some routine could be called as a task if an
 *         operating system is available.
 *         In case the OS is not available is possible to use the
 *         routine in a mega-loop but take care that the system are
 *         able to run without significat jitter.
 */
#include "stdint.h"

#include "modbus_IS.h"
#include "data_types_CAREL.h"
#include "data_types_IS.h"
#include "poll_engine.h"

#include "gme_config.h"

// related to the system!!!!
#include "port.h"
#include "mb_m.h"
#include "mbcontroller.h"
#include "mbconfig.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

int app_holding_register_read(const uint8_t addr, const int func, const int index, const int num);

// TO DO...implementatiotion depend on the sistem chip in use!!!
extern BOOL xMBMasterPortSerialTxPoll(void);


static TaskHandle_t MODBUS_TASK = NULL;

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
//#define __DEBUG_MODBUS_CAREL




/**
 * @brief Modbus_Init
 *        Initialize the modbus protocol
 *
 * @param none
 * @return C_SUCCESS or C_FAIL
 */
C_RES Modbus_Init(C_INT16 baud)  // stop /start /parity
{
	 // TO DO...implementatiotion depend on the sistem chip in use!!!

    // Set UART pin numbers
   	 esp_err_t err = uart_set_pin(MB_PORTNUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, -1);

     eMBErrorCode eStatus;
	 eMBErrorCode status = MB_EIO;

	 eStatus = eMBMasterInit(MB_RTU, 2, 19200, MB_PAR_NONE); // ok
	 vTaskDelay(5);

     if (0 == eStatus)
     {
    	eStatus = eMBMasterEnable();
    	if (0 == eStatus){

    		 // Set driver mode to Half Duplex
    		err = uart_set_mode(MB_PORTNUM, UART_MODE_RS485_HALF_DUPLEX);
    		return C_TRUE;
    	}
    	else{
    		printf("MODBUS initialize fail\n");
    		return C_FALSE;
    	}
     }
     else{
		 printf("MODBUS initialize fail\n");
		 return C_FALSE;
     }

     return C_FALSE;
}



/**
 * @brief Modbus_Init
 *        Start the modbus comunication
 *
 * @param none
 * @return none
 */

void Modbus_Task(void)
{
	while(1)
	{
			eMBMasterPoll();

			BOOL xSentState = xMBMasterPortSerialTxPoll();

			if (xSentState) {
				// Let state machine know that response was transmitted out
				(void)xMBMasterPortEventPost(EV_MASTER_FRAME_TRANSMITTED);
			}
		}
}


void Modbus_Task_Start(void)
{
	// to be implemented by USR

	xTaskCreate(&Modbus_Task, "MODBUS_START", 2*2048, NULL, 10, MODBUS_TASK );


	// the required stack for the task are 4096    <---------
}



// 0x01 //single or multi-coils
int app_coil_read(const uint8_t addr, const int func, const int index, const int num)
{
    const long timeout = 100;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadCoils(addr, saddr, num, timeout);
    return errorCode;
}


//  0x02 //single or multi-coils
int app_coil_discrete_input_read(const uint8_t addr, const int func, const int index, const int num)
{
    const long timeout = 100;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadDiscreteInputs(addr, saddr, num, timeout);
    return errorCode;
}


//  0x03 //single or multi-coils
int app_holding_register_read(const uint8_t addr, const int func, const int index, const int num)
{
    const long timeout = 100;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadHoldingRegister(addr, saddr, num, timeout);
    return errorCode;
}


//  0x04 //single or multi-coils
int app_input_register_read(const uint8_t addr, const int func, const int index, const int num)
{
    const long timeout = 100;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadInputRegister(addr, saddr, num, timeout);
    return errorCode;
}








