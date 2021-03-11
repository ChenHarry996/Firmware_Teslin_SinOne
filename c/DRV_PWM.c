/*******************************************************************
Copyright 
File name: DRV_PWM.c
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




STATIC USHORT usPATPWMDutyTimerValue = 0;
STATIC USHORT usVIBPWMDutyTimerValue = 0;

/**************************************************************************
* @function name: 		PWM_Init
* @Description: 			Initial PWM Resgister
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void PWM_Init(void)
{
#ifdef VIB_TIME_ENABLE

#else
#ifdef TIME2_ENABLE
	PWMCON  = 0x12;			//PWM output to P0.1£¬PWM CLK is Fsys/4
    //PWMCON  = 0x17;
#else
	PWMCON  = 0x1A;			//PWM output to P0.1, P0.0£¬PWM timer is Fsys/4
#endif
	
	PWMPRD  = 98;		    //PWM period=(98+1)*(1/3us)=33us	;
	PWMCFG  = 0x00;			//PWM output not reverse, PWM output to P0.0,P0.1,P0.2
 	PWMDTY0 = 0;		    //P0.0 PWM0 Duty
	PWMDTY1 = 0;			  //P0.1 PWM1 Duty 
	//PWMDTY2 = 10;			//P0.2 PWM2 Duty 
	PWMDTYA = 0x00;
	PWMCON |= 0x80;			//Open PWM
	IE1 |= 0x00;			  //Open PWM interrupt
	//EA = 1;
#endif
}

/**************************************************************************
* @function name: 		PWM Interrupt
* @Description: 		
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void PWMInt(void) interrupt 8
{	
	//P20=~P20;	  
}

/**************************************************************************
* @function name: 		PATMotorDutySet
* @Description: 			Set resgister value
* @param:							ucPWMDuty - The PWM Duty cycle will be set.
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void PATMotorDutySet(UCHAR ucPWMDuty)
{
#ifdef TIME2_ENABLE
	
	if(ucPWMDuty == 0)
	{
		/*if(TR2 == 1)
		{
			TR2 = 0;
		}*/
		PAT_MOTOR = 0;
		usPATPWMDutyTimerValue = 0;
		return;
	}
	else
	{
		if(TR2 == 0)
		{
			TR2 = 1;
		}
		usPATPWMDutyTimerValue = (PAT_MOTOR_FREQUENCE * (USHORT)ucPWMDuty)/100;		
	}
	return;
#else
	USHORT usPWMResgisterValue = 0;
	UCHAR ucPWMPRDResgisterValue = 0;
	
	ucPWMPRDResgisterValue = PWMPRD;
	usPWMResgisterValue = ((USHORT)(ucPWMDuty*(ucPWMPRDResgisterValue+1)))/100;
	
	if(usPWMResgisterValue == PWMDTY0)
	{
		return;
	}
	PWMDTY0 = usPWMResgisterValue;

	return;
#endif	
}

/**************************************************************************
* @function name: 		VIBMotorDutySet
* @Description: 		  Set resgister value
* @param:			        ucPWMDuty - The PWM Duty cycle will be set.
* @return:						NA							 
* @Author: 			
* @Date: 			
***************************************************************************/
void VIBMotorDutySet(UCHAR ucPWMDuty)
{
 #ifdef VIB_TIME_ENABLE
	
	if(ucPWMDuty == 0)
	{
		VIB_MOTOR = 0;
		usVIBPWMDutyTimerValue = 0;
		return;
	}
	else
	{
		if(TR2 == 0)
		{
			TR2 = 1;
		}
		usVIBPWMDutyTimerValue = (VIB_MOTOR_FREQUENCE * (USHORT)ucPWMDuty)/100;		
	}
	return;
#else
	USHORT usPWMResgisterValue = 0;
	UCHAR ucPWMPRDResgisterValue = 0;
	
	ucPWMPRDResgisterValue = PWMPRD;
	usPWMResgisterValue = ((USHORT)(ucPWMDuty*(ucPWMPRDResgisterValue+1)))/100;
	
	if(usPWMResgisterValue == PWMDTY1)
	{
		return;
	}
	PWMDTY1 = usPWMResgisterValue;
	
	return;
#endif
}

/**************************************************************************
* @function name: 		PATMotorDutyCycleRegisterValueGet
* @Description: 			
* @param:			
* @return:						PAT Motor Duty Cuycle
* @Author: 			
* @Date: 			
***************************************************************************/
UCHAR PATMotorDutyCycleRegisterValueGet(void)
{
#ifdef TIME2_ENABLE
	return ((UCHAR)usPATPWMDutyTimerValue);
#else
	USHORT usPATPWMDutyCycleRegister = 0;
	UCHAR ucPWMPRDResgisterValue = 0;
	UCHAR ucPWMDUTYResgisterValue = 0;
	
	ucPWMPRDResgisterValue = PWMPRD;
	ucPWMDUTYResgisterValue = PWMDTY0;
	usPATPWMDutyCycleRegister = (USHORT)(ucPWMDUTYResgisterValue*100)/(ucPWMPRDResgisterValue+1);
	
	return ((UCHAR)usPATPWMDutyCycleRegister);
#endif
}

/**************************************************************************
* @function name: 		VIBMotorDutyCycleRegisterValueGet
* @Description: 		
* @param:			
* @return:						VIB Motor Duty Cycle					 
* @Author: 			
* @Date: 			
***************************************************************************/
UCHAR VIBMotorDutyCycleRegisterValueGet(void)
{
#ifdef VIB_TIME_ENABLE
	return ((UCHAR)usVIBPWMDutyTimerValue);
#else
	USHORT usVIBPWMDutyCycleRegister = 0;
	UCHAR ucPWMPRDResgisterValue = 0;
	UCHAR ucPWMDUTYResgisterValue = 0;
	
	ucPWMPRDResgisterValue = PWMPRD;
	ucPWMDUTYResgisterValue = PWMDTY1;
	
	usVIBPWMDutyCycleRegister = (USHORT)(ucPWMDUTYResgisterValue*100)/(ucPWMPRDResgisterValue+1);
	
	return ((UCHAR)usVIBPWMDutyCycleRegister);
#endif
}
/**************************************************************************
* @function name: 		PWM_Stop
* @Description:
* @param:
* @return:			    Stop PWM generate
* @Author:
* @Date:
***************************************************************************/
void PWM_Stop(void)
{
	if(TR2 == 1)
	{
		TR2 = 0;
	}
	
	PWMCON = 0x00;     //Close PWM
}