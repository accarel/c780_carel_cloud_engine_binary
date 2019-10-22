/*
 * common.h
 *
 *  Created on: Jun 19, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#include "esp_err.h"
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"

#include <esp_http_server.h>
#include "lwip/err.h"
#include "lwip/sys.h"

//#include "cmd_custom.h"
#include "tcpip_adapter.h"

#include "gme_types.h"
#include"gme_config.h"

#include "CAREL_GLOBAL_DEF.h"
#include"memmgr.h"
#include "sntp.h"

#endif /* COMMON_H_ */
