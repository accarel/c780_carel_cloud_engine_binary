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




/* Functions Implementation --------------------------------------------------*/
C_RES Modbus_Init(C_INT32 baud, C_SBYTE parity, C_SBYTE stopbit, C_BYTE port);
void Modbus_Task(void);
void Modbus_Task_Start(void);

void Modbus_rw_test(void);

// READ
int app_coil_read(const uint8_t addr, const int func, const int index, const int num);
int app_coil_discrete_input_read(const uint8_t addr, const int func, const int index, const int num);
int app_holding_register_read(const uint8_t addr, const int func, const int index, const int num);
int app_input_register_read(const uint8_t addr, const int func, const int index, const int num);

C_RES app_report_slave_id_read(const uint8_t addr);

// WRITE
int app_coil_write(const uint8_t addr, const int index, short newData);
int app_hr_write(const uint8_t addr, const int index, C_CHAR num_of , C_UINT16 * newData);

void Modbus_Disable(void);
void Modbus_Enable(void);
void Modbus__ReadAddressFromNVM(void);
void Modbus__ReadDelayFromNVM(void);
C_UINT16 Modbus__GetAddress(void);
void Modbus__Delay(void);
#endif   /* #ifndef __MODBUS_IS_H */
