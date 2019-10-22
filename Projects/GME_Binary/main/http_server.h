/*
 * HTTPServer.h
 *
 *  Created on: Jun 17, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_HTTP_SERVER_H_
#define MAIN_HTTP_SERVER_H_

#include "common.h"

// Definitions

//	Login HTML Tgas
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


/*
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
*/

typedef enum e_html_pages{
	LOGIN = 0,
	CONFIG,
}html_pages;

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
}html_config_param_t;

typedef struct _html_login_cred_s
{
    char	login_usr[30];
    char	login_pswd[30];
}html_login_cred_t;


/*typedef union _html_config_param2_s
{
  uint8_t all[364];
  struct {
      uint8_t	gateway_mode;
      uint8_t ap_ssid_hidden;
      uint8_t ap_dhcp_mode;
      uint8_t sta_dhcp_mode;
      char   	ap_ssid[30];
      char   	ap_pswd[30];
      char   	ap_ip[30];
      char  	ap_dhcp_ip[30];
      char  	sta_ssid[30];
      char  	sta_encryption[30];
      char  	sta_pswd[30];
      char	sta_static_ip[30];
      char	sta_netmask[30];
      char	sta_gateway_ip[30];
      char	sta_primary_dns[30];
      char	sta_secondary_dns[30];
  }str;
}html_config_param2_t;*/

esp_err_t HTTPServer__StartFileServer (httpd_handle_t server, const char *base_path);
esp_err_t HTTPServer__StopServer(httpd_handle_t server);
html_config_param_t HTTPServer__GetCustomConfig (void);
uint8_t IsConfigReceived(void);

#endif /* MAIN_HTTP_SERVER_H_ */
