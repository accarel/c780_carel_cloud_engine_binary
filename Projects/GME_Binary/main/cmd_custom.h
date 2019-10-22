/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   FW Ver 90.00.00


*/
#pragma once

#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include <esp_event_loop.h>
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"
#include "cmd_system.h"

#ifdef __cplusplus
extern "C" {
#endif

// Register Custom functions
void initialize_console(void);
void initialize_cmdline(void);
void cmdline_handler(void *arg);
void register_custom(void);

#ifdef __cplusplus
}
#endif
