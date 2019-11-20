// Console example custom commands
//
//	FW Ver 90.00.00


//#include "file_system.h"
#include "cmd_custom.h"
#include "wifi.h"
#include <tcpip_adapter.h>

#include "utilities_CAREL.h"

//extern EventGroupHandle_t s_wifi_event_group;

struct {
    struct arg_int *index;
    struct arg_end *end;
} hello_args;


struct {
    struct arg_int *index;
    struct arg_end *end;
} printf_mqtt_json_args;


struct {
    struct arg_int *index;
    struct arg_end *end;
} printf_poll_engine_args;


struct {
    struct arg_end *end;
} ifconfig_args;

struct {
    struct arg_end *end;
} httpclient_test_args;

#define CONFIG_STORE_HISTORY 0

#if CONFIG_STORE_HISTORY

#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"

static void initialize_filesystem()
{
    static wl_handle_t wl_handle;
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(MOUNT_PATH, "storage", &mount_config, &wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
}
#endif // CONFIG_STORE_HISTORY




static int say_hello(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &hello_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, hello_args.end, argv[0]);
        return 1;
    }

    printf("	This is Carel Gateway Project\r\n");
    printf("The following files are available in /spiffs directory");
    FS_DisplayFiles();
    return 0;
}



static int printf_poll_engine_func(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &printf_poll_engine_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, printf_poll_engine_args.end, argv[0]);
        return 1;
    }

    if (printf_poll_engine_args.index->ival[0] == 1){
 //   	PollEngine__SetPollEnginePrintMsgs(1);
		printf("	Printing Polling Engine data is ENABLED\r\n");
	}

	else if (printf_poll_engine_args.index->ival[0] == 0){
//		PollEngine__SetPollEnginePrintMsgs(0);
		printf("	Printing Polling Engine data is DISABLED\r\n");
	}

	else{
		printf("	Enter 1 to print Polling Engine data or 0 to hide them\r\n");
	}

    return 0;
}

static int printf_mqtt_json_func(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &printf_mqtt_json_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, printf_mqtt_json_args.end, argv[0]);
        return 1;
    }

    if (printf_mqtt_json_args.index->ival[0] == 1){
//    	JSON__SetJsonMqttPrintMsgs(1);
        printf("	Printing MQTT and JSON msgs is ENABLED\r\n");
    }

    else if (printf_mqtt_json_args.index->ival[0] == 0){
//    	JSON__SetJsonMqttPrintMsgs(0);
    	printf("	Printing MQTT and JSON msgs is DISABLED\r\n");
    }

    else{
        printf("	Enter 1 to print MQTT and JSON msgs or 0 to hide them\r\n");
    }

    return 0;
}


