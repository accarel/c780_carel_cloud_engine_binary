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
#include "nvm.h"

static html_config_param_t HTMLConfig = {
		.gateway_mode = 0,
		.ap_ssid = AP_DEF_SSID,
		.ap_pswd = AP_DEF_PSSWD,
		.ap_ssid_hidden = AP_DEF_SSID_HIDDEN,
		.ap_ip = AP_DEF_IP,
		.ap_dhcp_mode = AP_DEF_DHCP,
		.ap_dhcp_ip = AP_DEF_DHCP_BASE,
		.sta_ssid = "z",
		.sta_encryption = "z",
		.sta_pswd= "z",
		.sta_dhcp_mode = 0,
		.sta_static_ip = "z",
		.sta_netmask = "z",
		.sta_gateway_ip = "z",
		.sta_primary_dns = "z",
		.sta_secondary_dns = "z"

};

static html_login_cred_t HTMLLogin = {
		.login_usr = HTMLLOGIN_DEF_PSWD,
		.login_pswd = HTMLLOGIN_DEF_PSWD
};

static uint8_t ReceivedConfig = 0;

static html_pages LastPageSent = 0;
// Max length a file path can have on storage
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

// Max size of an individual file. Make sure this
// value is same as that set in xconfig_page_test01.html 
#define MAX_FILE_SIZE   (200*1024) // 200 KB
#define MAX_FILE_SIZE_STR "200KB"

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

// Handler to redirect incoming GET request for /index.html to /
// This can be overridden by uploading file with same name 
static esp_err_t login_html_get_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "307 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);  // Response body can be empty
    return ESP_OK;
}

