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
#include "nvm_CAREL.h"
#include "port.h"
#include "mb_m.h"
//#include "mbcontroller.h"
#include "mbconfig.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "sys_IS.h"
#include "SoftWDT.h"

#include "IO_Port_IS.h"

#define  MODBUS_TIME_OUT     100

static C_SBYTE GetStopBitTable(C_SBYTE stp);
static C_SBYTE GetParityTable(C_SBYTE prt);

// TO DO...implementatiotion depend on the sistem chip in use!!!
extern BOOL xMBMasterPortSerialTxPoll(void);


static TaskHandle_t MODBUS_TASK = NULL;
static uint32_t MB_Device = 0;
static uint16_t MB_Delay = 0;

C_UINT16 ModbusDisabled = 0;


extern CHAR ucMBFileTransfer[256];
extern USHORT usMBFileTransferLen;

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
C_RES Modbus_Init(C_INT32 baud, C_SBYTE parity, C_SBYTE stopbit, C_BYTE port)  // TODO stop bit da capire
{
     eMBErrorCode eStatus;
     esp_err_t err = C_FAIL;

     // translate into the esp constant
     C_SBYTE mParity = GetParityTable(parity);

     if(port == MB_PORTNUM_485)
     {
   	    err = uart_set_pin(port, Get_TEST_TXD(), Get_TEST_RXD(), Get_TEST_RTS(), -1);   // MB_PORTNUM
   	    PRINTF_DEBUG("RS485 selected\n");
     }
   	 else
   	 {
        err = uart_set_pin(port, Get_TTL_TXD(), Get_TTL_RXD(), Get_TTL_RTS(), -1); 		// MB_PORTNUM
    	PRINTF_DEBUG("TTL selected\n");
   	 }

   	 if(err != 0)
   	   PRINTF_DEBUG("Setting UART pin fail\n");

	 eStatus = eMBMasterInit(MB_RTU, port, baud, mParity);
	 Sys__Delay(50);

     if (0 == eStatus)
     {
    	eStatus = eMBMasterEnable();
    	if (0 == eStatus){

    		// Set driver mode to Half Duplex
    		if(port == MB_PORTNUM_485)
    		  err = uart_set_mode(port, UART_MODE_RS485_HALF_DUPLEX);
    		else
    		  err = uart_set_mode(port, UART_MODE_RS485_HALF_DUPLEX);

    		return C_SUCCESS;
    	}
    	else{
    		PRINTF_DEBUG("MODBUS initialize fail\n");
    		return C_FAIL;
    	}
     }
     else{
		 PRINTF_DEBUG("MODBUS initialize fail\n");
		 return C_FAIL;
     }

     return C_FAIL;
}


//
// table translation for esp32 uart setting
//

static C_SBYTE GetStopBitTable(C_SBYTE stp)
{
   C_SBYTE val;

   switch(stp)
   {
   	   default:
   	   case 0:
   	   case 1:
   		   val =  UART_STOP_BITS_1;
   		   break;

   	   case 2:
   		   val =  UART_STOP_BITS_2;
   		break;

   	   case 3:
   		   val =  UART_STOP_BITS_1_5;
   		break;
   }
   return val;
}

static C_SBYTE GetParityTable(C_SBYTE prt)
{
	   C_SBYTE val;

	   switch(prt)
	   {
	   	   default:
	   	   case 0:
	   		   val =  UART_PARITY_DISABLE;
	   		   break;

	   	   case 1:
	   		   val =  UART_PARITY_EVEN;
	   		break;

	   	   case 2:
	   		   val =  UART_PARITY_ODD;
	   		break;
	   }
	   return val;
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
	SoftWDT_Init(SWWDT_MODBUS_RTU , SWWDT_DEFAULT_TIME);

#ifdef INCLUDE_PLATFORM_DEPENDENT
	while(1)
	{
		if(Modbus__GetStatus())
			Sys__Delay(100);

		SoftWDT_Reset(SWWDT_MODBUS_RTU );

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
    Modbus__Delay();
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
    Modbus__Delay();
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
    Modbus__Delay();
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
    Modbus__Delay();
    return result;
}


// app_coil_write
int app_coil_write(const uint8_t addr, const int index, short newData, int multi)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    if (multi == SINGLE)
    	errorCode = eMBMasterReqWriteCoil( addr, index, newData,timeout);
    else
    	errorCode = eMBMasterReqWriteMultipleCoils(addr, index, 1, &newData, timeout);	// we are sending a multiple coils write even if we always write a single coil
                                                                                    	// this is just for compatibility with those devices only accepting multiple write operations
    result = errorCode;
#endif
    Modbus__Delay();
    return result;
}