static int ifconfig_wifi(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &ifconfig_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, ifconfig_args.end, argv[0]);
        return 1;
    }

    html_config_param_t wifi_config = WiFi__GetCustomConfig();

    printf("\n");
    printf("MAC ADDRESS:      %s\n",Utilities__GetMACAddr());
    printf("\n");
    if(AP_MODE == wifi_config.gateway_mode){
    	printf("GME MODE: AP Mode\n");
    }else{
    	printf("GME Mode: APSTA Mode\n");
    }
    printf("\n-----------------------\n");
    printf("AP Mode Parameters\n");
    printf("-----------------------\n");
    printf("SSID:                  %s",wifi_config.ap_ssid);
    if(1 == wifi_config.ap_ssid_hidden){
    	printf(" ... Hidden\n");
    }else{
    	printf(" ... Visible\n");
    }
    printf("Password:              %s\n",wifi_config.ap_pswd);
    printf("IPv4:                  %s\n",wifi_config.ap_ip);
    if(1 == wifi_config.ap_dhcp_mode){
    	printf("DHCP: Enabled\n");
    	printf("DHCP IPv4:         %s\n",wifi_config.ap_dhcp_ip);
    }else{
    	printf("DHCP: Disabled\n");
    }

    if(APSTA_MODE == wifi_config.gateway_mode){
    	printf("\n-----------------------\n");
    	printf("STA Mode Parameters\n");
    	printf("-----------------------\n\n");
		printf("SSID:              %s\n",wifi_config.sta_ssid);
		printf("Encryption:        %s\n",wifi_config.sta_encryption);
		printf("Password:          %s\n",wifi_config.sta_pswd);

    tcpip_adapter_ip_info_t sta_ip;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &sta_ip);

		if(1 == wifi_config.sta_dhcp_mode){
			printf("DHCP: Enabled\n");
			printf("IPv4:          %s\n", ip4addr_ntoa(&sta_ip.ip));
			printf("Gateway IPv4:  %s\n", ip4addr_ntoa(&sta_ip.gw));
			printf("Netmask:       %s\n", ip4addr_ntoa(&sta_ip.netmask));

		}else{
			printf("DHCP: Disabled\n");
			printf("Static IP:     %s\n",wifi_config.sta_static_ip);
			printf("Netmask:       %s\n",wifi_config.sta_netmask);
			printf("Gateway IPv4:  %s\n",wifi_config.sta_gateway_ip);
			printf("Primary DNS:   %s\n",wifi_config.sta_primary_dns);
			printf("Secondary DNS: %s\n",wifi_config.sta_secondary_dns);
		}
    }

    return 0;
}


static int httpclient_test(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &ifconfig_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, ifconfig_args.end, argv[0]);
        return 1;
    }


    WiFi__WaitConnection();

 //   xTaskCreate(&HTTPClient__TestTask, "http_test_task", 8192, NULL, 5, NULL);

    return 0;
}

static void register_hello(void)
{

    //arg_int0 can work without parameters because it use the default value
    //arg_int1 can't work without parameters, that is why it doesn't need intialize the value
    
    hello_args.index = arg_int1(NULL, NULL, "<i>", "Phrase Index");
    hello_args.end = arg_end(1);

    const esp_console_cmd_t hello_cmd = {
        .command = "hello",
        .help = "Check on spiffs files",
		.hint = NULL,
        .func = &say_hello,
        .argtable = &hello_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&hello_cmd) );
}

static void register_printf_mqtt_json(void)
{

    //arg_int0 can work without parameters because it use the default value
    //arg_int1 can't work without parameters, that is why it doesn't need intialize the value

	printf_mqtt_json_args.index = arg_int1(NULL, NULL, "<i>", "Phrase Index");
	printf_mqtt_json_args.end = arg_end(1);

    const esp_console_cmd_t mqtt_json_cmd = {
        .command = "debug_mqtt_json",
        .help = "This will printf the json msgs and mqtt communications",
		.hint = NULL,
        .func = &printf_mqtt_json_func,
        .argtable = &printf_mqtt_json_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&mqtt_json_cmd) );
}


static void register_printf_poll_engine(void)
{

    //arg_int0 can work without parameters because it use the default value
    //arg_int1 can't work without parameters, that is why it doesn't need intialize the value

	printf_poll_engine_args.index = arg_int1(NULL, NULL, "<i>", "Phrase Index");
	printf_poll_engine_args.end = arg_end(1);

    const esp_console_cmd_t poll_engine_cmd = {
        .command = "debug_poll_engine",
        .help = "This will printf the polling engine data, polling, values, buffers, etc..",
		.hint = NULL,
        .func = &printf_poll_engine_func,
        .argtable = &printf_poll_engine_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&poll_engine_cmd) );
}

static void register_ifconfig(void)
{
    ifconfig_args.end = arg_end(1);
    const esp_console_cmd_t cmd_ifconfig = {
        .command = "ifconfig",
        .help = "Show WiFi interface Configuration",
		.hint = NULL,
        .func = &ifconfig_wifi,
        .argtable = &ifconfig_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd_ifconfig) );
}


