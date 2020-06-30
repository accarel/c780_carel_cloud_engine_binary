/*
  Description : CAREL_GLOBAL_DEF.H

  Scope       : used to customize the library
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAREL_GLOBAL_DEF
#define __CAREL_GLOBAL_DEF


#include "data_types_CAREL.h"


/* ========================================================================== */
/* DEVELOPMENT / TARGET PLATFORM                                              */
/* ========================================================================== */
/**
 * @brief DEVELOPMENT / TARGET PLATFORM
 *        define which target platform do you want to use
 *        REMEMBER Only one.
 */

/* The platform is determined by an I/O pin on 2G e WiFi model so that
 * the same FW runs on both platforms.
 * ONLY for the bCU the platform must be set manually and generate a
 * dedicated FW.
 * */

#define PLATFORM_DETECTED_WIFI  1
#define PLATFORM_DETECTED_2G    2
#define PLATFORM_DETECTED_BCU   3
#define PLATFORM_DETECTED_ESP_WROVER_KIT  4
#define PLATFORM_DETECTED_TEST_MODE 0x8000

/* ========================================================================== */
/* uncomment for development platform                                         */
/* ========================================================================== */
//#define __USE_ESP_WROVER_KIT

/* ========================================================================== */
/*  uncomment for bcu PLATFORM                                                */
/* ========================================================================== */
//#define __USE_CAREL_BCU_HW

/* ========================================================================== */
/*  for WiFi ad 2G platforms comment both above defines                       */
/*  model recognition will happen automatically	                              */
/* ========================================================================== */

/**
 * @brief INCLUDE_PLATFORM_DEPENDENT
 *        you MUST enable this define as soon you have implemented all the
 *        platform dependent routines.
 *        undef it to test you specific compiler and understand if all right.
 *        WARNING! this define MUST be DEFINED in the release version of the FW
 */
#define INCLUDE_PLATFORM_DEPENDENT 1

/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */

/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
/**
 * @brief __CCL_DEBUG_MODE 
 *        if defined enable print on the debug console some debugger message 
 *        take care to enable the _DEBUG_filename locally in each file you want
 *        to debug. 
 *        WARNING! remember that the debug output take elaboration time 
 *        WARNING! this define MUST be DISABLE in the release version of the FW
 */
#define __CCL_DEBUG_MODE

#ifdef __CCL_DEBUG_MODE
	#define	PRINTF_DEBUG	printf
#else
	#define	PRINTF_DEBUG(...)
#endif

/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */
#define FALSE 0
#define TRUE 1

/* ========================================================================== */
/* Device information                                                         */
/* ========================================================================== */
#define GW_WIFI_PARTNUMBER 	"GTW000MWT0"
#define GW_GSM_PARTNUMBER 	"GTW000MGP0"

#define GW_HW_REV  "100"
#define GW_FW_REV  "100"

/* ========================================================================== */
/* OS related                                                                 */
/* ========================================================================== */

/** @brief SYSTEM_TIME_TICK  system tick of the OS expressed in ms
 *                           if used in non OS system this is the incremet of 
 *                           tick time 
*/

#define SYSTEM_TIME_TICK	1

/* ========================================================================== */
/* Cloud related                                                              */
/* ========================================================================== */

#define MQTT_KEEP_ALIVE_DEFAULT_SEC   (60)

#define NTP_DEFAULT_PORT  	123

// period for status payload transmission
#define GW_MOBILE_TIME		60	//TODO temporarily set to 60s, should be 7200s (2 hours)
//period for mobile payload transmission
#define GW_STATUS_TIME		60	//TODO temporarily set to 60s, should be 7200s (2 hours)
#endif

