/*
 * HTTPServer.h
 *
 *  Created on: Jun 17, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_HTTP_SERVER_CAREL_H_
#define MAIN_HTTP_SERVER_CAREL_H_

#include "common.h"

// Definitions

//	Login HTML Tgas
#define HTMLLOGIN_CONF_NVM		"login_conf"
#define HTMLLOGIN_USR			"login_usr"
#define HTMLLOGIN_PSWD			"login_pswd"

// Config HTML responce tags
#define HTMLCONF_GATEWAY_MODE	"gateway_mode"

#define HTMLCONF_AP_SSID		"ap_ssid"
#define HTMLCONF_AP_SSID_HIDDEN	"ap_ssid_hidden"
#define HTMLCONF_AP_PSWD		"ap_pswd"
#define HTMLCONF_AP_IP			"ap_ip"
#define HTMLCONF_AP_DHCP_MODE	"ap_dhcp_mode"
#define HTMLCONF_AP_DHCP_IP		"ap_dhcp_ip"

#define HTMLCONF_STA_SSID		"sta_ssid"
#define HTMLCONF_STA_ENCRYP		"sta_encryption"
#define HTMLCONF_STA_PSWD		"sta_pswd"
#define HTMLCONF_STA_DHCP_MODE	"sta_dhcp_mode"
#define HTMLCONF_STA_STATIC_IP	"sta_static_ip"
#define HTMLCONF_STA_NETMASK	"sta_netmask"
#define HTMLCONF_STA_GATEWAY_IP	"sta_gateway"
#define HTMLCONF_STA_PRI_DNS	"sta_pri_dns"
#define HTMLCONF_STA_SCND_DNS	"sta_scnd_dns"

#define HTMLCONF_NTP_SRVR_ADDR	"ntp_addr"
#define HTMLCONF_NTP_SRVR_PORT	"ntp_port"

#define HTMLCONF_MQTT_SRVR_ADDR	"mqtt_addr"
#define HTMLCONF_MQTT_SRVR_PORT	"mqtt_port"



#if HTTP_SERVER_DEBUG == 1
	#define	PRINTF_DEBUG_SERVER	printf
#else
	#define	PRINTF_DEBUG_SERVER(...)
#endif



#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)


typedef enum e_html_pages{
	CHANGE_CRED = 0,
	LOGIN,
	CONFIG,
}html_pages;


#pragma pack(1)
typedef struct _html_config_param_s
{
    uint8_t	gateway_mode;
    char   	ap_ssid[30];
    uint8_t ap_ssid_hidden;
    char   	ap_pswd[30];
    char   	ap_ip[30];
    uint8_t ap_dhcp_mode;
    char  	ap_dhcp_ip[30];

    char  	sta_ssid[30];
    char  	sta_encryption[30];
    char  	sta_pswd[30];
    uint8_t sta_dhcp_mode;
    char	sta_static_ip[20];
    char	sta_netmask[30];
    char	sta_gateway_ip[30];
    char	sta_primary_dns[30];
    char	sta_secondary_dns[30];
    char	ntp_server_addr[30];
	char	ntp_server_port[6];
	char	mqtt_server_addr[60];
	char	mqtt_server_port[6];
}html_config_param_t;
#pragma pack()

#pragma pack(1)
typedef struct _html_login_cred_s
{
    char	login_usr[30];
    char	login_pswd[30];
}html_login_cred_t;
#pragma pack()

void url_decoder(char *buf);
const char* get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize);
void HTTPServer__ParseCredfromNVM(void);
int check_html_credentials(char* sent_parameters);
void get_html_config_received_data(char* sent_parameters);
void HTTPServer__ParseCredfromNVM(void);
int get_html_change_credentials(char* sent_parameters);
html_config_param_t HTTPServer__GetCustomConfig (void);

char* HTTPServer__SetAPDefSSID(const char* default_name);

#endif /* MAIN_HTTP_SERVER_H_ */
