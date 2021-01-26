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

#include"gme_config.h"

/**
 * @brief _gme_sm enum
 *        GME state machine status
 */
typedef enum _gme_sm{
	GME_INIT = 0,
	GME_CHECK_FILES,
	GME_WIFI_CONFIG,
	GME_WAITING_FOR_INTERNET,
	GME_STRAT_NTC,
	GME_CHECK_GW_CONFIG,
	GME_WAITING_FOR_CONFIG_FROM_MQTT,
	GME_SYSTEM_PREPARATION,
	GME_INIT_POLLING_ENGINE,
	GME_START_POLLING_ENGINE,
	GME_IDLE_INTERNET_CONNECTED,
	GME_IDLE_INTERNET_DISCONNECTED,
	GME_PASSING_MODE,
	GME_REBOOT,
	GME_MOBILE_CONFIG,
	GME_RADIO_CONFIG
}gme_sm_t;


/**
 * @brief _config_sm enum
 *        GME configuration state machine status
 */
typedef enum _config_sm{
	CHECK_FOR_CONFIG = 0,
	SET_DEFAULT_CONFIG,
	START_WIFI,
	WAITING_FOR_HTML_CONF_PARAMETERS,
	CONFIGURE_GME,
	GME_MAX,
}config_sm_t;


typedef enum _conf{
	DEFAULT = 0,
	TO_RECONFIGURE = 1,
	CONFIGURED = 2,
}configuration_t;


enum _wifi_interface{
	AP_MODE = 0,
	APSTA_MODE,
};

#endif /* MAIN_GME_TYPES_H_ */
