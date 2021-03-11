/*******************************************************************
Copyright 
File name: HAL_Battery.h
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
#ifndef _HAL_Battery_H_
#define _HAL_Battery_H_
//#include "H/Function_Init.H"


#define BATTERY_CAPACITY_TEN_PERCENT         3100UL   // (4200-3000MV)*0.1 + 3000
#define BATTERY_CAPACITY_SIXTEEN_PERCENT     3200UL   // (4200-3000MV)*0.16 + 3000
#define BATTERY_CAPACITY_THIRTYTHREE_PERCENT 3400UL   // (4200-3000MV)*0.33 + 3000
//#define BATTERY_CAPACITY_FULL        4200UL   // 4200MV*0.25  can't get 4.2V
//#define BATTERY_CAPACITY_FULL_FLOOR  4000UL   // 4200MV*0.25

#define BATTERY_CHARGE_TEMPERATURE_MAX		45
#define BATTERY_CHARGE_TEMPERATURE_MIN      0

#define BATTERY_WORK_TEMPERATURE_MAX      	50
#define BATTERY_WORK_TEMPERATURE_MIN      	0

#define BATTERY_SAMPLE_STABLE_TEN_SEC		20  //20*500MS

#define TEMPERATUREOVER_200MS				40   // 40*5MS
#define TEMPERATUREOVER_1000MS				5   // 5*TEMPERATUREOVER_200MS
#define TEMPERATUREOVER_3000MS				15  // 15*TEMPERATUREOVER_200MS

#define BATTERY_SAMPLE_DATA_LEN				10

extern void HAL_BatteryMonitor(void);
extern void BatterySampleFrequence(void);
extern void BatterySampleParamClear(void);
extern BOOL TemperatureOverStateGet(void);
#ifdef LOWPOWER_DISABLEMOTOR
extern BOOL BatteryCapacityStateGet(void);
#endif


#endif