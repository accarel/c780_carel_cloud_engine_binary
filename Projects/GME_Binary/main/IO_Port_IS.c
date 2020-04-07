/**
 * @file   IO_Port_IS.c
 * @author carel
 * @date 9 May 2020
 * @brief  functions implementations specific related to the managment of the
 *         I/O Port. 
 *
 */
 
/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#endif


#include "IO_Port_IS.h"


/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */



/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */



/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */
/**
 * @brief Check_HW_Platform_IS
 *        read the pin that return the HW platform
 * @return platform
 */

void Configure_IO_Check_HW_Platform_IS(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  gpio_pad_select_gpio(HW_PLATFORM_DETECT_PIN);
  gpio_set_direction(HW_PLATFORM_DETECT_PIN, GPIO_MODE_INPUT);
  #endif
}

/**
 * @brief Check_HW_Platform_IS
 *        read the pin that return the HW platform
 * @return platform
 */
C_BYTE Check_HW_Platform_IS(void)
{
	C_INT16 platform = 0;

    #ifdef INCLUDE_PLATFORM_DEPENDENT

    #ifdef  __USE_CAREL_BCU_HW
	platform = PLATFORM_DETECTED_BCU;
    #ifdef __DEBUG_IO_PORT_IS_LEV_2
	printf("__USE_CAREL_BCU_HW \r\n");
    #endif
    #endif

    #ifdef 	__USE_ESP_WROVER_KIT
	platform = PLATFORM_DETECTED_ESP_WROVER_KIT;
    #ifdef __DEBUG_IO_PORT_IS_LEV_2
	printf("__USE_ESP_WROVER_KIT \r\n");
    #endif
    #endif

    #ifdef __USE_USR_2G_HW || __USE_USR_WIFI_HW
	/* pin is pull down on 2G and pull upped on WiFi*/
	if (gpio_get_level(HW_PLATFORM_DETECT_PIN) == 0)
	{
		platform = PLATFORM_DETECTED_2G;
        #ifdef __DEBUG_IO_PORT_IS_LEV_2
	    printf("__USE_USR_2G_HW \r\n");
        #endif
	}
	else
	{
		platform = PLATFORM_DETECTED_WIFI;
        #ifdef __DEBUG_IO_PORT_IS_LEV_2
		printf("__USE_USR_WIFI_HW \r\n");
        #endif
	}
    #endif

    #endif

  return platform;
}



