/*
  File  : SoftWDT
  
  Scope : all the routine that manage the SW watchdog

*/

/* Define to prevent recursive inclusion */
#ifndef _SOFTWDT
#define _SOFTWDT

/* ==== Include ==== */

/* ==== Define fixed don't touch it ==== */
typedef struct tag_softwdt_struct
{
        uint16_t is_enabled;
        uint16_t wdt_counter_preset;
	uint16_t wdt_counter_value;					
}softwdt_struct;


/* ==== Define customizable ==== */

//#define SWWDT_DEFAULT_TIME      3000  /* millisec. */
#define SWWDT_DEFAULT_TIME        20    /*       sec. */


#define SWWDT_PCO_EXCHANGE 0
#define SWWDT_MAIN_DEVICE  1
#define SWWDT_MODBUS_RTU   2
#define SWWDT_POLLING      3
#define SWWDT_LED          4
#define SWWDT_FREE         5
#define MAX_SOFTWDT 	   6

/* ==== Global Variables ==== */


/* ==== Function prototype ==== */
void SoftWDT_Init(uint8_t which_one, uint16_t value);
void SoftWDT_DeInit(uint8_t which_one);
void SoftWDT_Reset(uint8_t which_one);
void SoftWDT_Manager(void);


#endif //_SOFTWDT