// Handler to respond with an icon file embedded in flash.
// Browsers expect to GET website icon at URI /favicon.ico.
// This can be overridden by uploading file with same name 
static esp_err_t favicon_get_handler(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_favicon_ico_start");
    extern const unsigned char favicon_ico_end[]   asm("_binary_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}


//Send style.css
static esp_err_t http_resp_css(httpd_req_t *req)
{
    /* Get handle to embedded file upload script */
    extern const unsigned char xstyle_css_start[] asm("_binary_xstyle_css_start");
    extern const unsigned char xstyle_css_end[]   asm("_binary_xstyle_css_end");
    const size_t xstyle_css_size = (xstyle_css_end - xstyle_css_start);

    httpd_resp_set_type(req, "text/css");
    /* Add file upload form and script which on execution sends a POST request to /upload */

    httpd_resp_send_chunk(req, (const char *)xstyle_css_start, xstyle_css_size);

    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}





static esp_err_t http_config_html(httpd_req_t *req, const char *dirpath)
{
    char entrypath[FILE_PATH_MAX];
    char entrysize[16];
    const char *entrytype;

    struct dirent *entry;
    struct stat entry_stat;

    DIR *dir = opendir(dirpath);
    const size_t dirpath_len = strlen(dirpath);

    /* Retrieve the base path of file storage to construct the full path */
    strlcpy(entrypath, dirpath, sizeof(entrypath));

    if (!dir) {
        ESP_LOGE(TAG, "Failed to stat dir : %s", dirpath);
        /* Respond with 404 Not Found */
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Directory does not exist");
        return ESP_FAIL;
    }

    /* Get handle to embedded file upload script */
    extern const unsigned char xconfig_page_test01_start[] asm("_binary_xconfig_page_test01_html_start");
    extern const unsigned char xconfig_page_test01_end[]   asm("_binary_xconfig_page_test01_html_end");
    const size_t xconfig_page_test01_size = (xconfig_page_test01_end - xconfig_page_test01_start);

    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_send_chunk(req, (const char *)xconfig_page_test01_start, xconfig_page_test01_size);

    closedir(dir);

    /* Send empty chunk to signal HTTP response completion */
    httpd_resp_sendstr_chunk(req, NULL);
    LastPageSent = CONFIG;
    return ESP_OK;
}


static esp_err_t http_login_html(httpd_req_t *req, const char *dirpath)
{
    char entrypath[FILE_PATH_MAX];
    char entrysize[16];
    const char *entrytype;

    struct dirent *entry;
    struct stat entry_stat;

    DIR *dir = opendir(dirpath);
    const size_t dirpath_len = strlen(dirpath);

    /* Retrieve the base path of file storage to construct the full path */
    strlcpy(entrypath, dirpath, sizeof(entrypath));

    if (!dir) {
        ESP_LOGE(TAG, "Failed to stat dir : %s", dirpath);
        /* Respond with 404 Not Found */
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Directory does not exist");
        return ESP_FAIL;
    }

    /* Get handle to embedded file upload script */
    extern const unsigned char xlogin_start[] asm("_binary_xlogin_html_start");
    extern const unsigned char xlogin_end[]   asm("_binary_xlogin_html_end");
    const size_t xlogin_size = (xlogin_end - xlogin_start);

    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_send_chunk(req, (const char *)xlogin_start, xlogin_size);

    closedir(dir);

    /* Send empty chunk to signal HTTP response completion */
    httpd_resp_sendstr_chunk(req, NULL);
    LastPageSent = LOGIN;
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
    }
    /* This is a limited set only */
    /* For any other type always set as plain text */
    return httpd_resp_set_type(req, "text/plain");
}

/* Copies the full path into destination buffer and returns
 * pointer to path (skipping the preceding base path) */
static const char* get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize)
{
	printf("base_path: %s \n", base_path);
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
        return http_login_html(req, filepath);
    }

    if (stat(filepath, &file_stat) == -1) {
        /* If file not present on SPIFFS check if URI
         * corresponds to one of the hardcoded paths */
        if (strcmp(filename, "/login.html") == 0) {
            return login_html_get_handler(req);		//redirecting to '/'
        } else if (strcmp(filename, "/config.html") == 0) {
        	return http_config_html(req, filepath);
        } else if (strcmp(filename, "/favicon.ico") == 0) {
            return favicon_get_handler(req);
        } else if (strcmp(filename, "/xstyle.css") == 0){
        	return http_resp_css(req);
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

    ESP_LOGI(TAG, "Sending file : %s (%ld bytes)...", filename, file_stat.st_size);
    set_content_type_from_file(req, filename);

    /* Retrieve the pointer to scratch buffer for temporary storage */
    char *chunk = ((struct file_server_data *)req->user_ctx)->scratch;
    size_t chunksize;
    do {
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

        /* Keep looping till the whole file is sent */
    } while (chunksize != 0);

    /* Close file after sending complete */
    fclose(fd);
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

/*
static void get_value_from_string(char* received_buf, const char* value_key, unsigned short value_key_len, char* req_value)
{
	char *key_buff = strstr(received_buf, value_key);

	if(key_buff != NULL)
	{
		char *and_key_buff = strstr(key_buff, "&");
		printf("debug2\n");
		key_buff += (unsigned int)value_key_len;
		printf("debug3\n");
		unsigned char len = (unsigned char)(and_key_buff - key_buff);
		printf("debug10\n");

		//key_buff[len]='\0';
		strcpy(req_value,key_buff);
		//memcpy(req_value, key_buff, (len));
		printf("debug11\n");
		req_value[len]='\0';
		printf("debug4\n");
	}
	else{
		req_value[0]='\0';
		printf("debug5\n");
	}
	printf("debug6\n");
}
*/

static int check_html_credentials(char* sent_parameters){

    char data_value[100];
    memset(data_value,0,strlen(data_value)*sizeof(char));
    printf("\nSTART PARSING\n");

    html_login_cred_t temp;

    //Read credentials from NVM

    //username
    get_value_from_string(sent_parameters, HTMLLOGIN_USR, (unsigned short)(strlen(HTMLLOGIN_USR)), &data_value[0]);
    strcpy(temp.login_usr,data_value);
    printf("login_usr: %s\n",temp.login_usr);

    //password
    get_value_from_string(sent_parameters, HTMLLOGIN_PSWD, (unsigned short)(strlen(HTMLLOGIN_PSWD)), &data_value[0]);
    strcpy(temp.login_pswd,data_value);
    printf("login_pswd: %s\n",temp.login_pswd);


    if ((strcmp(temp.login_usr, HTMLLOGIN_DEF_USR) == 0) && (strcmp(temp.login_pswd, HTMLLOGIN_DEF_PSWD) == 0)){
    	return 1;
    }


    return 0;
}


static void get_html_config_received_data(char* sent_parameters){
    
    char data_value[100];
    memset(data_value,0,strlen(data_value)*sizeof(char));

    printf("\nSTART PARSING\n");


    get_value_from_string(sent_parameters, HTMLCONF_AP_IP, (unsigned short)(strlen(HTMLCONF_AP_IP)), &data_value[0]);
    strcpy(HTMLConfig.ap_ip,data_value);
    printf("ap_ip: %s\n",HTMLConfig.ap_ip);
    //NVM__WriteString(HTMLCONF_AP_IP,HTMLConfig.ap_ip);
    //static size_t len;
    //NVM__ReadString(HTMLCONF_AP_IP, HTMLConfig.ap_ip, &len);


    get_value_from_string(sent_parameters, HTMLCONF_GATEWAY_MODE, (unsigned short)(strlen(HTMLCONF_GATEWAY_MODE)), &data_value[0]);
    if(strcmp(data_value,"ap_mode") == 0){
        HTMLConfig.gateway_mode = AP_MODE;
    }
    else if(strcmp(data_value,"apsta_mode") == 0){
        HTMLConfig.gateway_mode = APSTA_MODE;
    }
    printf("gateway_mode: %d\n",HTMLConfig.gateway_mode);

    //NVM__WriteU8Value(HTMLCONF_GATEWAY_MODE,HTMLConfig.gateway_mode);

    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID, (unsigned short)(strlen(HTMLCONF_AP_SSID)), &HTMLConfig.ap_ssid[0]);
    printf("ap_ssid: %s\n",HTMLConfig.ap_ssid);
    //NVM__WriteString(HTMLCONF_AP_SSID,HTMLConfig.ap_ssid);

    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID_HIDDEN, (unsigned short)(strlen(HTMLCONF_AP_SSID_HIDDEN)), &data_value[0]);
    if ('\0' == data_value[0]){
        HTMLConfig.ap_ssid_hidden = 0;
    	printf("ap_ssid_hidden: off\n");
    }
    else{
    	printf("ap_ssid_hidden: %s\n",data_value);
        HTMLConfig.ap_ssid_hidden = 1;
    }
    printf("ap_ssid_hidden: %d\n",HTMLConfig.ap_ssid_hidden);
    //NVM__WriteU8Value(HTMLCONF_AP_SSID_HIDDEN,HTMLConfig.ap_ssid_hidden);

    get_value_from_string(sent_parameters, HTMLCONF_AP_PSWD, (unsigned short)(strlen(HTMLCONF_AP_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.ap_pswd,data_value);
    printf("ap_pswd: %s\n",HTMLConfig.ap_pswd);
    //NVM__WriteString(HTMLCONF_AP_PSWD,HTMLConfig.ap_pswd);



    get_value_from_string(sent_parameters, HTMLCONF_AP_DHCP_MODE, (unsigned short)(strlen(HTMLCONF_AP_DHCP_MODE)), &data_value[0]);
    if(strcmp(data_value,"off") == 0){
        HTMLConfig.ap_dhcp_mode = 0;
    }
    else if(strcmp(data_value,"on") == 0){
        HTMLConfig.ap_dhcp_mode = 1;
    }
    printf("ap_dhcp_mode: %d\n",HTMLConfig.ap_dhcp_mode);
    //NVM__WriteU8Value(HTMLCONF_AP_DHCP_MODE,HTMLConfig.ap_dhcp_mode);

    get_value_from_string(sent_parameters, HTMLCONF_AP_DHCP_IP, (unsigned short)(strlen(HTMLCONF_AP_DHCP_IP)), &data_value[0]);
    strcpy(HTMLConfig.ap_dhcp_ip,data_value);
    printf("ap_dhcp_ip: %s\n",HTMLConfig.ap_dhcp_ip);
    //NVM__WriteString(HTMLCONF_AP_DHCP_IP,HTMLConfig.ap_dhcp_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_SSID, (unsigned short)(strlen(HTMLCONF_STA_SSID)), &data_value[0]);
    strcpy(HTMLConfig.sta_ssid,data_value);
    printf("sta_ssid: %s\n",HTMLConfig.sta_ssid);
    //NVM__WriteString(HTMLCONF_STA_SSID,HTMLConfig.sta_ssid);

    get_value_from_string(sent_parameters, HTMLCONF_STA_ENCRYP, (unsigned short)(strlen(HTMLCONF_STA_ENCRYP)), &data_value[0]);
    strcpy(HTMLConfig.sta_encryption,data_value);
    printf("sta_encryption: %s\n",HTMLConfig.sta_encryption);
    //NVM__WriteString(HTMLCONF_STA_ENCRYP,HTMLConfig.sta_encryption);

    get_value_from_string(sent_parameters, HTMLCONF_STA_PSWD, (unsigned short)(strlen(HTMLCONF_STA_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.sta_pswd,data_value);
    printf("sta_pswd: %s\n",HTMLConfig.sta_pswd);
    //NVM__WriteString(HTMLCONF_STA_PSWD,HTMLConfig.sta_pswd);

    get_value_from_string(sent_parameters, HTMLCONF_STA_DHCP_MODE, (unsigned short)(strlen(HTMLCONF_STA_DHCP_MODE)), &data_value[0]);
    if(strcmp(data_value,"off") == 0){
        HTMLConfig.sta_dhcp_mode = 0;
    }
    else if(strcmp(data_value,"on") == 0){
        HTMLConfig.sta_dhcp_mode = 1;
    }
    printf("sta_dhcp_mode: %d\n",HTMLConfig.sta_dhcp_mode);
    //NVM__WriteU8Value(HTMLCONF_STA_DHCP_MODE,HTMLConfig.sta_dhcp_mode);

    get_value_from_string(sent_parameters, HTMLCONF_STA_STATIC_IP, (unsigned short)(strlen(HTMLCONF_STA_STATIC_IP)), &data_value[0]);
    strcpy(HTMLConfig.sta_static_ip,data_value);
    printf("sta_static_ip: %s\n",HTMLConfig.sta_static_ip);
    //NVM__WriteString(HTMLCONF_STA_STATIC_IP,HTMLConfig.sta_static_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_NETMASK, (unsigned short)(strlen(HTMLCONF_STA_NETMASK)), &data_value[0]);
    strcpy(HTMLConfig.sta_netmask,data_value);
    printf("sta_netmask: %s\n",HTMLConfig.sta_netmask);
    //NVM__WriteString(HTMLCONF_STA_NETMASK,HTMLConfig.sta_netmask);

    get_value_from_string(sent_parameters, HTMLCONF_STA_GATEWAY_IP, (unsigned short)(strlen(HTMLCONF_STA_GATEWAY_IP)), &data_value[0]);
    strcpy(HTMLConfig.sta_gateway_ip,data_value);
    printf("sta_gateway_ip: %s\n",HTMLConfig.sta_gateway_ip);
    //NVM__WriteString(HTMLCONF_STA_GATEWAY_IP,HTMLConfig.sta_gateway_ip);

    get_value_from_string(sent_parameters, HTMLCONF_STA_PRI_DNS, (unsigned short)(strlen(HTMLCONF_STA_PRI_DNS)), &data_value[0]);
    strcpy(HTMLConfig.sta_primary_dns,data_value);
    printf("sta_primary_dns: %s\n",HTMLConfig.sta_primary_dns);
    //NVM__WriteString(HTMLCONF_STA_PRI_DNS,HTMLConfig.sta_primary_dns);

    get_value_from_string(sent_parameters, HTMLCONF_STA_SCND_DNS, (unsigned short)(strlen(HTMLCONF_STA_SCND_DNS)), &data_value[0]);
    strcpy(HTMLConfig.sta_secondary_dns,data_value);
    printf("sta_secondary_dns: %s\n",HTMLConfig.sta_secondary_dns);
    //NVM__WriteString(HTMLCONF_STA_SCND_DNS,HTMLConfig.sta_secondary_dns);

}


// Handler to upload a file onto the server
static esp_err_t upload_post_handler(httpd_req_t *req)
{
    char filepath[100];
    FILE *fd = NULL;
    struct stat file_stat;

    // Skip leading "/upload" from URI to get filename
    // Note sizeof() counts NULL termination hence the -1
    const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path, req->uri + sizeof("/upload") - 1, sizeof(filepath));




    static char sent_parameters[500];
    httpd_req_recv(req, sent_parameters, req->content_len);
    printf("sent_parameters: %s\n",sent_parameters);

    switch(LastPageSent){
    case LOGIN:
    	if(1 == check_html_credentials(sent_parameters)){
    		printf("\n Right login \n");
    		httpd_resp_set_status(req, "303 See Other");
    		httpd_resp_set_hdr(req, "Location", "/config.html");
    	}else{
    		printf("\n Worng login \n");
    		httpd_resp_set_status(req, "303 See Other");
    		httpd_resp_set_hdr(req, "Location", "/");
    	}
    	break;
    case CONFIG:
        //Parsing configuration data
        get_html_config_received_data(sent_parameters);
        ReceivedConfig = 1;

        //TODO
        //Update HTML values from JSON file

        httpd_resp_set_status(req, "303 See Other");
        httpd_resp_set_hdr(req, "Location", "/config.html");
    	break;

    default:
    	httpd_resp_set_status(req, "303 See Other");
    	httpd_resp_set_hdr(req, "Location", "/");
    	break;
    }

    httpd_resp_sendstr(req, "File uploaded successfully");

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

	// URI handler for getting uploaded files 
	httpd_uri_t file_download = {
		.uri       = "/*",  // Match all URIs of type /path/to/file
		.method    = HTTP_GET,
		.handler   = download_get_handler,
		.user_ctx  = server_data    // Pass server data as context
	};
	httpd_register_uri_handler(server, &file_download);

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




