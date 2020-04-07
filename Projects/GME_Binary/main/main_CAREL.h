/**
 * @file   main_CAREL.c
 * @author carel
 * @date 7 january 2020
 * @brief  Function implementations of main application engine
 *
 */
#include "CAREL_GLOBAL_DEF.h"
#include "binary_model.h"


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the communications errors
#define __DEBUG_MAIN_CAREL_LEV_1

//this define enable the output of others debug informations
#define __DEBUG_MAIN_CAREL_LEV_2

#endif




void Carel_Main_Task(void);

void GME__ExtractHeaderInfo(H_HeaderModel *pt);

H_HeaderModel * GME__GetHEaderInfo(void);
