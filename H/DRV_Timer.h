/*******************************************************************
Copyright 
File name: DRV_UART.h
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

#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__

#define PAT_MOTOR_FREQUENCE     (1000/20)   //20HZ
#define VIB_MOTOR_FREQUENCE     (1000/20)   //20HZ

extern void	Timer_Init(void);
extern void WatchDogInit(void);
extern void WatchDogStop(void);
extern void WatchDogClear(void);
extern void Delay_ms(USHORT usPara);
#endif