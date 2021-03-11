/*******************************************************************
Copyright 
File name: MAIN.c
Author: Harry.Chen
ID: 
Version: DVT
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
#include "H/Function_Init.H"



void main(void)
{
	EX_Init();
	GPIO_Init();
	//Uart0_Init();
	Timer_Init();
	PWM_Init();
	//TimerUartSendString("Initial\r\n");
	while(1)
	{ 
#ifdef MOTOR_PARAM_TEST
		/*UART TX test port*/
		MotorParamTest();
#else

		PowerButtonProc();
		HAL_BatteryMonitor();
		ButtonBoardsProc();
		DeviceAPP_ModusRun();
		DeviceAutoOff();
//		IAP_Test();
#endif
	}

}