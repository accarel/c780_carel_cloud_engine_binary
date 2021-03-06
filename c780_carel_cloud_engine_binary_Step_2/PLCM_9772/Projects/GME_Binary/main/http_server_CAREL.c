/*
 * httpserver.c
 *
 *  Created on: Jun 17, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#include "http_server_CAREL.h"
#include "http_server_IS.h"
#include "nvm_CAREL.h"
#include "utilities_CAREL.h"
#include <ctype.h>
#include "wifi.h"

static html_config_param_t HTMLConfig = {
		.ap_ssid = AP_DEF_SSID,
		.ap_pswd = AP_DEF_PSSWD,
		.ap_ssid_hidden = AP_DEF_SSID_HIDDEN,
		.ap_ip = AP_DEF_IP,
		.ap_scan_mode = 0,
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
};

static html_login_cred_t HTMLLogin = {
		.login_usr = "",
		.login_pswd = ""
};

static char ap_ssid_def[30] = {0};
bool login_done = FALSE;

/* ==========================================================================
 * Private Functions Declaration
 * ========================================================================== */

/**
 * @brief url_decoder
 *        decode the contents of the passed URL
 * @param char *buf
 *        pointer to the buffer that contains the URL
 * @return none
 */
void url_decoder(char *buf) {

    char eStr[3] = "00"; /* for a hex code */

    int i; /* the counter for the string */

    for(i=0;i<strlen(buf);++i) {

        if(buf[i] == '%')
        {
           if(buf[i+1] == 0)
             return;

           if(isxdigit((int)buf[i+1]) && isxdigit((int)buf[i+2]))
           {

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


/**
 * @brief get_path_from_uri
 *     Copies the full path into destination buffer and returns
 *     pointer to path (skipping the preceding base path)
 * @param char *dest
 * @param const char *base_path
 * @param const char *uri
 * @param size_t destsize
 *
 * @return pointer to (dest + base_pathlen)
 */
const char* get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize)
{
	PRINTF_DEBUG_SERVER("base_path: %s \n", base_path);
    const size_t base_pathlen = strlen(base_path);
    size_t pathlen = strlen(uri);

    const char *quest = strchr(uri, '?');
    if (quest) {
        pathlen = MIN(pathlen, quest - uri);
        P_COV_LN;
    }
    const char *hash = strchr(uri, '#');
    if (hash) {
        pathlen = MIN(pathlen, hash - uri);
        P_COV_LN;
    }

    if (base_pathlen + pathlen + 1 > destsize) {
        /* Full path string won't fit into destination buffer */
    	P_COV_LN;
        return NULL;
    }

    /* Construct full path (base + path) */
    strcpy(dest, base_path);
    strlcpy(dest + base_pathlen, uri, pathlen + 1);

    P_COV_LN;
    /* Return pointer to path, skipping the base */
    return dest + base_pathlen;
}


/**
 * @brief get_value_from_string
 *
 * @param char* received_buf
 * @param const char* value_key
 * @param unsigned short value_key_len
 * @param char* req_value
 *
 * @return none
 */
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
		P_COV_LN;
	}
	else{
		req_value[0]='\0';
		P_COV_LN;
	}
}


/**
 * @brief get_html_change_credentials
 *        set the new credential and store it in NVM
 *
 * @param char* sent_parameters
 *
 * @return 0 == FAIL
 *         1 == SUCCESS
 */
int get_html_change_credentials(char* sent_parameters){

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

    	if (ESP_OK == err1 && ESP_OK == err2){

    		if (C_SUCCESS == NVM__WriteU8Value(HTMLLOGIN_CONF_NVM, CONFIGURED))
    		{
    			P_COV_LN;
    			return 1;
    		}
    		else
    		{
    			P_COV_LN;
    			return 0;
    		}

    	}else{
    		P_COV_LN;
    		return 0;
    	}
    }

    return 0;
}



/**
 * @brief HTTPServer__ParseCredfromNVM
 *
 * @param none
 * @return none
 */
void HTTPServer__ParseCredfromNVM(void){
	size_t len = 0;
	NVM__ReadString(HTMLLOGIN_USR,HTMLLogin.login_usr,&len);
	NVM__ReadString(HTMLLOGIN_PSWD,HTMLLogin.login_pswd,&len);
	/* note retuned value not checked due to the fact that in case of
	 * error the system return anyway invalid credential
	 * */
}


/**
 * @brief check_html_credentials
 *         the credential to check
 *
 * @param char* sent_parameters
 *        the credential to check
 *
 * @return 0 == FAIL
 *         1 == SUCCESS
 */
int check_html_credentials(char* sent_parameters){

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
    	P_COV_LN;
    	login_done = TRUE;
    	return 1;
    }

    return 0;
}



/**
 * @brief get_html_config_received_data
 *
 * @param char* sent_parameters
 *        the data
 *
 * @return C_FAIL / C_SUCCESS
 */