static void register_httpclient_test(void)
{
	httpclient_test_args.end = arg_end(2);
    const esp_console_cmd_t cmd_httpclient_test = {
        .command = "http_test",
        .help = "Test HTTP Client",
		.hint = NULL,
        .func = &httpclient_test,
        .argtable = &httpclient_test_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd_httpclient_test) );
}

void register_custom(void)
{
	register_ifconfig();
    register_hello();
    register_printf_mqtt_json();
    register_printf_poll_engine();
    register_httpclient_test();
}

void  cmdline_handler(void *arg)
//void  cmdline_handler(void)
{
	/* Main loop */
	char* prompt = LOG_COLOR_I "CarelGME> " LOG_RESET_COLOR;
	while(1){
		/* Get a line using linenoise.
		 * The line is returned when ENTER is pressed.
		 */
		char* line = linenoise(prompt);
		if (line == NULL) { /* Ignore empty lines */
			continue;
		}
		/* Add the command to the history */
		linenoiseHistoryAdd(line);
	#if CONFIG_STORE_HISTORY
		/* Save command history to filesystem */
		linenoiseHistorySave(HISTORY_PATH);
	#endif

		/* Try to run the command */
		int ret;
		esp_err_t err = esp_console_run(line, &ret);
		if (err == ESP_ERR_NOT_FOUND) {
			printf("Unrecognized command\n");
		} else if (err == ESP_ERR_INVALID_ARG) {
			// command was empty
		} else if (err == ESP_OK && ret != ESP_OK) {
			printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(err));
		} else if (err != ESP_OK) {
			printf("Internal error: %s\n", esp_err_to_name(err));
		}
		/* linenoise allocates line buffer on the heap, so need to free it */
		linenoiseFree(line);
	}
}



void initialize_cmdline(void)
{
	char* prompt = LOG_COLOR_I "CarelGME> " LOG_RESET_COLOR;
	/* Register commands */
	register_system();
	register_custom();
	//register_wifi();
	esp_console_register_help_command();

	/* Prompt to be printed before each line.
	 * This can be customized, made dynamic, etc.
	 */


	/* Figure out if the terminal supports escape sequences */
	int probe_status = linenoiseProbe();
	if (probe_status) { /* zero indicates success */
		printf("\n"
			   "Your terminal application does not support escape sequences.\n"
			   "Line editing and history features are disabled.\n"
			   "On Windows, try using Putty instead.\n");
		linenoiseSetDumbMode(1);
	//#if CONFIG_LOG_COLORS
		/* Since the terminal doesn't support escape sequences,
		 * don't use color codes in the prompt.
		 */
		prompt = "CarelGME> ";
	}
	//#endif //CONFIG_LOG_COLORS

	//cmdline_handler();

}



void initialize_console(void)
{
    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    const uart_config_t uart_config = {
            .baud_rate = CONFIG_CONSOLE_UART_BAUDRATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .use_ref_tick = true
    };
    ESP_ERROR_CHECK( uart_param_config(CONFIG_CONSOLE_UART_NUM, &uart_config) );

    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(CONFIG_CONSOLE_UART_NUM,
            256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(CONFIG_CONSOLE_UART_NUM);

    /* Initialize the console */
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
#if CONFIG_LOG_COLORS
            .hint_color = atoi(LOG_COLOR_CYAN)
#endif
    };
    ESP_ERROR_CHECK( esp_console_init(&console_config) );

    /* Configure linenoise line completion library */
    /* Enable multiline editing. If not set, long commands will scroll within
     * single line.
     */
    linenoiseSetMultiLine(1);

    /* Tell linenoise where to get command completions and hints */
    linenoiseSetCompletionCallback(&esp_console_get_completion);    //Tab Compilation
    linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);

    /* Set command history size */
    linenoiseHistorySetMaxLen(100);

#if CONFIG_STORE_HISTORY
    /* Load command history from filesystem */
    linenoiseHistoryLoad(HISTORY_PATH);
#endif


}



