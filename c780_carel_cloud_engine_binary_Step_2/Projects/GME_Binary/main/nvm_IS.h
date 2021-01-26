#ifndef MAIN_NVM_IS_H_
#define MAIN_NVM_IS_H_

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_partition.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
#endif


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

 //this define enable the output of the errors
#define __DEBUG_NVM_IS_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_NVM_IS_LEV_2

#endif

#ifdef __DEBUG_NVM_IS_LEV_2
	#define	PRINTF_DEBUG_NVM	printf
#else
	#define	PRINTF_DEBUG_NVM(...)
#endif





C_RES NVM_Init(void);
C_RES NVM__Open(void);
void NVM__Close(void);
C_RES NVM__GetU8(const C_CHAR* var, C_BYTE* val);
C_RES NVM__GetU32(const C_CHAR* var, C_UINT32* val);
C_RES NVM__GetString(const C_CHAR* var, C_CHAR* str, size_t* len);
C_RES NVM__SetU8(const C_CHAR* var, C_BYTE val);
C_RES NVM__SetU32(const C_CHAR* var, C_UINT32 val);
C_RES NVM__SetString(const C_CHAR* var, C_CHAR* str);
C_RES NVM__GetBlob(const C_CHAR* var, void* vec, size_t* len);
C_RES NVM__SetBlob(const C_CHAR* var, void* vec, size_t len);
C_RES NVM__EraseK(const C_CHAR* var);
C_RES NVM__Erase(void);

#endif
