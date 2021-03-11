/*******************************************************************
Copyright 
File name: Function_Init.h
Author:  	 Harry.Chen
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

#ifndef _Function_Init_H_
#define _Function_Init_H_
#include "typedef.h"

#include "SC92F732x_C.H"
#include "intrins.H"
//#include <stdarg.h>
//#include "stdio.h"
//#include "string.h"
//#include "math.h"
#include "H/DRV_GPIO.h"
#include "H/DRV_PWM.h"
#include "H/DRV_ADC.h"
#include "H/DRV_Uart.h"
#include "H/DRV_Timer.h"
#include "HAL/HAL_Battery.h"
#include "HAL/HAL_Button.h"
#include "HAL/HAL_Temperature.h"
#include "H/Device.h"
//#include "H/DRV_IAP.h"





extern void EX_Init(void);







/*****************************************
Choose MCU Model
*****************************************/
#define SC92F7323  0
#define SC92F7322  1
#define SC92F7321  2
#define SC92F7320  3

#define IC_MODEL  SC92F7322

#endif