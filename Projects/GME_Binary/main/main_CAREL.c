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

#include "data_types_CAREL.h"
#include "File_System_IS.h"
#include "RTC_IS.h"
#include "modbus_IS.h"
#include "binary_model.h"
#include "utilities.h"

#include "sys.h"

// related to the device (ESP32)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mb_m.h"
#include "gme_types.h"
#include "wifi.h"
#include "mqtt.h"
#include "poll_engine.h"



#ifdef IS_A_GSM_GATEWAY
#include "GSM_Miscellaneous_IS.h"
#endif

#ifdef IS_A_WIFI_GATEWAY

#endif


#define CAREL_CHECK(res, field)  (res == C_TRUE ? printf("OK %s\n", field) : printf("FAIL %s\n", field))

/* Functions implementation -------------------------------------------------------*/

static C_RES retval;
static gme_sm_t sm;



void Carel_Main_Task(void);


void app_main(void)  // main_Carel
{
  //retval = Init_RTC();

  /*
  if (retval != C_SUCCESS)
  {


  }
  */

  xTaskCreate(Carel_Main_Task, "Carel_Task", 3*CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

}

void Carel_Main_Task(void)
{

  while(1)
  {
	  switch (sm)
	  {
		  //System Initialization
		  case GME_INIT:
		  {
			  retval = Sys__Init();								// RELATED TO ESP32 BOARD
			  CAREL_CHECK(retval, "SYSTEM");

			  sm = GME_CONFIG;
			  break;
		  }
		  //Start and configure WiFi interface
		  case GME_CONFIG:
		  {
			  uint8_t config_status;
			  config_status = Sys__Config(Sys_GetConfigSM());  // RELATED TO ESP32 BOARD

			  if(GME_REBOOT == config_status){
				  sm = GME_REBOOT;
			  }
			  else if(GME_STRAT_SYS == config_status){
				  sm = GME_STRAT_SYS;
			  }

			  break;
		  }
          //Starting the main functionalities of the GME
          case GME_STRAT_SYS:
          {
          	WiFi__WaitConnection();                         // RELATED TO ESP32 BOARD

          	// following call should have parameterized args
          	retval = RTC_Init("pool.ntp.org", 123);							// Carel
          	CAREL_CHECK(retval, "TIME");

          	Utilities__Init();								// da capire se CAREL o ESP32

  			//MQTT__Start();


          	sm = GME_START_POLLING_ENGINE;

  			break;
          }
          case GME_START_POLLING_ENGINE:
          {
          	if(1){    // MQTT_GetFlags().init == 1

          	    retval = BinaryModel_Init();		// CAREL
          	    CAREL_CHECK(retval, "MODEL");

          	    retval = Modbus_Init(19200);        // CAREL
          	    CAREL_CHECK(retval, "UART");

          	    vTaskDelay(1000 / portTICK_PERIOD_MS);

          	    Modbus_Task_Start();                // CAREL
          	    vTaskDelay(1000 / portTICK_PERIOD_MS);

          	    CarelEngineMB_Init();				// CAREL


  				sm = GME_IDLE;
  			}else{
  				sm = GME_START_POLLING_ENGINE;
  			}

  			break;
          }


          case GME_IDLE:
          	//TODO
              WiFi__WaitConnection();

              if(MQTT__GetFlags().init == 1){
              	MQTT__PeriodicTasks();			// manage the MQTT subscribes
             }

              //If we received a new WiFi configuration during system running (Re-Configure)
              if(IsConfigReceived()){
              	sm = GME_CONFIG;
              	Sys_SetConfigSM(WAITING_FOR_HTML_CONF_PARAMETERS);
              }
              break;


          //Reboot ESP after 5 seconds
          case GME_REBOOT:
          	for(int i=5; i>0; i--)
          	{
          		printf("Rebooting after %d sec ...\n",i);
          		vTaskDelay(1000/portTICK_RATE_MS);
          	}
              printf("Rebooting now ...\n");
              fflush(stdout);
              esp_restart();

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







  //vTaskDelay(1000 / portTICK_PERIOD_MS);

  //xTaskCreate(&Modbus_rw_test, "MODBUS_TEST", 2*1024, NULL, 4, NULL);



  /*for(;;)
  {
	  //printf("in main loop every 5sec\n");
	  //printf("valore letto %d, %d\n", test_value[0], test_value[1]);

	  vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
*/



}











