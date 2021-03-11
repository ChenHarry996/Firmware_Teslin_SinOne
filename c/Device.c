/*******************************************************************
Copyright 
File name: Device.c
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

STATIC DEVICE_APP_STATE_E enDeviceAppState = DEVICE_APP_STATE_SLEEP;
STATIC DEVICE_APP_MODUS_E  enDeviceAppModus = DEVICE_APP_MODUS_BOTT;

STATIC BOOL boIsDeviceWorkWithTouch    = FALSE;
STATIC BOOL boIsDeviceWorkWithoutTouch = FALSE;
STATIC UCHAR ucMotorMode = 0;
STATIC USHORT usDeviceAutoOff_TenMin1 = 0;   
STATIC UCHAR  ucDeviceAutoOff_TenMin2 = 0;   
STATIC USHORT usDeviceAutoOff_FiveMin = 0;
/**************************************************************************
* @function name: 		DeviceApp_GetSystemState
* @Description: 		
* @param:			
* @return:			system State
* @Author: 			
* @Date: 			
***************************************************************************/
DEVICE_APP_STATE_E DeviceApp_GetSystemState(void)
{
    return enDeviceAppState;
}

/**************************************************************************
* @function name: 		DeviceApp_SetAppState
* @Description: 			Set system state
* @param:							DEVICE_APP_STATE_E enAppState
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceApp_SetAppState(DEVICE_APP_STATE_E enAppState)
{
    if (enAppState == enDeviceAppState)
    {
        return;
    }

    enDeviceAppState = enAppState;

    return;
}
/**************************************************************************
* @function name: 		DeviceApp_GetSystemModus
* @Description: 		
* @param:			
* @return:					enDeviceAppModus
* @Author: 			
* @Date: 			
***************************************************************************/
DEVICE_APP_MODUS_E DeviceApp_GetModus(void)
{
    return enDeviceAppModus;
}

/**************************************************************************
* @function name: 		DeviceApp_SetAppModus
* @Description: 			Set device current Modus state
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceApp_SetAppModus(DEVICE_APP_MODUS_E enAppModus)
{
    if (enAppModus == enDeviceAppModus)
    {
        return;
    }
	//TimerUartSendString("DEVICE_APP_MODUS_ONE\r\n");
    enDeviceAppModus = enAppModus;

    return;
}
/**************************************************************************
* @function name: 		DeviceApp_GetSystemMode
* @Description: 		
* @param:			
* @return:			ucMotorMode
* @Author: 			
* @Date: 			
***************************************************************************/
UCHAR DeviceApp_GetMotorMode(void)
{
    return ucMotorMode;
}

/**************************************************************************
* @function name: 		DeviceApp_SetAppMode
* @Description: 			Set motor mode in different modus
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceApp_SetMotorMode(void)
{
	if( DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState())
	{
		ucMotorMode=0;
		DeviceApp_SetAppModus(DEVICE_APP_MODUS_ONE);
		//TimerUartSendString("DEVICE_APP_STATE_SLEEP\r\n");
		return;
	}
   
	if (ucMotorMode < DEVICE_MOTOR_MODE_NUMBER)
	{
		ucMotorMode++;
	}
	else
	{
		ucMotorMode = DEVICE_MOTOR_CASE_ONE;
	}
	VIBMotorLevelBackToLowWithCaseChange();
	
  return;
}

/**************************************************************************
* @function name: 		DeviceAPP_SetModus
* @Description: 			Switch modus
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceAPP_SetModus(void)
{
	if( DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{
		DeviceApp_SetAppModus(DEVICE_APP_MODUS_TWO);
	}
	else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
	{
		DeviceApp_SetAppModus(DEVICE_APP_MODUS_ONE);
	}
	
	ucMotorMode = DEVICE_MOTOR_CASE_ONE;
	
	return;
}
/**************************************************************************
* @function name: 		DeviceApp_SetAppMode
* @Description: 			Motor mmode switch and LED, Motor speed update
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceAPP_ModusRun(void)
{
	UCHAR ucPATDutyTemp = 0;
	UCHAR ucVIBDutyTemp = 0;
	
	MotorThreePlusIndication();
	
	if(( DEVICE_APP_MODUS_BOTT== DeviceApp_GetModus()) 
		||(DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState())
		||(TRUE == IsMotorIndicateProcess()))
	{
		return;
	}
	/*Temperature or BatteryCapacity over limit, Stop motor*/
