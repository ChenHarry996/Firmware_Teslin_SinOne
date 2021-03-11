/*******************************************************************
Copyright 
File name: DRV_Timer.c
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

UCHAR ucPATDutyCycleCount = 0;
UCHAR ucVIBDutyCycleCount = 0;

STATIC void PATMotorTimeRun(void);
STATIC void VIBMotorTimeRun(void);
/**************************************************************************
* @function name: 		Timer_Init
* @Description: 			T0/T1/T2 Init
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void Timer_Init(void)
{ 
	TMCON = 0X06;    //------111 ;Timer0¡¢Tiemr1 and Tiemr2 select CLK Fsys
	
	//T0 Set
	TMOD |= 0x01;                 //0000 0001;Timer0 register mode 1
	TL0 = (65536 - 5000)%256;    //Overflow time£ºCLK is Fsys/12£¬10000*£¨1/Fsys£©=5ms;
	TH0 = (65536 - 5000)/256;
	TR0 = 0;
	ET0 = 1;//Enable Timer 0
	TR0 = 1;//Open Timer 0

	#if 0
	/*Select Timer1 as the baud rate signal generator*/
	//T1 Set
	TMOD |= 0x20;            //0010 0000;Timer1 register mode 2
	TL1 = 256 - 240;   //Overflow time£ºCLK is Fsys£¬240*£¨1/Fsys£©=10us;
	TH1 = 256 - 240;
	TR1 = 0;
	ET1 = 1;//Enable Timer 1
	TR1 = 1;//Open Timer 1		
	#endif

#ifdef TIME2_ENABLE	
	//T2 Set
	T2MOD = 0x00;
	T2CON = 0x00;	 //Set to 16 bit overloaded register
	RCAP2H = (65536-12000)/256;     //Overflow time£ºCLK is Fsys£¬12000*£¨1/Fsys£©=1ms;
	RCAP2L = (65536-12000)%256;
	TR2 = 0;
	ET2 = 1;//Enable Timer 2
	TR2 = 0;//Open Timer 2		
#endif

	EA = 1;	
}

/**************************************************************************
* @function name: 		timer0/1/2() interrupt 1/3/5
* @Description: 			Set system clock frequency Timer0 5MS Timer1 1MS
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void timer0() interrupt 1
{

	TL0 = (65536 - 5000)%256;   // 5MS
	TH0 = (65536 - 5000)/256;

#ifdef MOTOR_PARAM_TEST
	
#else
	BatterySampleFrequence();
	PowerButtonDebounce();
	ButtonBoardsCount();
	MotorSmoothUpdateEnable();
	WatchDogClear();
	DeviceAutoOffCount();
#endif
	
}
/*
void timer1() interrupt 3
{
	P21 = ~P21;
}
*/


void Timer2Int(void) interrupt 5
{		
#ifdef TIME2_ENABLE	
	
    PATMotorTimeRun();
    VIBMotorTimeRun();

	TF2 = 0;   //Clear overflow		
#endif
}

/**************************************************************************
* @function name: 		Delay_ms
* @Description: 		Set system clock frequency
* @param:			
* @return:				NA					 
* @Author: 			
* @Date: 			
***************************************************************************/
void Delay_ms(USHORT usPara)
{
  USHORT usDelayClk=0;

	for(;usPara>0;usPara--)
	{
			for (usDelayClk = 0;usDelayClk < 1200;usDelayClk++)
			{

			}
	}
}
/**************************************************************************
* @function name: 		WatchDogInit
* @Description: 			Initial WatchDog Register
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void WatchDogInit(void)
{
	OPINX = 0XC1;  // Watch Dog register.
	OPREG |= 0X80;
	WDTCON |= 0x10;		 //Clear Watch Dog
  return;
}

/**************************************************************************
* @function name: 		WatchDogStop
* @Description: 			Stop WatchDog to enter into Low Power
* @param:			
* @return:						NA	 
* @Author: 			
* @Date: 			
***************************************************************************/
void WatchDogStop(void)
{
	OPINX  = 0XC1;  // Watch Dog register.
	OPREG &=~ 0X80 ;
	
	ucPATDutyCycleCount = 0;
  return;
}

/**************************************************************************
* @function name: 		WatchDogClear
* @Description: 			Clear WatchDog
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void WatchDogClear(void)
{
	WDTCON |= 0x10;		 //Clear Watch Dog
	
  return;
}
/**************************************************************************
* @function name: 		PATMotorTimeRun
* @Description: 		Use Timer2 to toggle PAT Port
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void PATMotorTimeRun(void)
{
    if(0 == PATMotorDutyCycleRegisterValueGet())
    {
        ucPATDutyCycleCount = 0;
    }
    else
    {    
        ucPATDutyCycleCount++;
        if((ucPATDutyCycleCount >= PATMotorDutyCycleRegisterValueGet()) && (ucPATDutyCycleCount < PAT_MOTOR_FREQUENCE))
        {		
            PAT_MOTOR = 0;
        }
        else if(ucPATDutyCycleCount >= PAT_MOTOR_FREQUENCE)
        {
            ucPATDutyCycleCount = 0;
            PAT_MOTOR = 1;
        }
    }
    return;
}
/**************************************************************************
* @function name: 		VIBMotorTimeRun
* @Description: 		Use Timer2 to toggle VIBs Port
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void VIBMotorTimeRun(void)
{
#ifdef VIB_TIME_ENABLE
    if(0 == VIBMotorDutyCycleRegisterValueGet())
    {
        ucVIBDutyCycleCount = 0;
    }
    else
    {
        ucVIBDutyCycleCount++;
        if((ucVIBDutyCycleCount >= VIBMotorDutyCycleRegisterValueGet()) && (ucVIBDutyCycleCount < VIB_MOTOR_FREQUENCE))
        {		
            VIB_MOTOR = 0;
        }
        else if(ucVIBDutyCycleCount >= VIB_MOTOR_FREQUENCE)
        {
            ucVIBDutyCycleCount = 0;
            VIB_MOTOR = 1;
        }
    }
#endif
    return;
}