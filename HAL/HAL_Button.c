/*******************************************************************
Copyright 
File name: HAL_Button.c
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
#include "HAL/HAL_Button.h"

/*Set button default status BUTTON_STATUS_RELEASED*/
//BUTTON_STATUS_E g_enButtonStatus = BUTTON_STATUS_RELEASED;
STATIC USHORT usPowerButtonDuration = 0;
STATIC USHORT usPowerButtonReleasedDuration = 0;
STATIC USHORT usButtonBoardDuration = 0;

STATIC BOOL boPowerButtonISRTrigged =FALSE;
STATIC BOOL boPowerButtonReleased = FALSE;
STATIC BOOL boStopRegisterClear = FALSE;
STATIC BOOL boIsPowerButtonValidPressed = FALSE;
STATIC BOOL boButtonBoardStartSample = FALSE;
STATIC BOOL boEnableMotorSmoothUpdate = FALSE;
STATIC BOOL boMotorIndicated = FALSE;

STATIC UCHAR ucButtonBoardPressed[BUTTON_PRESSED_NONE+1]={0};
STATIC UCHAR ucIsButtonBoardEventTrigged[BUTTON_PRESSED_NONE+1]={0};
STATIC UCHAR ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;
STATIC UCHAR ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
STATIC UCHAR ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
STATIC UCHAR ucMotorIndicateCount = 0;
STATIC UCHAR ucMotorIndicateNumber = 0;
STATIC UCHAR ucVIBMotorStep = 0;
STATIC UCHAR ucPATMotorStep = 0;
STATIC UCHAR ucVIBCurrentDutyCycle = 0;
STATIC UCHAR ucPATCurrentDutyCycle = 0;
STATIC UCHAR ucMotorChangePeriodCount = 0;

PWM_DUTY_MAP_S g_astPWMDutyMap[DEVICE_MOTOR_MODE_NUMBER+1] = 
{
	{0, 0, 0},
	{DEVICE_MOTOR_CASE_ONE, 	PWM_DUTY_CYCLE_VIB_CASE_ONE_MIN, 	PWM_DUTY_CYCLE_VIB_CASE_ONE_MAX},
	{DEVICE_MOTOR_CASE_TWO, 	PWM_DUTY_CYCLE_VIB_CASE_TWO_MIN, 	PWM_DUTY_CYCLE_VIB_CASE_TWO_MAX},
	{DEVICE_MOTOR_CASE_THREE, 	PWM_DUTY_CYCLE_VIB_CASE_THREE_MIN, 	PWM_DUTY_CYCLE_VIB_CASE_THREE_MAX},
};

/*PAT exercise level */
EXERCISE_LEVEL_MAP_S g_astExerciseLevelMap[EXERCISE_LEVEL_BOTT] =
{
	{EXERCISE_LEVEL_RESERVED, 0},
	{EXERCISE_LEVEL_ONE,      EXERCISE_LEVEL_ONE_DUTY},
	{EXERCISE_LEVEL_TWO, 	  EXERCISE_LEVEL_TWO_DUTY},
	{EXERCISE_LEVEL_THREE,    EXERCISE_LEVEL_THREE_DUTY},
	{EXERCISE_LEVEL_FOUR,     EXERCISE_LEVEL_FOUR_DUTY},
	{EXERCISE_LEVEL_FIVE,     EXERCISE_LEVEL_FIVE_DUTY},
	{EXERCISE_LEVEL_SIX,      EXERCISE_LEVEL_SIX_DUTY},
	{EXERCISE_LEVEL_SEVEN,    EXERCISE_LEVEL_SEVEN_DUTY},
	{EXERCISE_LEVEL_EIGHT,    EXERCISE_LEVEL_EIGHT_DUTY},
	{EXERCISE_LEVEL_NINE,     EXERCISE_LEVEL_NINE_DUTY},
	{EXERCISE_LEVEL_TEN,      EXERCISE_LEVEL_TEN_DUTY},
	{EXERCISE_LEVEL_ELEVEN,   EXERCISE_LEVEL_ELEVEN_DUTY},
	{EXERCISE_LEVEL_TWELEVE,  EXERCISE_LEVEL_TWELEVE_DUTY},
};


void ButtonPressedPlusProcess(void);
void ButtonPressedMinusProcess(void);	
//void ButtonPressedLongPlusProcess(void);
void ButtonPressedLongMinusProcess(void);
void MotorStop(void);
//void UpDownPressedLed(BUTTON_PRESSED_E enButtonPressed);
void UpDownLedFlashCountClear(void);
void DeviceOff(void);

