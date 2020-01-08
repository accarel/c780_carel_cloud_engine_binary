/*
 * httpserver.c
 *
 *  Created on: Jun 17, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#include "http_server.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"
#include "nvm_CAREL.h"
#include "cJSON.h"
#include "wifi.h"
//#include "file_system.h"
#include "utilities_CAREL.h"
#include <ctype.h>

static html_config_param_t HTMLConfig = {
		.gateway_mode = 0,
		.ap_ssid = AP_DEF_SSID,
		.ap_pswd = AP_DEF_PSSWD,
		.ap_ssid_hidden = AP_DEF_SSID_HIDDEN,
		.ap_ip = AP_DEF_IP,
		.ap_dhcp_mode = AP_DEF_DHCP,
		.ap_dhcp_ip = AP_DEF_DHCP_BASE,
		.sta_ssid = "",
		.sta_encryption = "",
		.sta_pswd= "",
		.sta_dhcp_mode = 0,
		.sta_static_ip = "",
		.sta_netmask = "",
		.sta_gateway_ip = "",
		.sta_primary_dns = "",
		.sta_secondary_dns = "",
		.ntp_server_addr = "",
		.ntp_server_port = "",
		.mqtt_server_addr = "",
		.mqtt_server_port = ""
};

static html_login_cred_t HTMLLogin = {
		.login_usr = "",
		.login_pswd = ""
};

static char ap_ssid_def[30] = {0};
static uint8_t ReceivedConfig = 0;

static html_pages LastPageSent = 0;
// Max length a file path can have on storage
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)


// Scratch buffer size 
#define SCRATCH_BUFSIZE  8192


#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

struct file_server_data {
    // Base path of file storage 
    char base_path[ESP_VFS_PATH_MAX + 1];

    // Scratch buffer for temporary storage during file transfer 
    char scratch[SCRATCH_BUFSIZE];
};

static const char *TAG = "http_server";

//Private Functions Definition
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename);
static void HTTPServer__ParseCredfromNVM(void);



//Private Functions Declaration
void url_decoder(char *buf) {

    char eStr[3] = "00"; /* for a hex code */

    int i; /* the counter for the string */

    for(i=0;i<strlen(buf);++i) {

        if(buf[i] == '%') {
        if(buf[i+1] == 0)
            return;

        if(isxdigit((int)buf[i+1]) && isxdigit((int)buf[i+2])) {

            /* combine the next to numbers into one */
            eStr[0] = buf[i+1];
            eStr[1] = buf[i+2];

            /* convert it to decimal */
            long int x = strtol(eStr, NULL, 16);

            /* remove the hex */
            memmove(&buf[i+1], &buf[i+3], strlen(&buf[i+3])+1);

            buf[i] = x;
        }
        }
        else if(buf[i] == '+') { buf[i] = ' '; }
    }
}


// Handler to respond with file requested.
static esp_err_t file_get_handler(httpd_req_t *req, const char* filename)
{

	struct stat file_stat;
	FILE *fd = NULL;


	fd = fopen(filename, "r");

	if(stat(filename, &file_stat) == -1){
		ESP_LOGE(TAG, "Failed to stat dir : %s", filename);
		httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Directory does not exist");
		return ESP_FAIL;
	}else{


		ESP_LOGI(TAG, "Sending file : %s (%ld bytes)...", filename, file_stat.st_size);
		set_content_type_from_file(req, filename);

		/* Retrieve the pointer to scratch buffer for temporary storage */
		char *chunk = ((struct file_server_data *)req->user_ctx)->scratch;
		size_t chunksize;
		do {
			PRINTF_DEBUG_SERVER("%s LOADING ...\n",filename);
			/* Read file in chunks into the scratch buffer */
			chunksize = fread(chunk, 1, SCRATCH_BUFSIZE, fd);

			/* Send the buffer contents as HTTP response chunk */
			if (httpd_resp_send_chunk(req, chunk, chunksize) != ESP_OK) {
				fclose(fd);
				ESP_LOGE(TAG, "File sending failed!");
				/* Abort sending file */
				httpd_resp_sendstr_chunk(req, NULL);
				/* Respond with 500 Internal Server Error */
				httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
				return ESP_FAIL;
			}

		} while (chunksize != 0);

	}

	fclose(fd);

	httpd_resp_sendstr_chunk(req, NULL);


	if(0 == strcmp(filename,LOGIN_HTML)){
		LastPageSent = LOGIN;
	}else if (0 == strcmp(filename,CONFIG_HTML)){
		LastPageSent = CONFIG;
	}else if (0 == strcmp(filename,CHANGE_CRED_HTML)){
		LastPageSent = CHANGE_CRED;
	}

    return ESP_OK;
}

