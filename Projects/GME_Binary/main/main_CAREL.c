/*
  File  : main_carel.c

  Scope :
  this is ONLY an example of the flow request to make a
  functional system based on the Carel Cloud Library.
  Some routine could be called as a task if an
  operating system is available.
  In case the OS is not available is possible to use the
  routine in a mega-loop but take care that the system are
  able to run without significat jitter.

  Note  :

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_IS.h"
#include "main_CAREL.h"

#include "gme_types.h"
#include "data_types_CAREL.h"
#include "modbus_IS.h"
#include "utilities_CAREL.h"

#include "sys_CAREL.h"
#include "sys_IS.h"

#include "common.h"

#include "wifi.h"
#include "polling_IS.h"
#include "nvm_CAREL.h"
#include "Led_Manager_IS.h"
#include "http_server_IS.h"
#include "mobile.h"
#include "radio.h"
#include "binary_model.h"

#define CAREL_CHECK(res, field)  (res == C_SUCCESS ? printf("OK %s\n", field) : printf("FAIL %s\n", field))

/* Functions implementation -------------------------------------------------------*/

//Variables
static gme_sm_t sm = GME_INIT;

void app_main(void)  // main_Carel
{
  Led_Task_Start();
  Carel_Main_Task_Start();
}

void Carel_Main_Task(void)
{
  C_RES retval;
  static bool once = false;
  static uint32_t waiting_conf_timer = 0;
  static uint8_t gw_config_status, line_config_status, devs_config_status;
  static uint8_t test3 = 0;

  static C_UINT32 NVMBoudrate;

  while(1)
  {
	  switch (sm)
	  {
		  //System Initialization
		  case GME_INIT:
		  {
			  retval = Sys__Init();
			  CAREL_CHECK(retval, "SYSTEM");

			  if(retval != C_SUCCESS){
				  sm = GME_REBOOT;
			  }else{
				  sm = GME_CHECK_FILES;
			  }
			  break;
		  }


	        case GME_CHECK_FILES:
	        {
	        	if(test3 == 0){
					if(C_SUCCESS == FS_CheckFiles()){
						sm = GME_RADIO_CONFIG;
					}else{
						sm = GME_CHECK_FILES;
						printf("Please be sure that the certificates are uploaded correctly under the following paths:\nCert1: %s\nCert2: %s\n\n",CERT1_SPIFFS,CERT2_SPIFFS);
					}
					test3 = 1;
	        	}

	        	break;
	        }

	        //Start and configure Radio interface
			case GME_RADIO_CONFIG:
			{
				printf("SM__Start .... GME_RADIO_CONFIG\n");
				uint8_t config_status;
				config_status = Radio__Config();

				if(GME_REBOOT == config_status){
					sm = GME_REBOOT;
				}
				else if(GME_WAITING_FOR_INTERNET == config_status){
					sm = GME_WAITING_FOR_INTERNET;
				}
			}
			break;


	        case GME_WAITING_FOR_INTERNET:
	        {
				if(CONNECTED == Radio__GetStatus()){
					printf("SM__Start .... GME_WAITING_FOR_INTERNET\n");
					sm = GME_STRAT_NTC;
				}
				GME__CheckHTMLConfig();

	        	break;
	        }

		  //Starting the main functionalities of the GME....TO MODIFY!!!
		  case GME_STRAT_NTC:
		  {
			//Start all modules, mqtt client, https client, modbus master, ..etc
			if(false == once){
				printf("Radio__Config .... GME_STRAT_MQTT_NTC\n");
				once = true;
			}

			Radio__WaitConnection();

			retval = RTC_Init( WiFi__GetCustomConfig().ntp_server_addr, WiFi__GetCustomConfig().ntp_server_port);
			retval = RTC_Sync();
			CAREL_CHECK(retval, "TIME");

			//Set boot time
			RTC_Set_UTC_Boot_Time();

			Sys__CertAlloc();

			sm = GME_CHECK_GW_CONFIG;

			GME__CheckHTMLConfig();

			break;
		  }


		  //Start and configure Radio interface
		  case GME_CHECK_GW_CONFIG:
		  {
			//Look for model's file, GW config and Line config
			printf("Radio__Config .... GME_CHECK_GW_CONFIG\n");

			NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status);
			NVM__ReadU8Value(SET_LINE_CONFIG_NVM, &line_config_status);
			NVM__ReadU8Value(SET_DEVS_CONFIG_NVM, &devs_config_status);

			if(( CONFIGURED == gw_config_status &&
				 CONFIGURED == line_config_status &&
				 CONFIGURED == devs_config_status))
			{
				sm = GME_SYSTEM_PREPARATION;
			}else{
				printf("gw_config_status = %d \nline_config_status= %d \ndevs_config_status = %d\n", gw_config_status, line_config_status, devs_config_status);
				sm = GME_WAITING_FOR_CONFIG_FROM_MQTT;
			}

			Utilities__Init();
			MQTT_Start();

			GME__CheckHTMLConfig();
			break;
		  }



        case GME_WAITING_FOR_CONFIG_FROM_MQTT:
        {
        	if(RTC_Get_UTC_Current_Time() > waiting_conf_timer + WAITING_CONF_COUNTER){
        		NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status);
        		NVM__ReadU8Value(SET_LINE_CONFIG_NVM, &line_config_status);
        		NVM__ReadU8Value(SET_DEVS_CONFIG_NVM, &devs_config_status);

				if(	CONFIGURED == gw_config_status &&			//Set_GW_config
					CONFIGURED == line_config_status &&			//Set_Line_config
					CONFIGURED == devs_config_status)			//Set_devs_config
				{
					sm = GME_REBOOT;
				}else{
					waiting_conf_timer = RTC_Get_UTC_Current_Time();
				}
        	}

        	GME__CheckHTMLConfig();

        }
        	break;


          case GME_SYSTEM_PREPARATION:
          {
          	Radio__WaitConnection();

            sm = GME_START_POLLING_ENGINE;

  			break;
          }


          case GME_START_POLLING_ENGINE:
          {
          	if(MQTT_GetFlags() == 1){

          	    retval = BinaryModel_Init();		// CAREL
          	    CAREL_CHECK(retval, "MODEL");

          	    if(retval == C_FAIL) {
          	    	// this means loaded model is not valid
          	    	// hence, clear SET_DEVS_CONFIG_NVM flag in nvm and go waiting for a new configuration
          	    	NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, DEFAULT);
          	    	sm = GME_WAITING_FOR_CONFIG_FROM_MQTT;
          	    	break;
          	    }


          	    NVM__ReadU32Value(MB_BAUDRATE_NVM, &NVMBoudrate);

          	    retval = Modbus_Init(NVMBoudrate, GME__GetHEaderInfo()->Rs485Parity, GME__GetHEaderInfo()->Rs485Stop);
          	    CAREL_CHECK(retval, "UART");

          	    Sys__Delay(1000);

          	    Modbus_Task_Start();                // CAREL
          	    Sys__Delay(1000);


         	    PollEngine_MBStart_IS();


  				sm = GME_IDLE_INTERNET_CONNECTED;
  			}else{
  				sm = GME_START_POLLING_ENGINE;
  			}

  			break;
          }


          case GME_IDLE_INTERNET_CONNECTED:
          	//TODO
              Radio__WaitConnection();

              if(MQTT_GetFlags() == 1)
              	MQTT_PeriodicTasks();			// manage the MQTT subscribes

      GME__CheckHTMLConfig();

              break;


          //Reboot GME after 5 seconds
          case GME_REBOOT:
        	  GME__Reboot();
              break;

          default:
              break;
          }

          //If the factory reset button has been pressed for X time (look gme_config.h)
          if(true == Sys__ResetCheck())
          {
          	printf("RESET CHECK DONE STATEMACHINE\n");
          	sm = GME_REBOOT;
          }
      }
}


//********************************************************
//					PRIVATE FUNCTIONS
//********************************************************

//If we received a new WiFi configuration during system running (Re-Configure)
void GME__CheckHTMLConfig(void){
	if(IsConfigReceived()){
		printf("IsConfigReceived\n");
		sm = GME_WIFI_CONFIG;
		WiFi_SetConfigSM(WAITING_FOR_HTML_CONF_PARAMETERS);
	}
}

void GME__Reboot(void){
	for(int i=5; i>0; i--)
	{
		printf("Rebooting after %d sec ...\n",i);
		Sys__Delay(1000);
	}
	printf("Rebooting now ...\n");
	fflush(stdout);
	GME_Reboot_IS();
}



static struct HeaderModel mHeaderModel;

void GME__ExtractHeaderInfo(H_HeaderModel *pt)
{
	mHeaderModel = *pt;
}


H_HeaderModel * GME__GetHEaderInfo(void)
{
   return &mHeaderModel;
}