BOOL IsPlusMinusFunctionLedFlashValid(BUTTON_PRESSED_E enButtonBoardsPressed);
/**************************************************************************
* @function name: 		ButtonCurrentStatusGet
* @Description: 		get button current status
* @param:			
* @return:			
		BUTTON_EVENT_E: BUTTON_STATUS_PRESSED - event of press
		BUTTON_EVENT_E: BUTTON_STATUS_RELEASED - event of release 
* @Author: 			
* @Date: 			
***************************************************************************/
BUTTON_STATUS_E ButtonPhysicalStateGet(void)
{
	BUTTON_STATUS_E enButtonStatus = BUTTON_STATUS_BOTT;
	
	if(POWER_BUTTON == 0)/*button pressed*/
	{
		//TimerUartSendString("PRESS\r\n");
		enButtonStatus = BUTTON_STATUS_PRESSED;
	}
	else/*button released*/
	{
		enButtonStatus = BUTTON_STATUS_RELEASED;
		//TimerUartSendString("No\r\n");
	}

	return enButtonStatus;
}

/**************************************************************************
* @function name: 		IsPowerButtonPressed
* @Description: 		get button current status
* @param:			
* @return:			
									  TRUE: Power Button Pressed
										FALSE:Power Button Release
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsPowerButtonPressed(void)
{
	if (BUTTON_STATUS_PRESSED == ButtonPhysicalStateGet())
	{
	    return TRUE;
	}

	return FALSE;
}
/**************************************************************************
* @function name: 		PowerButtonDebounce
* @Description: 		  Debounce
* @param:			
* @return:			
					 Button Press/Release count
* @Author: 			
* @Date: 			
***************************************************************************/
void PowerButtonDebounce(void)	
{
	if(TRUE == boPowerButtonISRTrigged)
	{
		 usPowerButtonDuration ++;
    }
	if(TRUE == boPowerButtonReleased)
	{
		 usPowerButtonReleasedDuration ++;
	}
	return;
}
/**************************************************************************
* @function name: 		PowerButtonISRTrigged
* @Description: 		   Trigger ISR
* @param:			
* @return:			
					 Button trigger interrupt, Then trigger Timer0 to count 
           Button Press Time.
* @Author: 			
* @Date: 			
***************************************************************************/
void PowerButtonISRTrigged(void)
{			
	if(DEVICE_APP_STATE_Charge == DeviceApp_GetSystemState())
	{
	  return;
	}
    boPowerButtonISRTrigged = TRUE;
	
	return;
}
/**************************************************************************
* @function name: 		IsButtonPressed
* @Description: 		Check Device Button Board is Pressed
* @param:			
* @return:			
										 TRUE: Power Button Board Pressed
										 FALSE:Power Button Board Release				 
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsButtonPressed(void)
{
  UCHAR ucLoop = 0;
  BOOL boButtonBoardsPressed = FALSE;
	
	for(ucLoop=0; ucLoop<BUTTON_PRESSED_NONE; ucLoop++)
	{
		if(ucButtonBoardPressed[ucLoop]== TRUE)
		{
			boButtonBoardsPressed = TRUE;
		}
	}
	
	if((TRUE == boButtonBoardsPressed) || (TRUE == IsPowerButtonPressed()))
	{
	  return TRUE;	 
	}
	
	return FALSE;
}
/**************************************************************************
* @function name: 		PowerButtonProc
* @Description: 		  Device ON/OFF
* @param:			
* @return:						NA				 
* @Author: 			
* @Date: 			
***************************************************************************/
void PowerButtonProc(void)
{
//	STATIC UCHAR sucCountValuePrint[1]={0};


	if(DEVICE_APP_STATE_Charge == DeviceApp_GetSystemState())
	{
		if(IsChargeDCInput() != TRUE)
		{
			DeviceApp_SetAppState(DEVICE_APP_STATE_SLEEP);
		}
		else
		{
			return;
		}
	}	
	else if(IsChargeDCInput()== TRUE)
	{
		PCON &= 0X80;
		DeviceApp_SetAppState(DEVICE_APP_STATE_Charge);
		WatchDogInit();
		ChargeFunctionEnable();
		ADBatteryPowerSupplyEnable(); 
		boPowerButtonISRTrigged=FALSE; 
		boIsPowerButtonValidPressed = FALSE;
		boPowerButtonReleased = FALSE;					
		usPowerButtonDuration=0;
		usPowerButtonReleasedDuration=0;
		BatteryUartSendString("Charging Start\r\n");
		
		return;
	}

	/*Device ON/OFF*/
	if(IsPowerButtonPressed() == TRUE)
	{	
		 boPowerButtonReleased = FALSE;					
		 usPowerButtonReleasedDuration=0;   //Clear relevant data after button dither
		 /* Clear Register value*/	
		if(boStopRegisterClear == FALSE)
		{
			PCON &= 0X80;                   	// Clear STOP mode register			
			boStopRegisterClear = TRUE;
			boPowerButtonISRTrigged = TRUE;   //Avoid the second boot into the interrupt failure
			ButtonUartSendString("boStopRegisterClear\r\n");
		}
		if (BUTTON_DITHERING_INTERVAL_NUM == usPowerButtonDuration)
		{      
			boIsPowerButtonValidPressed = TRUE;
			ButtonUartSendString("boIsPowerButtonValidPressed\r\n");
		}

			
		if(usPowerButtonDuration >= BUTTON_PRESSED_DURATION_LONG)  
		{	  
			if (DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState())
			{
					WatchDogInit();
					LEDCurrentState(LED_ON);
					ChargeFunctionEnable();
					TouchFunctionEnable();
					ADBatteryPowerSupplyEnable(); 
					DeviceApp_SetAppState(DEVICE_APP_STATE_RUN);
					DeviceApp_SetAppModus(DEVICE_APP_MODUS_ONE);
					DeviceApp_SetMotorMode();
					ButtonUartSendString("ON\r\n");
			}
			else if (DEVICE_APP_STATE_RUN == DeviceApp_GetSystemState())
			{		
					DeviceOff();
			}
			boPowerButtonISRTrigged=FALSE;			
			usPowerButtonDuration=0;
		}
  }
	else
	{
		if(DEVICE_APP_STATE_RUN == DeviceApp_GetSystemState())
		{
			if (TRUE == boIsPowerButtonValidPressed)  // Power button release
			{
					boPowerButtonReleased = TRUE;
					ButtonUartSendString("Released\r\n");
//				sucCountValuePrint[0]=usPowerButtonReleasedDuration;
//				TimerUartSendString(sucCountValuePrint);
					if (usPowerButtonReleasedDuration >= BUTTON_RELEASE_DITHERING_INTERVAL_NUM)  //
					{
#ifdef FOUR_BUTTON
						
#else						
						if((usPowerButtonDuration >= BUTTON_PRESSED_DURATION_CLICK ) &&
							(usPowerButtonDuration <= BUTTON_PRESSED_DURATION_SHORT))
							{
								LEDCurrentState(PLUS_BUTTON_PRESS);	
								
								DeviceApp_SetMotorMode();
							}
#endif
							boPowerButtonISRTrigged=FALSE; 
							boIsPowerButtonValidPressed = FALSE;
							boPowerButtonReleased = FALSE;					
							ButtonUartSendString("boPowerButtonReleased\r\n");
							usPowerButtonDuration=0;
							usPowerButtonReleasedDuration=0;
						 /*Button Release process can be add there*/
						
					}
			}
		}
		else if (DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState())
		{			
			DeviceOff();
		}
	}
	return;
}
/**************************************************************************
* @function name: 		ButtonBoardsCheck
* @Description: 		  Button Borads check press/release
* @param:			
* @return:			
					 ADC Sample check which button is pressed
* @Author: 			
* @Date: 			
***************************************************************************/
BUTTON_PRESSED_E ButtonBoardsCheck(void)
{
//STATIC UCHAR sucButtonVoltagePrint[1]={0};  
	
	BUTTON_PRESSED_E enButtonPressed = BUTTON_PRESSED_NONE;
	STATIC USHORT usButtonAdcSamplelValue = 0;
	
	usButtonAdcSamplelValue = ADC_Sample(ADC_BUTTON_CHANNEL);
//	sucButtonVoltagePrint[1] = usButtonAdcSamplelValue&0xff;
//	sucButtonVoltagePrint[0] = (usButtonAdcSamplelValue>>8)&0xff;	
//	TimerUartSendString(sucButtonVoltagePrint);
	if(usButtonAdcSamplelValue == NULL)
	{
		ButtonUartSendString("BUTTON_PRESSED_NONE\r\n");
		return enButtonPressed;
	}
#ifdef FOUR_BUTTON
	else if((usButtonAdcSamplelValue >= (MODE_BUTTON_PHYSICAL_VOLTAGE - BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)) &&
		 (usButtonAdcSamplelValue <= (MODE_BUTTON_PHYSICAL_VOLTAGE + BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)))
	{
	   enButtonPressed =	BUTTON_PRESSED_MODE;			
	}
#endif
	else if((usButtonAdcSamplelValue >= (PLUS_BUTTON_PHYSICAL_VOLTAGE - BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)) &&
		 (usButtonAdcSamplelValue <= (PLUS_BUTTON_PHYSICAL_VOLTAGE + BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)))
	{
	   enButtonPressed =	BUTTON_PRESSED_PLUS;			
	}
	else if((usButtonAdcSamplelValue >= (MINUS_BUTTON_PHYSICAL_VOLTAGE - BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)) &&
		 (usButtonAdcSamplelValue <= (MINUS_BUTTON_PHYSICAL_VOLTAGE + BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)))
	{
	   enButtonPressed =	BUTTON_PRESSED_MINUS;			
	}
	else if((usButtonAdcSamplelValue >= (PLUS_MINUS_BUTTON_PHYSICAL_VOLTAGE - BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)) &&
		 (usButtonAdcSamplelValue <= (PLUS_MINUS_BUTTON_PHYSICAL_VOLTAGE + BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE)))
	{
//		TimerUartSendString("BUTTON_PRESSED_PLUS_MINUS\r\n");
	   enButtonPressed =	BUTTON_PRESSED_PLUS_MINUS;			
	}	
	 return enButtonPressed;
}
/**************************************************************************
* @function name: 		ButtonBoardsCount
* @Description: 			Count Button Press time
* @param:			
* @return:			
											Button pressed time
* @Author: 			
* @Date: 			
***************************************************************************/
void ButtonBoardsCount(void)
{
	UCHAR ucLoop = 0;
	
	boButtonBoardStartSample = TRUE;

	for(ucLoop=0; ucLoop < BUTTON_PRESSED_NONE; ucLoop++)
	{
		if(ucButtonBoardPressed[ucLoop]== TRUE)
		{
			usButtonBoardDuration ++;
		}
	}
	
	if(TRUE == boMotorIndicated)
	{
		ucMotorIndicateCount++;
	}
		
	return;
}

