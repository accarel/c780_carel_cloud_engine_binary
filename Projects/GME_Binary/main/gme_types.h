/*
 * gme_types.h
 *
 *  Created on: Jun 14, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_GME_TYPES_H_
#define MAIN_GME_TYPES_H_


typedef enum _gme_sm{
	GME_INIT = 0,
	GME_CONFIG,
	GME_STRAT_SYS,
	GME_START_POLLING_ENGINE,
	GME_IDLE,
	//GME_RESET,
	GME_REBOOT,
}gme_sm_t;


typedef enum _config_sm{
	CHECK_FOR_CONFIG = 0,
	SET_DEFAULT_CONFIG,
	START_WIFI,
	//WAITING_FOR_DEVICES,
	WAITING_FOR_HTML_CONF_PARAMETERS,
	CONFIGURE_GME,
	GME_MAX,
}config_sm_t;

typedef enum _wifi_conf{
	DEFAULT = 0,
	TO_RECONFIGURE = 1,
	CONFIGURED = 2,
}wifi_conf_t;

enum _wifi_interface{
	AP_MODE = 0,
	APSTA_MODE,
};

#endif /* MAIN_GME_TYPES_H_ */
