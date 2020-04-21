

#include "esp_spiffs.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "data_types_CAREL.h"
#include "nvm_CAREL.h"
#include "utilities_CAREL.h"
#include "wifi.h"
#include "http_server_CAREL.h"
#include "http_server_IS.h"

static const char *TAG = "http_server";
static C_BYTE ReceivedConfig = 0;
static C_BYTE WpsMode = 0;
static html_pages LastPageSent = 0;

static C_RES set_content_type_from_file(httpd_req_t *req, const char *filename);

// Handler to respond with file requested.
static C_RES file_get_handler(httpd_req_t *req, const char* filename)
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
static C_RES http_resp_config_json(httpd_req_t *req)
{
    //Get config values into a json struct
	char *out;
	char ap_ssid_temp[30] = {0};
	size_t len=0;
	cJSON *html_config;
	html_config_param_t *wifi_config;
	html_config = cJSON_CreateObject();
	wifi_config = WiFi__GetCustomConfigPTR();

	if(ESP_OK != NVM__ReadString(HTMLCONF_AP_SSID, ap_ssid_temp, &len)){
		strcpy(ap_ssid_temp, HTTPServer__SetAPDefSSID(AP_DEF_SSID));
		PRINTF_DEBUG_SERVER("ap_ssid_temp : %s\n",ap_ssid_temp);
	}
	cJSON_AddItemToObject(html_config, HTMLCONF_AP_SSID, cJSON_CreateString(ap_ssid_temp));

	cJSON_AddItemToObject(html_config, HTMLCONF_AP_SSID_HIDDEN, cJSON_CreateBool((bool)wifi_config->ap_ssid_hidden));
	cJSON_AddItemToObject(html_config, HTMLCONF_AP_PSWD, cJSON_CreateString(wifi_config->ap_pswd));
	cJSON_AddItemToObject(html_config, HTMLCONF_AP_IP, cJSON_CreateString(wifi_config->ap_ip));

	cJSON_AddItemToObject(html_config, HTMLCONF_STA_SSID, cJSON_CreateString(wifi_config->sta_ssid));
	// could change this to a for loop...
	cJSON_AddItemToObject(html_config, "sta_ssid1", cJSON_CreateString(GetAvailableAPs(0)));
	cJSON_AddItemToObject(html_config, "sta_ssid2", cJSON_CreateString(GetAvailableAPs(1)));
	cJSON_AddItemToObject(html_config, "sta_ssid3", cJSON_CreateString(GetAvailableAPs(2)));
	cJSON_AddItemToObject(html_config, "sta_ssid4", cJSON_CreateString(GetAvailableAPs(3)));
	cJSON_AddItemToObject(html_config, "sta_ssid5", cJSON_CreateString(GetAvailableAPs(4)));
	cJSON_AddItemToObject(html_config, "sta_ssid6", cJSON_CreateString(GetAvailableAPs(5)));
	cJSON_AddItemToObject(html_config, "sta_ssid7", cJSON_CreateString(GetAvailableAPs(6)));
	cJSON_AddItemToObject(html_config, "sta_ssid8", cJSON_CreateString(GetAvailableAPs(7)));
	cJSON_AddItemToObject(html_config, "sta_ssid9", cJSON_CreateString(GetAvailableAPs(8)));
	cJSON_AddItemToObject(html_config, "sta_ssid10", cJSON_CreateString(GetAvailableAPs(9)));

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
static C_RES set_content_type_from_file(httpd_req_t *req, const char *filename)
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


/* Handler to download a file kept on the server */
static C_RES download_get_handler(httpd_req_t *req)
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


// Handler to upload a file onto the server
static C_RES upload_post_handler(httpd_req_t *req)
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
		}else
			PRINTF_DEBUG_SERVER("\nCred change is failed\n");

		httpd_resp_set_status(req, "303 See Other");
		httpd_resp_set_hdr(req, "Location", "/");

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
        if(GetSsidSelection() != 2)
        	SetConfigReceived();
        else
        	SetWpsMode();
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
static C_RES delete_post_handler(httpd_req_t *req)
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
C_RES HTTPServer__StartFileServer (httpd_handle_t server, const char *base_path)
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

C_RES HTTPServer__StopServer(httpd_handle_t server){
    // Stop the httpd server
    esp_err_t err = httpd_stop(server);
    return err;
}

void SetConfigReceived(void){
	ReceivedConfig = 1;
}

C_BYTE IsConfigReceived(void){
    return ReceivedConfig;
}

void SetWpsMode(void){
	WpsMode = 1;
}

void UnSetWpsMode(void){
	WpsMode = 0;
}

C_BYTE IsWpsMode(void){
    return WpsMode;
}
