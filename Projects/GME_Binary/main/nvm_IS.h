#ifndef MAIN_NVM_IS_H_
#define MAIN_NVM_IS_H_

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_partition.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
#endif

#if NVM_DEBUG == ENABLED
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