C_RES get_html_config_received_data(char* sent_parameters){
    char data_value[100];
	esp_err_t err;
    C_RES retval;

    char current_page;
    retval = C_SUCCESS;

    memset(data_value,0,strlen(data_value)*sizeof(char));

    PRINTF_DEBUG_SERVER("\nSTART PARSING\n");

    // check which page is display on the browser
    get_value_from_string(sent_parameters, HTML_CURRENT_PAGE, (unsigned short)(strlen(HTML_CURRENT_PAGE)), &current_page);

   if(current_page == '2')
   {
    get_value_from_string(sent_parameters, HTMLCONF_AP_IP, (unsigned short)(strlen(HTMLCONF_AP_IP)), &data_value[0]);
    strcpy(HTMLConfig.ap_ip,data_value);
    PRINTF_DEBUG_SERVER("ap_ip: %s\n",HTMLConfig.ap_ip);

    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID, (unsigned short)(strlen(HTMLCONF_AP_SSID)), &HTMLConfig.ap_ssid[0]);
    PRINTF_DEBUG_SERVER("ap_ssid: %s\n",HTMLConfig.ap_ssid);

    get_value_from_string(sent_parameters, HTMLCONF_AP_SSID_HIDDEN, (unsigned short)(strlen(HTMLCONF_AP_SSID_HIDDEN)), &data_value[0]);
    if ('\0' == data_value[0]){
        HTMLConfig.ap_ssid_hidden = 0;
    	PRINTF_DEBUG_SERVER("ap_ssid_hidden: off\n");
    }
    else{
    	PRINTF_DEBUG_SERVER("ap_ssid_hidden: on\n");
        HTMLConfig.ap_ssid_hidden = 1;
    }
    PRINTF_DEBUG_SERVER("ap_ssid_hidden: %d\n",HTMLConfig.ap_ssid_hidden);

    get_value_from_string(sent_parameters, HTMLCONF_AP_PSWD, (unsigned short)(strlen(HTMLCONF_AP_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.ap_pswd,data_value);
    PRINTF_DEBUG_SERVER("ap_pswd: %s\n",HTMLConfig.ap_pswd);

    get_value_from_string(sent_parameters, "sta_scan_mode", (unsigned short)(strlen("sta_scan_mode")), &data_value[0]);
    if(strcmp(data_value,"off") == 0){
    	HTMLConfig.ap_scan_mode = 0;
    	get_value_from_string(sent_parameters, HTMLCONF_STA_SSID_MAN, (unsigned short)(strlen(HTMLCONF_STA_SSID_MAN)), &data_value[0]);
    }
    else if(strcmp(data_value,"on") == 0){
        HTMLConfig.ap_scan_mode = 1;
        get_value_from_string(sent_parameters, HTMLCONF_STA_SSID_SCAN, (unsigned short)(strlen(HTMLCONF_STA_SSID_SCAN)), &data_value[0]);
    }
    else if(strcmp(data_value,"wps") == 0){
		HTMLConfig.ap_scan_mode = 2;
	}
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

    if (strlen(HTMLConfig.ntp_server_addr) > 0){
        	err = NVM__WriteString(NTP_SERVER, HTMLConfig.ntp_server_addr);
        	if (err != ESP_OK) retval = C_FAIL;
       }

   }

    get_value_from_string(sent_parameters, HTMLLOGIN_USR, (unsigned short)(strlen(HTMLLOGIN_USR)), &data_value[0]);
    strcpy(HTMLConfig.login_usr,data_value);
    PRINTF_DEBUG_SERVER("login_usr: %s\n",HTMLConfig.login_usr);

    get_value_from_string(sent_parameters, HTMLLOGIN_PSWD, (unsigned short)(strlen(HTMLLOGIN_PSWD)), &data_value[0]);
    strcpy(HTMLConfig.login_pswd,data_value);
    PRINTF_DEBUG_SERVER("login_pswd: %s\n",HTMLConfig.login_pswd);

    if ((strlen(HTMLConfig.login_usr) > 0) && (strlen(HTMLConfig.login_pswd) > 0)){
     	err = NVM__WriteString(HTMLLOGIN_USR, HTMLConfig.login_usr);
     	if (err != ESP_OK) retval = C_FAIL;
     	err = NVM__WriteString(HTMLLOGIN_PSWD, HTMLConfig.login_pswd);
     	if (err != ESP_OK) retval = C_FAIL;
     	P_COV_LN;
    }

    return retval;
}



html_config_param_t HTTPServer__GetCustomConfig (void){
    return HTMLConfig;
}


void SetWpsParameters(wifi_config_t wifi_config_temp){

	// read from file system
	GetNtpServer(HTMLConfig.ntp_server_addr);
	// set wps recovered data
	strcpy(HTMLConfig.sta_ssid, (char*)wifi_config_temp.ap.ssid);
	strcpy(HTMLConfig.sta_pswd, (char*)wifi_config_temp.ap.password);
	// when configuring via wps, dhcp mode is forced
	HTMLConfig.sta_dhcp_mode = 1;
	return;
}


char* HTTPServer__SetAPDefSSID(const char* default_name){

	/*
	 *  like the BLE devices (look at the PJ BLE) the ssid name show only the low part of the mac-addr (LSB part)
	 *  for example, mac-addr = 30AEA4EF4738
	 *  the name shown will be  "GME_EF4738"
	 *
	 */
	sprintf(ap_ssid_def, "%s_%.2s%.2s", default_name,
			                            	Utilities__GetMACAddr()+8,
											Utilities__GetMACAddr()+10);

	return ap_ssid_def;
}

uint8_t GetSsidSelection(void){
	return HTMLConfig.ap_scan_mode;
}

bool IsLoginDone(void){
	return login_done;
}

void SetLoginDone(bool login) { login_done = login; }

void GetLoginUsr(char* tmp_login_usr){
	size_t len = 0;
	NVM__ReadString(HTMLLOGIN_USR,tmp_login_usr,&len);
}

void GetLoginPsw(char* tmp_login_psw){
	size_t len = 0;
	NVM__ReadString(HTMLLOGIN_PSWD,tmp_login_psw,&len);
}