//Send config.json
static esp_err_t http_resp_config_json(httpd_req_t *req)
{
    //Get config values into a json struct

	char *out;
	char ap_ssid_temp[30] = {0};
	size_t len=0;
	cJSON *html_config;
	html_config_param_t *wifi_config;
	html_config = cJSON_CreateObject();
	wifi_config = WiFi__GetCustomConfigPTR();




	if(ESP_OK == NVM__ReadString(HTMLCONF_AP_SSID, ap_ssid_temp, &len)){
		cJSON_AddItemToObject(html_config, HTMLCONF_AP_SSID, cJSON_CreateString(wifi_config->ap_ssid));
	}else{
		strcpy(ap_ssid_temp, HTTPServer__SetAPDefSSID(AP_DEF_SSID));
		cJSON_AddItemToObject(html_config, HTMLCONF_AP_SSID, cJSON_CreateString(ap_ssid_temp));
		PRINTF_DEBUG_SERVER("ap_ssid_temp : %s\n",ap_ssid_temp);
	}

	cJSON_AddItemToObject(html_config, HTMLCONF_AP_SSID_HIDDEN, cJSON_CreateBool((bool)wifi_config->ap_ssid_hidden));
	cJSON_AddItemToObject(html_config, HTMLCONF_AP_PSWD, cJSON_CreateString(wifi_config->ap_pswd));
	cJSON_AddItemToObject(html_config, HTMLCONF_AP_IP, cJSON_CreateString(wifi_config->ap_ip));

	cJSON_AddItemToObject(html_config, HTMLCONF_AP_DHCP_MODE, cJSON_CreateBool((bool)wifi_config->ap_dhcp_mode));
	if(0 == wifi_config->ap_dhcp_mode){
		cJSON_AddItemToObject(html_config, HTMLCONF_AP_DHCP_IP, cJSON_CreateNull());
	}else{
		cJSON_AddItemToObject(html_config, HTMLCONF_AP_DHCP_IP, cJSON_CreateString(wifi_config->ap_dhcp_ip));
		}

	cJSON_AddItemToObject(html_config, HTMLCONF_STA_SSID, cJSON_CreateString(wifi_config->sta_ssid));
	cJSON_AddItemToObject(html_config, HTMLCONF_STA_PSWD, cJSON_CreateString(wifi_config->sta_pswd));

	cJSON_AddItemToObject(html_config, HTMLCONF_STA_DHCP_MODE, cJSON_CreateBool((bool)wifi_config->sta_dhcp_mode));
	if(0 == wifi_config->sta_dhcp_mode){
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_STATIC_IP, cJSON_CreateString(wifi_config->sta_static_ip));
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_NETMASK, cJSON_CreateString(wifi_config->sta_netmask));
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_GATEWAY_IP, cJSON_CreateString(wifi_config->sta_gateway_ip));
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_PRI_DNS, cJSON_CreateString(wifi_config->sta_primary_dns));
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_SCND_DNS, cJSON_CreateString(wifi_config->sta_secondary_dns));
	}else{
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_STATIC_IP, cJSON_CreateNull());
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_NETMASK, cJSON_CreateNull());
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_GATEWAY_IP, cJSON_CreateNull());
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_PRI_DNS, cJSON_CreateNull());
		cJSON_AddItemToObject(html_config, HTMLCONF_STA_SCND_DNS, cJSON_CreateNull());
	}

	cJSON_AddItemToObject(html_config, HTMLCONF_NTP_SRVR_ADDR, cJSON_CreateString(wifi_config->ntp_server_addr));
	cJSON_AddItemToObject(html_config, HTMLCONF_NTP_SRVR_PORT, cJSON_CreateString(wifi_config->ntp_server_port));

	cJSON_AddItemToObject(html_config, HTMLCONF_MQTT_SRVR_ADDR, cJSON_CreateString(wifi_config->mqtt_server_addr));
	cJSON_AddItemToObject(html_config, HTMLCONF_MQTT_SRVR_PORT, cJSON_CreateString(wifi_config->mqtt_server_port));



	/* print everything */
	out = cJSON_Print(html_config);
	PRINTF_DEBUG_SERVER("html_config.json:%s\n", out);


    httpd_resp_set_type(req, "application/json");
    /* Add file upload form and script which on execution sends a POST request to /upload */

    httpd_resp_send_chunk(req, (const char *)out, strlen(out));

    httpd_resp_sendstr_chunk(req, NULL);


	/* free all objects under root and root itself */
	cJSON_Delete(html_config);

    return ESP_OK;
}

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename)
{
    if (IS_FILE_EXT(filename, ".html")) {
        return httpd_resp_set_type(req, "text/html");
    } else if (IS_FILE_EXT(filename, ".jpeg")) {
        return httpd_resp_set_type(req, "image/jpeg");
    } else if (IS_FILE_EXT(filename, ".ico")) {
        return httpd_resp_set_type(req, "image/x-icon");
    }else if (IS_FILE_EXT(filename, ".css")) {
        return httpd_resp_set_type(req, "text/css");
    }else if (IS_FILE_EXT(filename, ".json")) {
        return httpd_resp_set_type(req, "application/javascript");
    }
    /* This is a limited set only */
    /* For any other type always set as plain text */
    return httpd_resp_set_type(req, "text/plain");
}

