/*
 * test_hw_CAREL.h
 *
 *  Created on:  29 Jun 2020
 *      Author: carel
 */

#ifndef _TEST_HW_CAREL_H_
#define _TEST_HW_CAREL_H_


/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"


/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */

enum{
	TEST_PRINT_MACADDR,
	TEST_TRY_CONNECT,
	TEST_WAIT_RESP_CONNECT,
	TEST_PRINT_RESP_CONNECT,
	TEST_PRINT_MODBUS,
	TEST_PRINT_IMSI,
	TEST_END,
	TEST_NUM_OF_STEP
};


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

#endif

/* ========================================================================== */
/* functions                                                                  */
/* ========================================================================== */


void test_hw(C_BYTE platform);

#endif
