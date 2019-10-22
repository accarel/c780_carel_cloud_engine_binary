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
C_RES Modbus_Init(C_INT16 baud);
void Modbus_Task(void);
void Modbus_Task_Start(void);

void Modbus_rw_test(void);

int app_coil_read(const uint8_t addr, const int func, const int index, const int num);
int app_coil_discrete_input_read(const uint8_t addr, const int func, const int index, const int num);
int app_holding_register_read(const uint8_t addr, const int func, const int index, const int num);
int app_input_register_read(const uint8_t addr, const int func, const int index, const int num);




#endif   /* #ifndef __MODBUS_IS_H */