/**************************************************************************
* @function name: 		IsButtonBoardsPressed
* @Description: 		  Button click valid
* @param:							BUTTON_PRESSED_E enButtonPressed
* @return:			
											Button clicked
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsButtonBoardsPressed(BUTTON_PRESSED_E enButtonPressed)
{
	if(TRUE == ucButtonBoardPressed[enButtonPressed])
	{
//		if((usButtonBoardDuration >= BUTTON_PRESSED_DURATION_CLICK ) &&
//			(usButtonBoardDuration <= BUTTON_PRESSED_DURATION_SHORT))
		if(usButtonBoardDuration >= BUTTON_PRESSED_DURATION_CLICK ) 
		{
			return TRUE;
		}
	}
	//TimerUartSendString("IsButtonBoardsPressed FALSE\r\n");
	return FALSE;
}
/**************************************************************************
* @function name: 		ButtonBoardsProc
* @Description: 		  Button Boards pressed to trigger event
* @param:			
* @return:			
					 Function should be trigged when button press 
* @Author: 			
* @Date: 			
***************************************************************************/
void ButtonBoardsProc(void)
{
	UCHAR ucLoop = 0;
	
	if((IsPowerButtonPressed() == TRUE)
		||(DEVICE_APP_STATE_RUN != DeviceApp_GetSystemState())
		|| (TRUE == TemperatureOverStateGet()))
	{
		return;
	}
	if(TRUE == boButtonBoardStartSample) // 5ms to sample one ADC data
	{
		boButtonBoardStartSample = FALSE;
		/*Check if there has any button pressed*/
#ifdef FOUR_BUTTON		
		/*Mode Button Check*/
		if(BUTTON_PRESSED_MODE == ButtonBoardsCheck())
		{			
			if(FALSE == ucButtonBoardPressed[BUTTON_PRESSED_MODE])
			{			 
				ucButtonBoardPressed[BUTTON_PRESSED_MODE] = TRUE;  // Button  press to enable count
				//ButtonUartSendString("BUTTON_PRESSED_MODE\r\n");
			}
			else
			{
				LEDCurrentState(PLUS_BUTTON_PRESS);

				if(TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_MODE))  // Button press time is valid
				{
					//ButtonUartSendString("IsButtonBoardsPreValid\r\n");
					if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MODE])
					{				 
						 DeviceApp_SetMotorMode();
						 //DeviceAPP_ModusRun();
						 ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MODE] = TRUE;  // Tigger event once
						 ButtonUartSendString("BUTTON_PRESSED_MODE\r\n");
					}
				}
			}
		}
		/*PLUS_MINUS Both Button Check*/
		 else if(BUTTON_PRESSED_PLUS_MINUS == ButtonBoardsCheck())
