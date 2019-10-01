/**
 * @file   MQTT_Interface_CAREL.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         real time clock.
 *         Probably the MQTT require a persistent handler typical in every implementations
 *         to pass the connection data between the routine, implement as a global variable
 *         or implement a function to retrieve it   
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "MQTT_Interface_CAREL.h"   
#include "File_System_CAREL.h"


/**
 * @brief mqtt_engine_status contain the status of the MQTT engine 
 *        MQTT_IS_NOT_CONNECTED/MQTT_IS_CONNECTED    
 */  
C_BYTE mqtt_engine_status = MQTT_IS_NOT_CONNECTED;  

/**
 * @brief broker_uri contain the status of the MQTT engine 
 *        MQTT_IS_NOT_CONNECTED/MQTT_IS_CONNECTED    
 */  
//C_URI broker_uri;

/**
 * @brief broker_port contain the status of the MQTT engine 
 *        MQTT_IS_NOT_CONNECTED/MQTT_IS_CONNECTED    
 */  
//C_UINT16 broker_port;



  
  
/* Functions implementations -------------------------------------------------------*/

/**
 * @brief mqtt_subscribe_to_default_topics
 *        
 * @param none
 * @return C_SUCCESS/C_FAIL
 */

C_RES mqtt_subscribe_to_default_topics(void)
{	
  C_GATEWAY_ID dev_id;
  C_RES resval;
  C_MQTT_TOPIC topic;

  if (MQTT_IS_NOT_CONNECTED == mqtt_engine_status) return C_FAIL;
 
  dev_id[0] = 0x00; 
  Get_Gateway_ID(&dev_id);

  /* /req topic is the topic that receive the command from the cloud */    
  topic[0]=0x00;
  strcpy((char*)topic,(char*)dev_id); 
  strcat((char*)topic, "/req");  
  resval = mqtt_client_subscribe(&topic[0], 0);
  
  /* after the subscribing we communicate that we are connected */
  topic[0]=0x00;
  strcpy((char*)topic,(char*)dev_id); 
  strcat((char*)topic, "/connected");
  mqtt_client_publish(&topic[0], (C_BYTE*)"1", 1, 1, 1);

  return C_SUCCESS;
}	



/**
 * @brief mqtt_message_received_callback
 *         
 * @param param1 broker_uri  - the uri of the broker ie. "mqtt-dev.tera.systems"
 * @param param2 broke_port  - the listen port of the broker ie. 8883
 * @return none 
 */
void mqtt_message_received_callback(C_CHAR *msg, C_UINT16 len)
{
	
	
}


