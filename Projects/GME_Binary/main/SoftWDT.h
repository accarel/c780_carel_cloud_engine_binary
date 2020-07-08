/**
 * @file   SoftWDT.c
 * @author carel
 * @date   9 Sep 2019
 * @brief  a softwd implementation to monitor the system
 *
 */

/* Define to prevent recursive inclusion */
#ifndef _SOFTWDT_H
#define _SOFTWDT_H

/* ==== Include ==== */

/* ==== Define fixed don't touch it ==== */
typedef struct tag_softwdt_struct
{
        uint16_t is_enabled;
        uint16_t wdt_counter_preset;
	uint16_t wdt_counter_value;					
}softwdt_struct;


/* ==== Define customizable ==== */
/**
 * @brief SWWDT_DEFAULT_TIME
 *        the time in sec. of the SW WD timers
*/
#define SWWDT_DEFAULT_TIME        20    /*       sec. */


/* the monitored FW zones */
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
