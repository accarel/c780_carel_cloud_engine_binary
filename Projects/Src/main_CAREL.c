/**
 * @file main_carel.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  this is ONLY an example of the flow request to make a  
 *         functional system based on the Carel Cloud Library. 
 *         Some routine could be called as a task if an 
 *         operating system is available.
 *         In case the OS is not available is possible to use the
 *         routine in a mega-loop but take care that the system are
 *         able to run without significat jitter.
 */

#include "data_types_CAREL.h"
#include "File_System_IS.h"
#include "RTC_IS.h"
#include "MQTT_Interface_CAREL.h"


#ifdef IS_A_GSM_GATEWAY
#include "GSM_Miscellaneous_IS.h"
#endif

#ifdef IS_A_WIFI_GATEWAY

#endif



/* Functions implementation -------------------------------------------------------*/


void main_carel(void)
{
  C_RES retval;

  retval = RTC_Init();
  
  if (C_SUCCESS != retval)
  {
	  //reboot ?
  }


  


  /* MQTT Initialize section */
  
  
  broker_uri[0]=0x00;
  broker_port = 0;
  
  //Get_Configuration_Broker_URI(broker_uri);
  //Get_Configuration_Broker_Port(&broker_port);
    
  mqtt_client_set_uri(broker_uri, broker_port);
  
  retval = mqtt_client_init();
  
  if (C_FAIL == retval) 
  {
	  
  }
  else
  {
    retval = mqtt_client_start();
    if (C_SUCCESS == retval) 
    {
	  mqtt_engine_status = MQTT_IS_CONNECTED;	 	  	 
    }
  }
  
  mqtt_subscribe_to_default_topics();
  

}



