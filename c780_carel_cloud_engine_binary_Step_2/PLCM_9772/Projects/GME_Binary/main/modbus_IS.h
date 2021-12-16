/**
 * @file modbus_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   this file contains some management routine of
 *          the Carel Cloud Library
 */
#include "data_types_CAREL.h"
#include "stdint.h"

#ifndef __MODBUS_IS_H
#define __MODBUS_IS_H


/* Includes ------------------------------------------------------------------*/




/* Varaibles -----------------------------------------------------------------*/


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */

#ifdef __CCL_DEBUG_MODE

//this define enable the output of the command and communications errors
//#define __DEBUG_MODBUS_INTERFACE_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_MODBUS_INTERFACE_LEV_2
#endif



/* Functions Implementation --------------------------------------------------*/
C_RES Modbus_Init(C_INT32 baud, C_SBYTE parity, C_SBYTE stopbit, C_BYTE port);
void Modbus_Task(void);
void Modbus_Task_Start(void);

void Modbus_rw_test(void);

// READ
int app_coil_read(const uint8_t addr, const int index, const int num);
int app_coil_discrete_input_read(const uint8_t addr, const int index, const int num);
int app_holding_register_read(const uint8_t addr, const int index, const int num);   //  const int func,
int app_input_register_read(const uint8_t addr, const int index, const int num);

C_RES app_report_slave_id_read(const uint8_t addr);
C_RES app_file_transfer(unsigned char* data_tx, uint8_t packet_len);

// WRITE
int app_coil_write(const uint8_t addr, const int index, short newData, int multi);
int app_hr_write(const uint8_t addr, const int index, C_CHAR num_of , C_UINT16 * newData, int multi);

void Modbus_Disable(void);
void Modbus_Enable(void);
void Modbus__ReadAddressFromNVM(void);
void Modbus__ReadDelayFromNVM(void);
C_UINT16 Modbus__GetAddress(void);
void Modbus__Delay(void);
C_UINT16 Modbus__GetStatus(void);

C_RES app_file_read(unsigned char* data_tx, uint8_t packet_len, unsigned char * data_rx);


#endif   /* #ifndef __MODBUS_IS_H */
