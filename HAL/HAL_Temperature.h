/*******************************************************************
Copyright 
File name: HAL_Temperature.h
Author: 	 Harry.Chen
ID: 
Version: 
Date: 
Description: 
Others: 
History: 
		1. 	Date:
			Author: 
			ID:
			Modification:
		2.	Date:
			Author: 
			ID:
			Modification:
*******************************************************************/
#ifndef _HAL_TEMPERATURE_H_
#define _HAL_TEMPERATURE_H_



typedef struct st_NtcResistanceTemperatureMap
{
    CHAR  cTemperature;/* Unit centigrade */
    ULONG ulResistance;/* Unit ohms */
}NTC_RESISTANCE_TEMPERATURE_MAP_S;


extern CHAR HAL_TemperatureMonitor(void);




#endif