#else
		 if(BUTTON_PRESSED_PLUS_MINUS == ButtonBoardsCheck())
#endif
		{
			if(FALSE == ucButtonBoardPressed[BUTTON_PRESSED_PLUS_MINUS])
			{			 
				ucButtonBoardPressed[BUTTON_PRESSED_PLUS_MINUS] = TRUE;		
				//PowerLEDON();  // PLUS OR MINUS LED will be trigged before this will cause LED OFF				
			}

			if(usButtonBoardDuration >= BUTTON_PRESSED_DURATION_LONG)  // Button press 2s to On/Off sensor
			{
				LEDCurrentState(BOTH_BUTTON_PRESS);
				
				if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS_MINUS])
				{
					TouchFunctionSwitch();																						// will add LED flash twice
					ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS_MINUS] = TRUE;  // Tigger event once
					ButtonUartSendString("BUTTON_PRESSED_PLUS_MINUS\r\n");
				}			
			 }			
		}
		/*PLUS Button Check*/
		else if(BUTTON_PRESSED_PLUS == ButtonBoardsCheck())
		{
			if(TRUE == ucButtonBoardPressed[BUTTON_PRESSED_PLUS_MINUS])
			{
				ucButtonBoardPressed[BUTTON_PRESSED_PLUS] = FALSE;
					
				return;
			}

			 if(FALSE == ucButtonBoardPressed[BUTTON_PRESSED_PLUS])
			 {
				 ucButtonBoardPressed[BUTTON_PRESSED_PLUS] = TRUE;
			 }
			 else
			 {
				if(TRUE == IsPlusMinusFunctionLedFlashValid(BUTTON_PRESSED_PLUS))
				{
					LEDCurrentState(PLUS_BUTTON_PRESS);
				}

				if(TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_PLUS))
				{	
					if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS])
					{
						ButtonPressedPlusProcess();
						//DeviceAPP_ModusRun();
						ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS] = TRUE;  // Tigger event once
						ButtonUartSendString("BUTTON_PRESSED_PLUS\r\n");
					}			 
				}

				if(usButtonBoardDuration >= BUTTON_PRESSED_DURATION_LONGER)  // Button press 10s to switch modus
				{
					if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS_LONGER])
					{
						ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS_LONGER] = TRUE;  // Tigger event once
						//ButtonPressedPlusProcess();
						//DeviceAPP_SetModus();
						//MotorIndicate();
						boMotorIndicated = TRUE;
						DeviceAPP_SetModus();						
						ButtonUartSendString("BUTTON_PRESSED_PLUS_LONGER\r\n");
					}

					//ButtonPressedLongPlusProcess();						
				}						 
			 }
		}
		/*MINUS Button Check*/
		else if(BUTTON_PRESSED_MINUS == ButtonBoardsCheck())
		{
			 if(TRUE == ucButtonBoardPressed[BUTTON_PRESSED_PLUS_MINUS])
			 {
				 ucButtonBoardPressed[BUTTON_PRESSED_MINUS] = FALSE;
				
				 return;
			 }
			 
			 if(FALSE == ucButtonBoardPressed[BUTTON_PRESSED_MINUS])
			 {
					ucButtonBoardPressed[BUTTON_PRESSED_MINUS] = TRUE;
			 }
			 else
			 {
				if(TRUE == IsPlusMinusFunctionLedFlashValid(BUTTON_PRESSED_MINUS))
				{
					LEDCurrentState(MINUS_BUTTON_PRESS);
				}

				if(TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_MINUS))
				{	
					if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MINUS])
					{							
						ButtonPressedMinusProcess();
						ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MINUS] = TRUE;  // Tigger event once
						ButtonUartSendString("BUTTON_PRESSED_MINUS\r\n");
					}			 
				}
					
				if(usButtonBoardDuration >= BUTTON_PRESSED_DURATION_LONG)  // Button press 2s to enter into falldown mode
				{
					LEDCurrentState(MINUS_BUTTON_LONG_PRESS);	
					
					if(FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MINUS_LONG])
					{							 
						ButtonPressedLongMinusProcess();
						ucIsButtonBoardEventTrigged[BUTTON_PRESSED_MINUS_LONG] = TRUE;  // Tigger event once
						ButtonUartSendString("BUTTON_PRESSED_MINUS_LONG\r\n");
					}		
									
				}	 
			 }
		}
    	
		#if 0
		/*check if any button has been released, There can add some release logic*/
		else
		{

			if (TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_MODE))
			{
				DeviceApp_SetMotorMode();
				ButtonUartSendString("BUTTON_RELEASED_MODE\r\n");
			}
			else if (TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_PLUS_MINUS))
			{
				ButtonUartSendString("BUTTON_RELEASED_PLUS_MINUS\r\n");
			}
			else if (TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_PLUS))
			{
				//ButtonPressedPlusProcess();
				if (TRUE == IsPlusFunctionLedFlashValid(BUTTON_PRESSED_PLUS))
				{
					LEDCurrentState(PLUS_BUTTON_PRESS);
				}

				if (TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_PLUS))
				{
					if (FALSE == ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS])
					{
						ButtonPressedPlusProcess();
						//DeviceAPP_ModusRun();
						ucIsButtonBoardEventTrigged[BUTTON_PRESSED_PLUS] = TRUE;  // Tigger event once
						ButtonUartSendString("BUTTON_PRESSED_PLUS\r\n");
					}
				}
				ButtonUartSendString("BUTTON_RELEASED_PLUS\r\n");
			}
			else if (TRUE == IsButtonBoardsPressed(BUTTON_PRESSED_MINUS))
			{
				//ButtonPressedMinusProcess();
				ButtonUartSendString("BUTTON_RELEASED_MINUS\r\n");
			}
			}
