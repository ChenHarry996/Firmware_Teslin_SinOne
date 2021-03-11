/*******************************************************************
Copyright 
File name: EX_Init.c
Author:    Harry.Chen
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
#include "H/Function_Init.H"


//UCHAR INT1_flag = 0x00;

/**************************************************************************
* @function name: 		EX_Init
* @Description: 		  External interrput register initialize
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void EX_Init(void)
{	
	#if 0
	//Config interrupt port, this has been completed by GPIO.Init
	P1CON &= 0X37;     //Interrupt IO port set to high resistance input
	P1PH  |= 0xC8;     //Interrupt IO port set to high resistance, pull up
	P2CON &= 0XFC;     //Interrupt IO port set to high resistance input
	P2PH  |= 0x03;     //Interrupt IO port set to high resistance, pull up
 #endif
  //Falling Edge Setting	
	INT0F = 0X01 ;    //xxxx 0000  0 Disable  1 Enable
//	INT1F = 0X0C ;    //xxxx xxxx  0 Disable  1 Enable
//  INT2F = 0X30 ;    //0000 xxxx  0 Disable  1 Enable

  //Rising edge setting	
	INT0R = 0X00 ;    //xxxx 0000  0 Disable  1 Enable
//	INT1R = 0X00 ;    //xxxx xxxx  0 Disable  1 Enable
//	INT2R = 0X30 ;    //0000 xxxx  0 Disable  1 Enable	

#ifdef DC_INTERRUPT
	INT2F = 0X00 ;	
	INT2R = 0X10 ; 
#endif

	
	//External interrupt priority setting
	IE  |= 0x01;	//0000 0x0x	
	IP  |= 0X01; 
#ifdef DC_INTERRUPT
	IE1 |= 0x08;		//DC_IN_INT as a interrupt to wake up MCU
	IP1 |= 0X08;
#else
	IE1 |= 0x00;	
	IP1 |= 0X00;
#endif

	EA = 1;
}

/**************************************************************************
* @function name: 		EX0/1/2() interrupt	0/2/10
* @Description: 		  External interrput
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void EX0() interrupt	0
{
	 PowerButtonISRTrigged();
}

void EX1() interrupt	2
{
	/*Add intrrupt logic*/
}

void EX2() interrupt	10
{
#ifdef DC_INTERRUPT
	if(IsChargeState()== TRUE)
	{
		PCON &= 0X80;
		DeviceApp_SetAppState(DEVICE_APP_STATE_Charge);
		
		return;
	}
#endif
}
