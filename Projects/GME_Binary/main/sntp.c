/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/apps/sntp.h"

#include "esp_console.h"

#include "sntp.h"

static const char *TAG = "sntp";

static int sntp_time(int argc, char **argv);

static esp_console_cmd_t esp_cli_commands[] = {
    {.command = "sntp-time", 				.help = "sntp-time", 					.func = sntp_time},
};


static uint32_t last_boot_time;

static int sntp_esp_cli_commands_init(){
    int i;

    for (i = 0; i < sizeof(esp_cli_commands) / sizeof(esp_console_cmd_t); i++)
    {
            if (esp_console_cmd_register(&esp_cli_commands[i]))
            {
                return 0;
            }
    }
    return 0;

}


static int sntp_time(int argc, char **argv){

	ESP_LOGI(__func__, "invoked %d", argc);

	const char *err_str;

    if (1 == argc) {

        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        char strftime_buf[64];

        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(__func__, "The current date/time is: %s", strftime_buf);

        memset(strftime_buf, 0, 64);
        sntp_get_time(strftime_buf, 64);


    } else if (7 == argc) {

    	int val;
    	struct tm c_time;

    	/* Year */
    	val = atoi(argv[1]);
    	if (val < 1970 || val > 2111) {
    		err_str = "year should be > 1970";
    		goto error_usage;
    	}
    	c_time.tm_year = val - 1900;

    	/* Month */
    	val = atoi(argv[2]);
    	if (val < 1 || val > 12) {
    		err_str = "month should be from 1 to 12";
    		goto error_usage;
    	}
    	/* tm struct containing month value form 0-11*/
    	c_time.tm_mon = val - 1;

    	/* Day */
    	val = atoi(argv[3]);
    	if (val < 1 || val > 31) {
    		err_str = "date should be from 1 to 31";
    		goto error_usage;
    	}
    	c_time.tm_mday = val;

    	/* Hour */
    	val = atoi(argv[4]);
    	if (val < 0 || val > 23) {
    		err_str = "hour should be from 0 to 23";
    		goto error_usage;
    	}
    	c_time.tm_hour = val;

    	/* Minute */
    	val = atoi(argv[5]);
    	if (val < 0 || val > 59) {
    		err_str = "minute should be from 0 to 59";
    		goto error_usage;
    	}
    	c_time.tm_min = val;

    	/* Second */
    	val = atoi(argv[6]);
    	if (val < 0 || val > 59) {
    		err_str = "second should be from 0 to 59";
    		goto error_usage;
    	}
    	c_time.tm_sec = val;

    	time_t time = mktime(&c_time);

    	SNTP_SET_SYSTEM_TIME_US(time, 0);

    } else {
    	printf("invalid number of arguments\n");
    	return 0;
    }

    return 0;

error_usage:
	ESP_LOGI(__func__, "%s", err_str);
	return 0;

}


void Init_RTC(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_stop();
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    sntp_esp_cli_commands_init();
    last_boot_time = Get_UTC_Current_Time();

}

void sntp_get_time(char * buffer, size_t size)
{
    struct timeval now_timeval;
    gettimeofday(&now_timeval, NULL);
    struct tm timeinfo_tm;
    localtime_r(&(now_timeval.tv_sec), &timeinfo_tm);
    int len = 0;
	strftime(buffer, size, "%Y-%m-%dT%H:%M:%S", &timeinfo_tm);
	len = strlen(buffer);
    sprintf(buffer + len,".%dZ",(int)(now_timeval.tv_usec/100000));
	ESP_LOGI(__func__, "The current date/time is: %s", buffer);
}


uint32_t Get_UTC_Current_Time(void)
{
	struct timeval now_timeval;
	gettimeofday(&now_timeval, NULL);
	return (uint32_t)now_timeval.tv_sec;

}

uint32_t Get_last_boot_time(void){
	return last_boot_time;
}


void Update_RTC (void)
{

}