#endif
			else 
			{
				//TimerUartSendString(ucButtonBoardPressed);
				for (ucLoop = 0; ucLoop < BUTTON_PRESSED_NONE; ucLoop++)
				{
					ucButtonBoardPressed[ucLoop] = FALSE;
					ucIsButtonBoardEventTrigged[ucLoop] = FALSE;
				}
				usButtonBoardDuration = 0;
				//ucMotorIndicateCount = 0;
				//ucMotorIndicateNumber = 0;
				LedBlinkParamemterClear();
			}
		//}
		  //TimerUartSendString("BUTTON_PRESSED_NONE\r\n");
	}
	return;
}

/**************************************************************************
* @function name: 		ButtonPressedPlusProcess
* @Description: 			Plus button function
* @param:			
* @return:						NA					 
* @Author: 			
* @Date: 			
***************************************************************************/

void ButtonPressedPlusProcess(void)
{
	UCHAR ucDeviceMotorMode = 0;
//	STATIC UCHAR sucPWMDutyPrint[4]={0};
	
//	ucPATPWMDutyCycle = PATMotorDutyCycleRegisterValueGet();
//	ucVIBPWMDutyCycle = VIBMotorDutyCycleRegisterValueGet();
	ucDeviceMotorMode = DeviceApp_GetMotorMode();

	if (DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{
		if (ucVIBPWMDutyCycle < PWM_DUTY_CYCLE_VIB_DEFAULT_MAX)
		{

			ucVIBPWMDutyCycle = ucVIBPWMDutyCycle + PWM_DUTY_CYCLE_VIB_STEP;

		}
		else
		{
			ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MAX;
		}
	}	
	else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
	{
		if(ucVIBPWMDutyCycle < g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMax)
		{
			/*if(ucVIBPWMDutyCycle < g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin)
			{
				ucVIBPWMDutyCycle= g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin;
			}*/
			ucVIBPWMDutyCycle = ucVIBPWMDutyCycle + PWM_DUTY_CYCLE_VIB_MODUES_TWO_STEP;
		}
		else
		{
			ucVIBPWMDutyCycle = g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMax;
		}
	}
	
	if(ucPATPWMDutyCycle < PWM_DUTY_CYCLE_PAT_MAX)
	{		
		ucMotorExerciseLevel++;
		//ucPATPWMDutyCycle = ucPATPWMDutyCycle + PWM_DUTY_CYCLE_PAT_STEP;

	}
	else
	{
		ucMotorExerciseLevel = EXERCISE_LEVEL_TWELEVE;
		//ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MAX;
	}
	ucPATPWMDutyCycle = g_astExerciseLevelMap[ucMotorExerciseLevel].ucPWMDuty;
//	sucPWMDutyPrint[1] = ucVIBPWMDutyCycle;
//	sucPWMDutyPrint[2] = ucPATPWMDutyCycle;
//	sucPWMDutyPrint[3] = ucDeviceMotorMode;
//	sucPWMDutyPrint[0] = 0xff;
//	PWMUartSendString(sucPWMDutyPrint);

		
	return;
}
/**************************************************************************
* @function name: 		ButtonPressedMinusProcess
* @Description: 			MINUS button function
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ButtonPressedMinusProcess(void)
{
	UCHAR ucDeviceMotorMode = 0;
//	STATIC UCHAR sucPWMDutyPrint[4]={0};
	
	//ucPATPWMDutyCycle = PATMotorDutyCycleRegisterValueGet();
	//ucVIBPWMDutyCycle = VIBMotorDutyCycleRegisterValueGet();
	ucDeviceMotorMode = DeviceApp_GetMotorMode();

	if (DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{
		if (ucVIBPWMDutyCycle > PWM_DUTY_CYCLE_VIB_DEFAULT_MIN)
		{
			ucVIBPWMDutyCycle = ucVIBPWMDutyCycle - PWM_DUTY_CYCLE_VIB_STEP;
		}
		else
		{
			ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;
		}
	}
	else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
	{
		 if(ucVIBPWMDutyCycle > g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin)
		 {
			ucVIBPWMDutyCycle = ucVIBPWMDutyCycle - PWM_DUTY_CYCLE_VIB_MODUES_TWO_STEP;
		 }
		 else
		 {
			ucVIBPWMDutyCycle=g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin;
		 }
	}
	
	if(ucPATPWMDutyCycle > PWM_DUTY_CYCLE_PAT_MIN)
	{
		ucMotorExerciseLevel--;
		//ucPATPWMDutyCycle = ucPATPWMDutyCycle - PWM_DUTY_CYCLE_PAT_STEP;
	}
	else
	{
		ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
		//ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
	}
	ucPATPWMDutyCycle = g_astExerciseLevelMap[ucMotorExerciseLevel].ucPWMDuty;
//	sucPWMDutyPrint[1] = ucVIBPWMDutyCycle;
//	sucPWMDutyPrint[2] = ucPATPWMDutyCycle;
//	sucPWMDutyPrint[3] = ucDeviceMotorMode;
//	sucPWMDutyPrint[0] = 0xff;
//	PWMUartSendString(sucPWMDutyPrint);
	return;
}

/**************************************************************************
* @function name: 		MotorThreePlusIndication
* @Description: 			Motor three plus
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void MotorThreePlusIndication(void)
{
	if(TRUE == boMotorIndicated)
	{
		if(ucMotorIndicateNumber < MOTOR_INDICATE_NUMBER)
		{
			if(ucMotorIndicateCount == (MOTOR_WORK_DURATION*2))
			{
				ucMotorIndicateCount=0;
				ucMotorIndicateNumber++;
			}
			if(ucMotorIndicateCount == 0)
			{
				ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_STRONG_LEVEL;		
			}
			else if(ucMotorIndicateCount >= MOTOR_WORK_DURATION)
			{
				ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_LEVEL_ZERO;			
			}
			
			VIBMotorDutySet(ucVIBPWMDutyCycle);
			
			return;
		}
		else
		{
			boMotorIndicated = FALSE;
			ucMotorIndicateNumber =0;
			ucMotorIndicateCount=0;
			if (DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
			{
				ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;
			}
			else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
			{
				ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_CASE_ONE_MIN;
			}
			
			ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
			ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
		}
	}
	return;
}
/**************************************************************************
* @function name: 		ButtonPressedLongMinusProcess
* @Description: 			Minus Long pressed function
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ButtonPressedLongMinusProcess(void)
{
	UCHAR ucDeviceMotorMode = 0;
	
	ucDeviceMotorMode = DeviceApp_GetMotorMode();
	
	if( DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{		
		ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;
	}
	else if(DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
	{
		ucVIBPWMDutyCycle = g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin;		
	}
	ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
	ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
	
  return;
}
/**************************************************************************
* @function name: 		IsPlusFunctionLedFlashValid
* @Description: 		When the PWM duty cycle meet the range, PLUS/MINUS button LED should not flash
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsPlusMinusFunctionLedFlashValid(BUTTON_PRESSED_E enButtonBoardsPressed)
{
	UCHAR ucDeviceMotorMode = 0;
	
	ucDeviceMotorMode = DeviceApp_GetMotorMode();

	if( DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{
		if( enButtonBoardsPressed == BUTTON_PRESSED_PLUS)  // PLUS BUTTON
		{
			if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_ONE)
			{
				if((ucVIBPWMDutyCycle < PWM_DUTY_CYCLE_VIB_DEFAULT_MAX)||
					 (ucPATPWMDutyCycle < PWM_DUTY_CYCLE_PAT_MAX))
				{
					return TRUE;
				}
			}
			else if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_TWO)
			{
				if(ucPATPWMDutyCycle < PWM_DUTY_CYCLE_PAT_MAX)
				{
					return TRUE;
				}
			}
			else if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_THREE)
			{
				if(ucVIBPWMDutyCycle < PWM_DUTY_CYCLE_VIB_DEFAULT_MAX)
				{
					return TRUE;
				}
			}
		}
		else if( enButtonBoardsPressed == BUTTON_PRESSED_MINUS)  // MINUS BUTTON
		{
			if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_ONE)
			{
				if((ucVIBPWMDutyCycle > PWM_DUTY_CYCLE_VIB_DEFAULT_MIN)||
					 (ucPATPWMDutyCycle > PWM_DUTY_CYCLE_PAT_MIN))
				{
					return TRUE;
				}
			}
			else if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_TWO)
			{
				if(ucPATPWMDutyCycle > PWM_DUTY_CYCLE_PAT_MIN)
				{
					return TRUE;
				}
			}
			else if( ucDeviceMotorMode == DEVICE_MOTOR_CASE_THREE)
			{
				if(ucVIBPWMDutyCycle > PWM_DUTY_CYCLE_VIB_DEFAULT_MIN)
				{
					return TRUE;
				}
			}
		}
	}
	else if( DEVICE_APP_MODUS_TWO == DeviceApp_GetModus())
	{
		if( enButtonBoardsPressed == BUTTON_PRESSED_PLUS)
		{
			if((ucVIBPWMDutyCycle < g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMax)||
					 (ucPATPWMDutyCycle < PWM_DUTY_CYCLE_PAT_MAX))
			{
				return TRUE;
			}
		}
		else if( enButtonBoardsPressed == BUTTON_PRESSED_MINUS)
		{
			if((ucVIBPWMDutyCycle > g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin)||
					(ucPATPWMDutyCycle > PWM_DUTY_CYCLE_PAT_MIN))
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}
/**************************************************************************
* @function name: 		VIBMotorLevelBackToLowWithCaseChange
* @Description: 		When motor case chage, all motor speed down to low
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void VIBMotorLevelBackToLowWithCaseChange(void)
{
	UCHAR ucDeviceMotorMode = 0;
	
	ucDeviceMotorMode = DeviceApp_GetMotorMode();
	if (DEVICE_APP_MODUS_ONE == DeviceApp_GetModus())
	{
		ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;
	}
	else
	{
		ucVIBPWMDutyCycle = g_astPWMDutyMap[ucDeviceMotorMode].usPWMDutyMin;
	}	
	
	ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
	ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
}
/**************************************************************************
* @function name: 		IsMotorIndicateProcess
* @Description: 			Motor three plus flag to stop other motor process
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsMotorIndicateProcess(void)
{
	return boMotorIndicated;
}

/**************************************************************************
* @function name: 		MotorSmoothUpdateEnable
* @Description: 		 Enable PWM  update the setting value in 250ms 
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void MotorSmoothUpdateEnable(void)
{
	if(TRUE == IsTouchState())
	{
		ucMotorChangePeriodCount++;
		if(ucMotorChangePeriodCount > 2)
		{
			boEnableMotorSmoothUpdate = TRUE;
		}
	}
	else
	{
		ucMotorChangePeriodCount=0;
		boEnableMotorSmoothUpdate = FALSE;
	}
	
return;
}
/**************************************************************************
* @function name: 		MotorSmoothUpdate
* @Description: 		PWM will update the setting value in 250ms 
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void MotorSmoothUpdate(void)
{
	//UCHAR ucVIBRegisterDutyCycle=0;
	
	//ucVIBRegisterDutyCycle = VIBMotorDutyCycleRegisterValueGet();
	
	if(FALSE == TOUCH_EN)
	{
		//PWMUartSendString("0");
		ucVIBCurrentDutyCycle = ucVIBPWMDutyCycle;
		ucPATCurrentDutyCycle = ucPATPWMDutyCycle;
		
		return;		
	}
	
	if(ucVIBCurrentDutyCycle == ucVIBPWMDutyCycle)
	{
		ucVIBMotorStep = 0;
		ucVIBCurrentDutyCycle = ucVIBPWMDutyCycle;

		ucPATMotorStep = 0;
		//PWMUartSendString("1");
		
	}
		
	if(TRUE == boEnableMotorSmoothUpdate)
	{
		boEnableMotorSmoothUpdate = FALSE; // 5MS check once
		ucMotorChangePeriodCount = 0;

		if(ucVIBMotorStep == 0)
		{
			ucVIBMotorStep = ((ucVIBPWMDutyCycle - ucVIBCurrentDutyCycle)/25)+1; //250ms update completed
			
			//PWMUartSendString("5");
		}

		if (ucPATMotorStep == 0)
		{
			ucPATMotorStep = ((ucPATPWMDutyCycle - ucPATCurrentDutyCycle) / 25) + 1; //250ms update completed

			//PWMUartSendString("5");
		}

		if (ucVIBCurrentDutyCycle >= ucVIBPWMDutyCycle)
		{
			ucVIBCurrentDutyCycle = ucVIBPWMDutyCycle;
		}
		else
		{
			ucVIBCurrentDutyCycle = ucVIBCurrentDutyCycle + ucVIBMotorStep;
		}

		if (ucPATCurrentDutyCycle >= ucPATPWMDutyCycle)
		{
			ucPATCurrentDutyCycle = ucPATPWMDutyCycle;
		}
		else
		{
			ucPATCurrentDutyCycle = ucPATCurrentDutyCycle + ucPATMotorStep;
		}

		//PWMUartSendString("2");
		return ;
	}
	
	//PWMUartSendString("3");
	return ;
}
/**************************************************************************
* @function name: 		PATMotorDutyGet
* @Description: 			get current PAT motor Duty cycle
* @param:
* @return:			NA
* @Author:
* @Date:
***************************************************************************/
UCHAR PATMotorDutyGet(void)
{
	return ucPATCurrentDutyCycle;
}
/**************************************************************************
* @function name: 		VIBMotorDutyGet
* @Description: 			get current VIB motor Duty cycle
* @param:
* @return:			NA
* @Author:
* @Date:
***************************************************************************/
UCHAR VIBMotorDutyGet(void)
{
	return ucVIBCurrentDutyCycle;
}

/**************************************************************************
* @function name: 		MotorStop
* @Description: 			Stop motor and make current value to level 1
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void MotorStop(void)
{
	ucVIBPWMDutyCycle = PWM_DUTY_CYCLE_VIB_DEFAULT_MIN;

	ucMotorExerciseLevel = EXERCISE_LEVEL_ONE;
	ucPATPWMDutyCycle = PWM_DUTY_CYCLE_PAT_MIN;
	
	PATMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
	VIBMotorDutySet(PWM_DUTY_CYCLE_LEVEL_ZERO);
	return;
}
/**************************************************************************
* @function name: 		MotorSmoothStepClear
* @Description: 		
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void MotorSmoothStepClear(void)
{
	ucVIBMotorStep = 0;
	ucPATMotorStep = 0;
	ucVIBCurrentDutyCycle = 0;
	ucPATCurrentDutyCycle = 0;
	return;
}
/**************************************************************************
* @function name: 		ButtonTriggerParamClear
* @Description: 		
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ButtonTriggerParamClear(void)
{
	usPowerButtonDuration = 0;
	boPowerButtonISRTrigged = FALSE;
	boStopRegisterClear = FALSE;
	boIsPowerButtonValidPressed = FALSE;
	boPowerButtonReleased = FALSE;
	return;
}
/**************************************************************************
* @function name: 		DeviceOff
* @Description: 		  Device off and enter into low power
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void DeviceOff(void)
{
	LEDOff();
	WatchDogStop();
	TouchFunctionDisable();
	ADBatteryPowerSupplyDisable();
	PWM_Stop();
	ADC_Stop();
	UART_Stop();
	MotorStop();
	DeviceAutoOffParamClear();
	BatterySampleParamClear();
	ButtonTriggerParamClear();
	DeviceApp_SetAppState(DEVICE_APP_STATE_SLEEP);
	DeviceApp_SetMotorMode();
	ButtonUartSendString(" Device OFF\r\n");
	PCON |= 0X02;			// Stop Mode
	_nop_();		
	_nop_();
	_nop_();
	_nop_();

	return;
}