#ifdef LOWPOWER_DISABLEMOTOR	
	if((TRUE == TemperatureOverStateGet())
	 ||(TRUE == BatteryCapacityStateGet()))
#else
	if(TRUE == TemperatureOverStateGet())
#endif
	{
		VIBMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
		PATMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
		
		return;
	}

	if((TRUE == IsTouchState())|| (FALSE == TOUCH_EN))
	{
		MotorSmoothUpdate();

		ucPATDutyTemp = PATMotorDutyGet();
		ucVIBDutyTemp = VIBMotorDutyGet();

		if( DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
		{
		//DeviceUartSendString("DEVICE_APP_MODUS_ONE\r\n");
			if(DeviceApp_GetMotorMode() == DEVICE_MOTOR_CASE_ONE)
			{
				PATMotorDutySet(ucPATDutyTemp);
				VIBMotorDutySet(ucVIBDutyTemp);
			}
			else if(DeviceApp_GetMotorMode() == DEVICE_MOTOR_CASE_TWO)
			{
				PATMotorDutySet(ucPATDutyTemp);
				VIBMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
			}
			else if(DeviceApp_GetMotorMode() == DEVICE_MOTOR_CASE_THREE)
			{
				PATMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
				VIBMotorDutySet(ucVIBDutyTemp);
			}
		}
		else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
		{
				PATMotorDutySet(ucPATDutyTemp);
				VIBMotorDutySet(ucVIBDutyTemp);
		}
	}
	else
	{
		PATMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
		VIBMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
		//LEDOff();
		MotorSmoothStepClear();
	}
 return;
}

/**************************************************************************
* @function name: 		DeviceAutoOff
* @Description: 			Auto off 
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceAutoOff(void)
{
#if 1
	if((DEVICE_APP_STATE_Charge == DeviceApp_GetSystemState())
		||(DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState()))
	{
		return;
	}

	if(TRUE == TOUCH_EN)
	{
		if(TRUE == IsTouchState())
		{
			boIsDeviceWorkWithTouch = TRUE;
			usDeviceAutoOff_FiveMin=0;
			boIsDeviceWorkWithoutTouch=FALSE;
			/*10 min Off*/
			if(ucDeviceAutoOff_TenMin2 >= DEVICE_AUTO_OFF_TEN_MINUTES_COUNT)
			{
				DeviceOff();
				//TimerUartSendString("ucDeviceAutoOff_TenMin2\r\n");
			}
		}
		else
		{
			boIsDeviceWorkWithTouch = FALSE;
			ucDeviceAutoOff_TenMin2 =0;
			usDeviceAutoOff_TenMin1 =0;
			boIsDeviceWorkWithoutTouch = TRUE;
			/*5 min Off*/
			if(usDeviceAutoOff_FiveMin >= DEVICE_AUTO_OFF_FIVE_MINUTES)
			{
				DeviceOff();
				//TimerUartSendString("usDeviceAutoOff_FiveMin\r\n");
			}
		}
	}
#endif
	return;
}
/**************************************************************************
* @function name: 		DeviceAutoOffCount
* @Description: 		
* @param:			
* @return:					NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceAutoOffCount(void)
{
	if(TRUE == boIsDeviceWorkWithTouch)
	{
		usDeviceAutoOff_TenMin1++;
		if(usDeviceAutoOff_TenMin1 >= DEVICE_AUTO_OFF_FIVE_MINUTES)
		{
			ucDeviceAutoOff_TenMin2++;	// 2 cycles meet 10 min
			usDeviceAutoOff_TenMin1=0;
		}
	}
	else if(TRUE == boIsDeviceWorkWithoutTouch)
	{
		usDeviceAutoOff_FiveMin++;
	}
	return;
}
/**************************************************************************
* @function name: 		DeviceAutoOffParamClear
* @Description: 		
* @param:			
* @return:					NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceAutoOffParamClear(void)
{
	boIsDeviceWorkWithTouch = FALSE;
	ucDeviceAutoOff_TenMin2 = 0;
	usDeviceAutoOff_TenMin1 = 0;
	boIsDeviceWorkWithoutTouch = FALSE;
	usDeviceAutoOff_FiveMin = 0;
	
	return;
}