// app_hr_write
int app_hr_write(const uint8_t addr, const int index, C_CHAR num_of , C_UINT16 * newData, int multi)
{
	C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    const USHORT saddr = index;

    if (multi == SINGLE)
    	errorCode = eMBMasterReqWriteHoldingRegister( addr, index, *newData, timeout );
    else
    	errorCode = eMBMasterReqWriteMultipleHoldingRegister( addr, index, num_of, newData, timeout );  // we are sending a multiple hrs write even if we can write at most 2 contiguous hrs
                                                                                                      	// this is just for compatibility with those devices only accepting multiple write operations
    result = errorCode;
#endif
    Modbus__Delay();
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
    Modbus__Delay();
    return result;
}

C_RES app_file_transfer(unsigned char* data_tx, uint8_t packet_len)
{
   C_RES result = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT

    const long timeout = MODBUS_TIME_OUT;
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    errorCode = eMBMAsterReqFileTransfer(1, data_tx, packet_len, timeout);
    result = errorCode;

    char data_rx[260];
    uint16_t data_rx_len = usMBFileTransferLen + 3;
    data_rx[0] = ucMBFileTransfer[0];
    // get response
    for(C_INT16 i = 0; i < data_rx_len + 2; i++)
   	  (data_rx[i + 1]) = 	ucMBFileTransfer[i];

    if(data_rx_len != packet_len){
    	printf("Received packet length %d doesn't match the transmitted packet length %d\n", data_rx_len, packet_len);
#ifdef __CCL_DEBUG_MODE
    	printf("uart_read_bytes len = %d\n",data_rx_len);

    	for(int i=0; i<data_rx_len; i++){
   			printf("%02X ",data_rx[i]);
   		}
   		printf("\n");
#endif
   		result = C_FAIL;
   	}else{
		if(data_rx[data_rx_len-1] != data_tx[packet_len-1] || data_rx[data_rx_len-2] != data_tx[packet_len-2]){
			printf("Received packet content doesn't match the transmitted packet content\n");
			result = C_FAIL;
		}else{
			result = C_SUCCESS;
#ifdef __CCL_DEBUG_MODE
			printf("uart_read_bytes len = %d\n",data_rx_len);

			for(int i=0; i<data_rx_len; i++){
				printf("%02X ",data_rx[i]);
			}
			printf("\n");
#endif
		}
   	}

#endif
    Modbus__Delay();
    return result;
}

void Modbus_Disable(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	eMBMasterDisable();
#endif
	ModbusDisabled = 1;
}
C_UINT16 Modbus__GetStatus(void){
	return ModbusDisabled;
}

// pay attention when calling this function... if you enable Modbus task when it is already enabled
// there s a good chance that freertos will crash...
void Modbus_Enable(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	ClearQueueMB();
	eMBMasterEnable();
	// avoid Modbus engine to stop
	vMBMasterRunResRelease();
#endif
	ModbusDisabled = 0;
}

void Modbus__ReadAddressFromNVM(void){
	C_UINT32 dev_addr;
	if(C_SUCCESS == NVM__ReadU32Value(MB_DEV_NVM, &dev_addr))
	    MB_Device = dev_addr;
	else
		MB_Device = 1;
}

void Modbus__ReadDelayFromNVM(void){
	C_UINT32 delay;
	if(C_SUCCESS == NVM__ReadU32Value(MB_DELAY_NVM, &delay))
	    MB_Delay = delay;
	else
	    MB_Delay = 0;
	PRINTF_DEBUG("MB_Delay %d\n", MB_Delay);
}

C_UINT16 Modbus__GetAddress(void){
	return MB_Device;
}

void Modbus__Delay(void){
	Sys__Delay(MB_Delay);
}
