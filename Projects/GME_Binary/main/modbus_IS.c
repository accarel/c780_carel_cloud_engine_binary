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
#include "polling_CAREL.h"

#include "gme_config.h"
#include "nvm.h"

// related to the system!!!!
#include "port.h"
#include "mb_m.h"
//#include "mbcontroller.h"
#include "mbconfig.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


#define  MODBUS_TIME_OUT     100


// TO DO...implementatiotion depend on the sistem chip in use!!!
extern BOOL xMBMasterPortSerialTxPoll(void);


static TaskHandle_t MODBUS_TASK = NULL;
static uint32_t MB_Device = 0;

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
    		return C_SUCCESS;
    	}
    	else{
    		printf("MODBUS initialize fail\n");
    		return C_FAIL;
    	}
     }
     else{
		 printf("MODBUS initialize fail\n");
		 return C_FAIL;
     }

     return C_FAIL;
}



/**
 * @brief Modbus_Task
 *        Start the modbus comunication
 *
 * @param none
 * @return none
 */

void Modbus_Task(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	while(1)
	{
			eMBMasterPoll();

			BOOL xSentState = xMBMasterPortSerialTxPoll();

			if (xSentState) {
				// Let state machine know that response was transmitted out
				(void)xMBMasterPortEventPost(EV_MASTER_FRAME_TRANSMITTED);
			}
		}
#endif
}


/**
 * @brief Modbus_Task_Start
 *        if we have a OS we let's start the task here
 *
 * @param none
 * @return none
 */
void Modbus_Task_Start(void)
{
	// to be implemented by USR
#ifdef INCLUDE_PLATFORM_DEPENDENT
	xTaskCreate(&Modbus_Task, "MODBUS_START", 2*2048, NULL, 10, MODBUS_TASK );
#endif

}



// 0x01 //single or multi-coils
int app_coil_read(const uint8_t addr, const int func, const int index, const int num)
{
	C_RES result = C_SUCCESS;
#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadCoils(addr, saddr, num, timeout);
    result = errorCode;
#endif

    return result;
}


//  0x02 //single or multi-coils
int app_coil_discrete_input_read(const uint8_t addr, const int func, const int index, const int num)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadDiscreteInputs(addr, saddr, num, timeout);
    result = errorCode;
#endif

    return result;
}


//  0x03 //single or multi-coils
int app_holding_register_read(const uint8_t addr, const int func, const int index, const int num)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadHoldingRegister(addr, saddr, num, timeout);
    result = errorCode;
#endif

    return result;
}


//  0x04 //single or multi-coils
int app_input_register_read(const uint8_t addr, const int func, const int index, const int num)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;
    errorCode = eMBMasterReqReadInputRegister(addr, saddr, num, timeout);
    result = errorCode;
#endif

    return result;
}


// app_coil_write
int app_coil_write(const uint8_t addr, const int index, short newData)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;

    errorCode = eMBMasterReqWriteCoil( addr, index, newData,timeout);

    result = errorCode;
#endif

    return result;
}



// app_hr_write
int app_hr_write(const uint8_t addr, const int index, C_CHAR num_of , C_UINT16 * newData)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;

    errorCode = eMBMasterReqWriteMultipleHoldingRegister( addr, index, num_of, newData, timeout );

    result = errorCode;
#endif

    return result;
}





// app_report_slave_id_read
C_RES app_report_slave_id_read(const uint8_t addr)
{
   C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;

    errorCode = eMBMAsterReqReportSlaveId(addr, timeout);
    result = errorCode;
#endif

    return result;
}

void Modbus_Disable(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	eMBMasterDisable();
#endif
}

void Modbus_Enable(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	ClearQueueMB();
	eMBMasterEnable();
#endif
}

void Modbus__ReadAddressFromNVM(void){
	C_UINT32 dev_addr;
	if(C_SUCCESS == NVM__ReadU32Value(MB_DEV_NVM, &dev_addr))
	    MB_Device = dev_addr;
	else
		MB_Device = 1;
}

C_UINT16 Modbus__GetAddress(void){
	return MB_Device;
}