/* Copies the full path into destination buffer and returns
 * pointer to path (skipping the preceding base path) */
static const char* get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize)
{
	PRINTF_DEBUG_SERVER("base_path: %s \n", base_path);
    const size_t base_pathlen = strlen(base_path);
    size_t pathlen = strlen(uri);

    const char *quest = strchr(uri, '?');
    if (quest) {
        pathlen = MIN(pathlen, quest - uri);
    }
    const char *hash = strchr(uri, '#');
    if (hash) {
        pathlen = MIN(pathlen, hash - uri);
    }

    if (base_pathlen + pathlen + 1 > destsize) {
        /* Full path string won't fit into destination buffer */
        return NULL;
    }

    /* Construct full path (base + path) */
    strcpy(dest, base_path);
    strlcpy(dest + base_pathlen, uri, pathlen + 1);

    /* Return pointer to path, skipping the base */
    return dest + base_pathlen;
}

/* Handler to download a file kept on the server */
static esp_err_t download_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    FILE *fd = NULL;
    struct stat file_stat;

    const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path,
                                             req->uri, sizeof(filepath));
    ESP_LOGI(TAG, "DOWNLOAD_get_handler");


    if (!filename) {
        ESP_LOGE(TAG, "Filename is too long");
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
        return ESP_FAIL;
    }

    /* If name has trailing '/', respond with directory contents */
    if (filename[strlen(filename) - 1] == '/') {
    	uint8_t cred_conf = DEFAULT;
    	if(ESP_OK == NVM__ReadU8Value(HTMLLOGIN_CONF_NVM, &cred_conf) && (cred_conf == CONFIGURED)){
    		return file_get_handler(req, LOGIN_HTML);
    	}else{
    		return file_get_handler(req, CHANGE_CRED_HTML);
    	}
    }


    PRINTF_DEBUG_SERVER("Requested path = %s\n",filename);
    if (stat(filepath, &file_stat) == 0) {
        /* If file not present on SPIFFS check if URI
         * corresponds to one of the hardcoded paths */
        if (strcmp(filename, "/login.html") == 0) {
        	httpd_resp_set_status(req, "303 See Other");
            return httpd_resp_set_hdr(req, "Location", "/");
        }
        else if (strcmp(filename, "/config.html") == 0) {
        	return file_get_handler(req, CONFIG_HTML);
        }
        else if (strcmp(filename, "/fav.ico") == 0) {
            return file_get_handler(req, FAV_ICON);
        }
        else if (strcmp(filename, "/style.css") == 0){
        	return file_get_handler(req, STYLE_CSS);
        }
    }else{
    	if (strcmp(filename, "/config.json") == 0){
			return http_resp_config_json(req);
		}

		ESP_LOGE(TAG, "Failed to stat file : %s", filepath);
		/* Respond with 404 Not Found */
		httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
		return ESP_FAIL;
    }

    fd = fopen(filepath, "r");
    if (!fd) {
        ESP_LOGE(TAG, "Failed to read existing file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }


    ESP_LOGI(TAG, "File sending complete");

    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}


static void get_value_from_string(char* received_buf, const char* value_key, unsigned short value_key_len, char* req_value)
{
	char *key_buff = strstr(received_buf, value_key);

	if(key_buff != NULL)
	{
		char *and_key_buff = strstr(key_buff, "&");
		key_buff += (unsigned int)value_key_len + 1;
		unsigned char len = (unsigned char)(and_key_buff - key_buff);
		memcpy((void*)req_value,(void*)key_buff ,(len));
		req_value[len]='\0';
	}
	else{
		req_value[0]='\0';
	}
}


static int get_html_change_credentials(char* sent_parameters){

    char data_value[100];
    memset(data_value,0,strlen(data_value)*sizeof(char));
    PRINTF_DEBUG_SERVER("\nSTART PARSING\n");

    html_login_cred_t html_login;

    //username
    get_value_from_string(sent_parameters, HTMLLOGIN_USR, (unsigned short)(strlen(HTMLLOGIN_USR)), &data_value[0]);
    strcpy(html_login.login_usr,data_value);
    PRINTF_DEBUG_SERVER("change cred login_usr: %s\n",html_login.login_usr);

    //password
    get_value_from_string(sent_parameters, HTMLLOGIN_PSWD, (unsigned short)(strlen(HTMLLOGIN_PSWD)), &data_value[0]);
    strcpy(html_login.login_pswd,data_value);
    PRINTF_DEBUG_SERVER("change cred login_pswd: %s\n",html_login.login_pswd);


    if ((strlen(html_login.login_usr) > 0) && (strlen(html_login.login_pswd) > 0)){

    	esp_err_t err1, err2;
    	err1 = NVM__WriteString(HTMLLOGIN_USR, html_login.login_usr);
    	err2 = NVM__WriteString(HTMLLOGIN_PSWD, html_login.login_pswd);

    	if(ESP_OK == err1 && ESP_OK == err2){
    		NVM__WriteU8Value(HTMLLOGIN_CONF_NVM, CONFIGURED);
    		return 1;
    	}else{
    		return 0;
    	}
    }

    return 0;
}

static void HTTPServer__ParseCredfromNVM(void){
	size_t len = 0;
	NVM__ReadString(HTMLLOGIN_USR,HTMLLogin.login_usr,&len);
	NVM__ReadString(HTMLLOGIN_PSWD,HTMLLogin.login_pswd,&len);
}

static int check_html_credentials(char* sent_parameters){

    char data_value[100];
    memset(data_value,0,strlen(data_value)*sizeof(char));
    PRINTF_DEBUG_SERVER("\nSTART PARSING\n");

    html_login_cred_t html_login;


    //username
    get_value_from_string(sent_parameters, HTMLLOGIN_USR, (unsigned short)(strlen(HTMLLOGIN_USR)), &data_value[0]);
    strcpy(html_login.login_usr,data_value);
    PRINTF_DEBUG_SERVER("login_usr: %s\n",html_login.login_usr);

    //password
    get_value_from_string(sent_parameters, HTMLLOGIN_PSWD, (unsigned short)(strlen(HTMLLOGIN_PSWD)), &data_value[0]);
    strcpy(html_login.login_pswd,data_value);
    PRINTF_DEBUG_SERVER("login_pswd: %s\n",html_login.login_pswd);


    if ((strcmp(html_login.login_usr, HTMLLogin.login_usr) == 0) && (strcmp(html_login.login_pswd, HTMLLogin.login_pswd) == 0)){
    	return 1;
    }


    return 0;
}


static void get_html_config_received_data(char* sent_parameters){
    
    char data_value[100];
    memset(data_value,0,strlen(data_value)*sizeof(char));

    PRINTF_DEBUG_SERVER("\nSTART PARSING\n");


    get_value_from_string(sent_parameters, HTMLCONF_AP_IP, (unsigned short)(strlen(HTMLCONF_AP_IP)), &data_value[0]);
    strcpy(HTMLConfig.ap_ip,data_value);
    PRINTF_DEBUG_SERVER("ap_ip: %s\n",HTMLConfig.ap_ip);

    get_value_from_string(sent_parameters, HTMLCONF_GATEWAY_MODE, (unsigned short)(strlen(HTMLCONF_GATEWAY_MODE)), &data_value[0]);
    if(strcmp(data_value,"ap_mode") == 0){
        HTMLConfig.gateway_mode = AP_MODE;
    }
    else if(strcmp(data_value,"apsta_mode") == 0){
        HTMLConfig.gateway_mode = APSTA_MODE;
    }
    PRINTF_DEBUG_SERVER("gateway_mode: %d\n",HTMLConfig.gateway_mode);


    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID, (unsigned short)(strlen(HTMLCONF_AP_SSID)), &HTMLConfig.ap_ssid[0]);
    PRINTF_DEBUG_SERVER("ap_ssid: %s\n",HTMLConfig.ap_ssid);

    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID_HIDDEN, (unsigned short)(strlen(HTMLCONF_AP_SSID_HIDDEN)), &data_value[0]);
    if ('\0' == data_value[0]){
        HTMLConfig.ap_ssid_hidden = 0;
    	PRINTF_DEBUG_SERVER("ap_ssid_hidden: off\n");
    }
    else{
    	PRINTF_DEBUG_SERVER("ap_ssid_hidden: %s\n",data_value);
        HTMLConfig.ap_ssid_hidden = 1;
    }
    PRINTF_DEBUG_SERVER("ap_ssid_hidden: %d\n",HTMLConfig.ap_ssid_hidden);

    get_value_from_string(sent_parameters, HTMLCONF_AP_PSWD, (unsigned short)(strlen(HTMLCONF_AP_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.ap_pswd,data_value);
    PRINTF_DEBUG_SERVER("ap_pswd: %s\n",HTMLConfig.ap_pswd);

    get_value_from_string(sent_parameters, HTMLCONF_AP_DHCP_MODE, (unsigned short)(strlen(HTMLCONF_AP_DHCP_MODE)), &data_value[0]);
    if(strcmp(data_value,"off") == 0){
        HTMLConfig.ap_dhcp_mode = 0;
    }
    else if(strcmp(data_value,"on") == 0){
        HTMLConfig.ap_dhcp_mode = 1;
    }
    PRINTF_DEBUG_SERVER("ap_dhcp_mode: %d\n",HTMLConfig.ap_dhcp_mode);

    get_value_from_string(sent_parameters, HTMLCONF_AP_DHCP_IP, (unsigned short)(strlen(HTMLCONF_AP_DHCP_IP)), &data_value[0]);
    strcpy(HTMLConfig.ap_dhcp_ip,data_value);
    PRINTF_DEBUG_SERVER("ap_dhcp_ip: %s\n",HTMLConfig.ap_dhcp_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_SSID, (unsigned short)(strlen(HTMLCONF_STA_SSID)), &data_value[0]);
    strcpy(HTMLConfig.sta_ssid,data_value);
    PRINTF_DEBUG_SERVER("sta_ssid: %s\n",HTMLConfig.sta_ssid);

    get_value_from_string(sent_parameters, HTMLCONF_STA_ENCRYP, (unsigned short)(strlen(HTMLCONF_STA_ENCRYP)), &data_value[0]);
    strcpy(HTMLConfig.sta_encryption,data_value);
    PRINTF_DEBUG_SERVER("sta_encryption: %s\n",HTMLConfig.sta_encryption);

    get_value_from_string(sent_parameters, HTMLCONF_STA_PSWD, (unsigned short)(strlen(HTMLCONF_STA_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.sta_pswd,data_value);
    PRINTF_DEBUG_SERVER("sta_pswd: %s\n",HTMLConfig.sta_pswd);

    get_value_from_string(sent_parameters, HTMLCONF_STA_DHCP_MODE, (unsigned short)(strlen(HTMLCONF_STA_DHCP_MODE)), &data_value[0]);
    if(strcmp(data_value,"off") == 0){
        HTMLConfig.sta_dhcp_mode = 0;
    }
    else if(strcmp(data_value,"on") == 0){
        HTMLConfig.sta_dhcp_mode = 1;
    }
    PRINTF_DEBUG_SERVER("sta_dhcp_mode: %d\n",HTMLConfig.sta_dhcp_mode);

    get_value_from_string(sent_parameters, HTMLCONF_STA_STATIC_IP, (unsigned short)(strlen(HTMLCONF_STA_STATIC_IP)), &data_value[0]);
    strcpy(HTMLConfig.sta_static_ip,data_value);
    PRINTF_DEBUG_SERVER("sta_static_ip: %s\n",HTMLConfig.sta_static_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_NETMASK, (unsigned short)(strlen(HTMLCONF_STA_NETMASK)), &data_value[0]);
    strcpy(HTMLConfig.sta_netmask,data_value);
    PRINTF_DEBUG_SERVER("sta_netmask: %s\n",HTMLConfig.sta_netmask);

    get_value_from_string(sent_parameters, HTMLCONF_STA_GATEWAY_IP, (unsigned short)(strlen(HTMLCONF_STA_GATEWAY_IP)), &data_value[0]);
    strcpy(HTMLConfig.sta_gateway_ip,data_value);
    PRINTF_DEBUG_SERVER("sta_gateway_ip: %s\n",HTMLConfig.sta_gateway_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_PRI_DNS, (unsigned short)(strlen(HTMLCONF_STA_PRI_DNS)), &data_value[0]);
    strcpy(HTMLConfig.sta_primary_dns,data_value);
    PRINTF_DEBUG_SERVER("sta_primary_dns: %s\n",HTMLConfig.sta_primary_dns);

    get_value_from_string(sent_parameters, HTMLCONF_STA_SCND_DNS, (unsigned short)(strlen(HTMLCONF_STA_SCND_DNS)), &data_value[0]);
    strcpy(HTMLConfig.sta_secondary_dns,data_value);
    PRINTF_DEBUG_SERVER("sta_secondary_dns: %s\n",HTMLConfig.sta_secondary_dns);

    get_value_from_string(sent_parameters, HTMLCONF_NTP_SRVR_ADDR, (unsigned short)(strlen(HTMLCONF_NTP_SRVR_ADDR)), &data_value[0]);
	strcpy(HTMLConfig.ntp_server_addr,data_value);
	PRINTF_DEBUG_SERVER("ntp_server_addr: %s\n",HTMLConfig.ntp_server_addr);

	get_value_from_string(sent_parameters, HTMLCONF_NTP_SRVR_PORT, (unsigned short)(strlen(HTMLCONF_NTP_SRVR_PORT)), &data_value[0]);
	strcpy(HTMLConfig.ntp_server_port,data_value);
	PRINTF_DEBUG_SERVER("ntp_server_port: %s\n",HTMLConfig.ntp_server_port);

	get_value_from_string(sent_parameters, HTMLCONF_MQTT_SRVR_ADDR, (unsigned short)(strlen(HTMLCONF_MQTT_SRVR_ADDR)), &data_value[0]);
	/*int i=0, j=0;
	for(i=0, j=0; i< strlen(data_value); j++, i++){
		if(data_value[i] == '%' && data_value[i+1] == '3' && (data_value[i+2] == 'A' || data_value[i+2] == 'a')){
			HTMLConfig.mqtt_server_addr[j] = ':';
			i += 2;
		} else if(data_value[i] == '%' && data_value[i+1] == '2' && (data_value[i+2] == 'F' || data_value[i+2] == 'f')){
			HTMLConfig.mqtt_server_addr[j] = '/';
			i += 2;
		}else{
			HTMLConfig.mqtt_server_addr[j] = data_value[i];
		}
	}*/
	strcpy(HTMLConfig.mqtt_server_addr,data_value);
	PRINTF_DEBUG_SERVER("mqtt_server_addr: %s\n",HTMLConfig.mqtt_server_addr);


	get_value_from_string(sent_parameters, HTMLCONF_MQTT_SRVR_PORT, (unsigned short)(strlen(HTMLCONF_MQTT_SRVR_PORT)), &data_value[0]);
	strcpy(HTMLConfig.mqtt_server_port,data_value);
	PRINTF_DEBUG_SERVER("mqtt_server_port: %s\n",HTMLConfig.mqtt_server_port);

}


// Handler to upload a file onto the server
static esp_err_t upload_post_handler(httpd_req_t *req)
{

    bool send_config = false;

    static char sent_parameters[HTTPD_MAX_URI_LEN + 1] = {0};

    httpd_req_recv(req, sent_parameters, req->content_len);

    url_decoder(sent_parameters);
    PRINTF_DEBUG_SERVER("Sent parameters: %s\n\n",sent_parameters);
    PRINTF_DEBUG_SERVER("LastPageSent = %d\n",LastPageSent);

    switch(LastPageSent){
    case CHANGE_CRED:
		if(1 == get_html_change_credentials(sent_parameters)){
			PRINTF_DEBUG_SERVER("\nCred change is succeeded\n");
			HTTPServer__ParseCredfromNVM();
			httpd_resp_set_status(req, "303 See Other");
			httpd_resp_set_hdr(req, "Location", "/");

		}else{
			PRINTF_DEBUG_SERVER("\nCred change is failed\n");
			httpd_resp_set_status(req, "303 See Other");
			httpd_resp_set_hdr(req, "Location", "/");
		}
		break;


    case LOGIN:
    	if(1 == check_html_credentials(sent_parameters)){
    		PRINTF_DEBUG_SERVER("\nRight login \n");
    		httpd_resp_set_status(req, "303 See Other");
    		httpd_resp_set_hdr(req, "Location", "/config.html");
    		//send_config = true;

    	}else{
    		PRINTF_DEBUG_SERVER("\nWorng login \n");
    		httpd_resp_set_status(req, "303 See Other");
    		httpd_resp_set_hdr(req, "Location", "/");
    	}
    	break;
    case CONFIG:
        //Parsing configuration data
        get_html_config_received_data(sent_parameters);
        ReceivedConfig = 1;
        PRINTF_DEBUG_SERVER("config case received\n");

        httpd_resp_set_status(req, "303 See Other");
        httpd_resp_set_hdr(req, "Location", "/config.html");
    	break;

    default:
    	httpd_resp_set_status(req, "303 See Other");
    	httpd_resp_set_hdr(req, "Location", "/");
    	break;
    }

    httpd_resp_sendstr(req, "File uploaded successfully");

    if(true == send_config){
    	PRINTF_DEBUG_SERVER("SendingConfig\n");

    }


    return ESP_OK;
}

/* Handler to delete a file from the server */
static esp_err_t delete_post_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    struct stat file_stat;

    /* Skip leading "/delete" from URI to get filename */
    /* Note sizeof() counts NULL termination hence the -1 */
    const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path,
                                             req->uri  + sizeof("/delete") - 1, sizeof(filepath));
    ESP_LOGI(TAG, "DELETE_get_handler");
    if (!filename) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
        return ESP_FAIL;
    }

    /* Filename cannot have a trailing '/' */
    if (filename[strlen(filename) - 1] == '/') {
        ESP_LOGE(TAG, "Invalid filename : %s", filename);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Invalid filename");
        return ESP_FAIL;
    }

    if (stat(filepath, &file_stat) == -1) {
        ESP_LOGE(TAG, "File does not exist : %s", filename);
        /* Respond with 400 Bad Request */
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File does not exist");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Deleting file : %s", filename);
    /* Delete file */
    unlink(filepath);

    /* Redirect onto root to see the updated file list */
    httpd_resp_set_status(req, "303 See Other");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_sendstr(req, "File deleted successfully");
    return ESP_OK;
}


