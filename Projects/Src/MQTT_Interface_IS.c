/**
 * @file   MQTT_Interface_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         real time clock.
 *         Probably the MQTT require a persistent handler typical in every implementations
 *         to pass the connection data between the routine, implement as a global variable
 *         or implement a function to retrieve it   
 *
 */

/* Includes ------------------------------------------------------------------------ */
#include "MQTT_Interface_IS.h"   

  
/* Functions implementations ------------------------------------------------------- */

/**
 * @brief mqtt_client_subscribe
 *          
 * @param param1 Topic name
 * @param param2 Quality of service level
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */
C_RES mqtt_client_subscribe(C_CHAR *topic, C_INT16 qos)
{ /* TO BE implemented */
	
	
  return C_SUCCESS;
}



/**
 * @brief mqtt_client_unsubscribe
 *          
 * @param param1 Topic name
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */

C_RES mqtt_client_unsubscribe(C_CHAR *topic)
{ /* TO BE implemented */

  /* if (...)
     {
	   return C_FAIL;	 
	 }	
  */		
  return C_SUCCESS;
}



/**
 * @brief mqtt_client_publish
 *          
 * @param param1 topic - Topic name
 * @param param2 data  - array of bytes to publish
 * @param param3 len   - num of bytes in data
 * @param param4 qos   - Quality of service level
 * @param param5 retain- indicate if the publish topic is of retain type 
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */
C_RES mqtt_client_publish(C_CHAR *topic, C_BYTE *data, C_INT16 len, C_INT16 qos, C_INT16 retain)
{/* TO BE implemented */

  /* if (...)
     {
		 
	   return C_FAIL;	 
	 }	
  */

  return C_SUCCESS;	
}



/**
 * @brief mqtt_client_start
 *        start the MQTT client engine   
 * @param None
 * @return C_FAIL ie. not connected to internet etc.  C_SUCCESS
 */
C_RES mqtt_client_start(void)
{
	
  /* if (...)
     {
	   return C_FAIL;	 
	 }	
  */
  
  return C_SUCCESS;	
}



/**
 * @brief mqtt_client_stop
 *        stop the MQTT client engine    
 * @param None
 * @return C_FAIL ie. timeout on wait to action completation / C_SUCCESS
 */
C_RES mqtt_client_stop(void)
{/* TO BE implemented */
  
 /* 
   if you using an operating system be sure to wait that all the action
   related to MQTT are in a safe state before to stop
   ie. message are sent and the acknowledge receive etc. 
   For this reason a timeout mechanism must be implemented in this routine
   
 */ 
	
  /* if (...)
     {
	   return C_FAIL;	 
	 }	
  */	
	
	
  return C_SUCCESS;	
}



/**
 * @brief mqtt_client_init
 *        initialize all the data request by the MQTT client engine     
 * @param None
 * @return C_FAIL ie. no memory available (very strange but everything is possible) / C_SUCCESS
 */
C_RES mqtt_client_init(void)
{ /* TO BE implemented */

  /* The below routine must be called every time a subscribed topic message 
     arrive.  
     You must decomment the below rountine and assign to the proper handler
	 or alternatively call it inside your receiving routine.
     
     mqtt_message_received_callback(C_CHAR *msg, C_UINT len) 
  */
	
  return C_SUCCESS;	
}



/**
 * @brief mqtt_client_set_uri
 *        set the MQTT server name and port     
 * @param param1 broker_uri  - the uri of the broker ie. "mqtt-dev.tera.systems"
 * @param param2 broke_port  - the listen port of the broker ie. 8883
 * @return none 
 */
void mqtt_client_set_uri(C_CHAR *broker_uri, C_UINT16 broker_port)
{ /* TO BE implemented */

	
		
}