//Public Functions
esp_err_t HTTPServer__StartFileServer (httpd_handle_t server, const char *base_path)
{
	uint8_t cred_conf = DEFAULT;
	static struct file_server_data *server_data = NULL;

	// Validate file storage base path 
	if (!base_path || strcmp(base_path, "/spiffs") != 0) {
		ESP_LOGE(TAG, "File server presently supports only '/spiffs' as base path");
		return ESP_ERR_INVALID_ARG;
	}

	if (server_data) {
		ESP_LOGE(TAG, "File server already started");
		return ESP_ERR_INVALID_STATE;
	}

	// Allocate memory for server data 
	server_data = calloc(1, sizeof(struct file_server_data));
	if (!server_data) {
		ESP_LOGE(TAG, "Failed to allocate memory for server data");
		return ESP_ERR_NO_MEM;
	}
	strlcpy(server_data->base_path, base_path,
			sizeof(server_data->base_path));

	//httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	// Use the URI wildcard matching function in order to
	//allow the same handler to respond to multiple different
	//target URIs which match the wildcard scheme 
	config.uri_match_fn = httpd_uri_match_wildcard;

	ESP_LOGI(TAG, "Starting HTTP Server");
	if (httpd_start(&server, &config) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to start file server!");
		return ESP_FAIL;
	}

	httpd_uri_t get_config_page = {
		.uri       = "/config.html",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &get_config_page);


	// URI handler for getting uploaded files 
	httpd_uri_t get_login_page = {
		.uri       = "/",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &get_login_page);


	httpd_uri_t get_favicon = {
		.uri       = "/fav.ico",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &get_favicon);

	httpd_uri_t get_css = {
		.uri       = "/style.css",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &get_css);


	httpd_uri_t get_config_json = {
		.uri       = "/config.json",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &get_config_json);







	// URI handler for uploading files to server 
	httpd_uri_t file_upload = {
		.uri       = "/upload/*",   // Match all URIs of type /upload/path/to/file
		.method    = HTTP_POST,
		.handler   = upload_post_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &file_upload);

	// URI handler for deleting files from server 
	httpd_uri_t file_delete = {
		.uri       = "/delete/*",   // Match all URIs of type /delete/path/to/file
		.method    = HTTP_POST,
		.handler   = delete_post_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &file_delete);


	if(ESP_OK == NVM__ReadU8Value(HTMLLOGIN_CONF_NVM, &cred_conf) && (cred_conf == CONFIGURED)){
		HTTPServer__ParseCredfromNVM();
	}
	return ESP_OK;
}


esp_err_t HTTPServer__StopServer(httpd_handle_t server){
    // Stop the httpd server
    esp_err_t err = httpd_stop(server);
    return err;
}


html_config_param_t HTTPServer__GetCustomConfig (void){
    return HTMLConfig;
}

uint8_t IsConfigReceived(void){
    return ReceivedConfig;
}


char* HTTPServer__SetAPDefSSID(const char* default_name){

	sprintf(ap_ssid_def, "%s_%.6s", default_name, Utilities__GetMACAddr());

	return ap_ssid_def;
}

