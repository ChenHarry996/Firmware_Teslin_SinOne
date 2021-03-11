/*******************************************************************
Copyright 
File name: DRV_GPIO.c
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

#include "H/Function_Init.H"


//STATIC LED_MODE_E enCurrtLEDValue = POWER_BUTTON_PRESS;
//STATIC UCHAR ucLEDFlashCount = 0;
STATIC BOOL boLEDFlashEnable = FALSE;
STATIC BOOL boLEDBlinkEnable = FALSE;
STATIC BOOL boLongPressedLEDBlinkEnable = FALSE;

void UpDownPressedLedBlink(void);
void MinusButtonLongPressedLedBlink(void);
void BothPressedLedBlink(void);
/**************************************************************************
* @function name: 		IO_Init
* @Description: 			Config the IO register
* @param:			
* @return:						NA					
* @Author: 			
* @Date: 			
***************************************************************************/
void IO_Init(void)
{
	#if (IC_MODEL == SC92F7323)  //SC92F7323 GPIO define
	P0CON = 0x00;  //Set P0 to high resistance,pull up mode
	P0PH  = 0xFF;
	P1CON = 0x00;  //Set P1 to high resistance,input mode
	P1PH  = 0x00;
	P2CON = 0xFF;  //Set P2 to strong push-pull mode
	P2PH  = 0x00;
	P5CON = 0xFF;  //Set P5 to strong push-pull mode
	P5PH  = 0x00;
	#endif
	
	#if (IC_MODEL == SC92F7322)  //SC92F7323 GPIO define
	/*Following the PCBA revision C*/
	P0CON = 0x3B; 
	P0PH  = 0x3B;
	//P1CON = 0xC8;  
	P1CON = 0x88; 
	P1PH  = 0xCF;
	P2CON = 0xC0;  
	P2PH  = 0xC0;
	SC92F7322_NIO_Init(); //NO initial IO port configuration 
	#endif	
	
	#if (IC_MODEL == SC92F7321)  //SC92F7321 GPIO define
	P0CON = 0x00;  //Set P0 to high resistance,pull up mode
	P0PH  = 0xFF;
	P1CON = 0x00;  //Set P1 to high resistance,input mode
	P1PH  = 0x00;
	P2CON = 0xFD;  //Set P2 to strong push-pull mode
	P2PH  = 0x00;
	SC92F7321_NIO_Init(); //NO initial IO port configuration 
	#endif
	
	#if (IC_MODEL == SC92F7320)  //SC92F7320 GPIO define
	P1CON = 0x00;  //Set P1 to high resistance,input mode
	P1PH  = 0x00;
	P2CON = 0xFF;  //Set P2 to strong push-pull mode
	P2PH  = 0x00;
	SC92F7320_NIO_Init(); //NO initial IO port configuration 
	#endif
}
/**************************************************************************
* @function name: 		GPIO_Init
* @Description: 			Initial GPIO state
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void GPIO_Init(void)
{
	IO_Init();
  LEDOff();
	ADBatteryPowerSupplyDisable();
	ChargeFunctionEnable();
	TouchFunctionDisable();
	return;
}

/**************************************************************************
* @function name: 		LEDON
* @Description: 		  Mode LED
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void LEDON(void)
{
 	GPIO_OUTPUT_LOW(LED_EN);
	
	return;
}

/**************************************************************************
* @function name: 		LEDOff
* @Description: 			Mode LED
* @param:		
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void LEDOff(void)
{
 	GPIO_OUTPUT_HIGH(LED_EN);
	
	//GPIO_OUTPUT_HIGH(LED_POWER_RIGHT);
	GPIO_OUTPUT_HIGH(LED_POWER_LEFT);
	GPIO_OUTPUT_HIGH(LED_POWER_UP);
	
	return;
}
/**************************************************************************
* @function name: 		LEDFlash
* @Description: 			
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void LEDFlash(void)
{
		GPIO_OUTPUT_TOGGLE(LED_EN);
	
	return;
}

/**************************************************************************
* @function name: 		LEDCurrentState
* @Description: 			Choose Different LED state
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void LEDCurrentState(LED_MODE_E ucLedMode)
{
	switch(ucLedMode)
	{
		case POWER_BUTTON_PRESS:

				break;
		case PLUS_BUTTON_PRESS:

		    UpDownPressedLedBlink();
				break;
		case MINUS_BUTTON_PRESS:

		    UpDownPressedLedBlink();
				break;
		case BOTH_BUTTON_PRESS:

			BothPressedLedBlink();
				break;
		case MINUS_BUTTON_LONG_PRESS:

			MinusButtonLongPressedLedBlink();
				break;		
		case LED_FLASH:
			
				 LEDFlash();
				break;
		case LED_ON:
			
				LEDON();
				break;		
		case LED_OFF:

				LEDOff();
				break;
		
		default:
			
				break;
	}
	return;
}


/**************************************************************************
* @function name: 		UpDownPressedLedBlink
* @Description: 			Plus or Minus button Pressed,need LED toggle
* @param:		
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void UpDownPressedLedBlink(void)  
{
	if(boLEDFlashEnable == TRUE)
	{
		return;
	}
	LEDFlash();
	Delay_ms(100);
	LEDFlash();
	boLEDFlashEnable = TRUE;
	return;
}
/**************************************************************************
* @function name: 		BothPressedLedBlink
* @Description: 		  Plus and Minus button pressed together
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void BothPressedLedBlink(void)
{
	UCHAR ucLoop = 0;
	if(boLEDBlinkEnable)
	{
		return;
	}
	for(;ucLoop<=2;ucLoop++)
	{
		LEDFlash();
		Delay_ms(100);
	}	
	LEDFlash();
	boLEDBlinkEnable = TRUE;	
	
	return;
}
/**************************************************************************
* @function name: 		MinusButtonLongPressedLedBlink
* @Description: 		Minus Button Long Pressed
* @param:
* @return:						NA
* @Author:
* @Date:
***************************************************************************/
void MinusButtonLongPressedLedBlink(void)
{
	if (boLongPressedLEDBlinkEnable == TRUE)
	{
		return;
	}
	LEDFlash();
	Delay_ms(100);
	LEDFlash();
	boLongPressedLEDBlinkEnable = TRUE;
	return;
}
/**************************************************************************
* @function name: 		LedBlinkParamemterClear
* @Description: 			Clear Paramemter
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void LedBlinkParamemterClear(void)
{
	boLEDFlashEnable = FALSE;
	boLEDBlinkEnable = FALSE;
	boLongPressedLEDBlinkEnable = FALSE;
	//TimerUartSendString("3\r\n");
	return;
}
/**************************************************************************
* @function name: 		IsChargeDCInput
* @Description: 			
* @param:			
* @return:			
					FALSE: 	Charge
					TRUE:   DisCharge
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsChargeDCInput(void)
{
	
	if(CHARGE_POWER_ACTIVE == CHARGE_DC_INPUT)
	{
		return TRUE;
	}

	return FALSE;
}

/**************************************************************************
* @function name: 		ChargeFunctionEnable
* @Description: 		  Enable Charge IC Work
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ChargeFunctionEnable(void)
{
	GPIO_OUTPUT_LOW(CHARGE_EN);
	
	return;
}

/**************************************************************************
* @function name: 		ChargeFunctionDisable
* @Description: 			Disable Charge IC
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ChargeFunctionDisable(void)
{
	GPIO_OUTPUT_HIGH(CHARGE_EN);
	
	return;
}
/**************************************************************************
* @function name: 		IsChargeState
* @Description: 			
* @param:			
* @return:			
					FALSE: 	Charging
					TRUE:   FullCharge
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsChargeComplete(void)
{
	if(BATTERY_CHARGE_COMPLETE == CHARGE_STATE)
	{
		return TRUE;
	}

	return FALSE;
}
/**************************************************************************
* @function name: 		IsTouchState
* @Description: 		
* @param:			
* @return:			
					FALSE: 	No Touch
					TRUE:   Touch
* @Author: 			
* @Date: 			
***************************************************************************/
BOOL IsTouchState(void)
{	
	if(TOUCH_SENSOR_ACTIVE == TOUCH_STATE_INPUT)
	{
		return TRUE;
	}

	return FALSE;
}

/**************************************************************************
* @function name: 		TouchFunctionEnable
* @Description: 		  Enable Touch Function 
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TouchFunctionEnable(void)
{
	GPIO_OUTPUT_HIGH(TOUCH_EN);
	
	return;
}

/**************************************************************************
* @function name: 		TouchFunctionDisable
* @Description: 			Disable Touch Function
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TouchFunctionDisable(void)
{
	GPIO_OUTPUT_LOW(TOUCH_EN);
	
	return;
}

/**************************************************************************
* @function name: 		TouchFunctionSwitch
* @Description: 			Switch function
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TouchFunctionSwitch(void)
{
	if(TRUE == TOUCH_EN)
	{
		TouchFunctionDisable();
		GPIOUartSendString("TouchFunctionDisable\r\n");
	}
	else
	{
		TouchFunctionEnable();
		GPIOUartSendString("TouchFunctionEnable\r\n");
	}
	return;
}
/**************************************************************************
* @function name: 		ADBatteryPowerSupplyEnable
* @Description: 			Power the AD sample ciruit, Set the GPIO as Input Pin
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ADBatteryPowerSupplyEnable(void)
{
	P2CON |= 0xC0;
	P2PH  |= 0xC0;
	
	GPIO_OUTPUT_LOW(ADC_EN);
	
	return;
}

/**************************************************************************
* @function name: 		ADBatteryPowerSupplyDisable
* @Description: 			Stop the AD sample ciruit,Set the GPIO as High resistance
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ADBatteryPowerSupplyDisable(void)
{	
	
	/*switch to High in*/
	P2CON &= 0x80;
	P2PH  &= 0x80;
	
	//GPIO_OUTPUT_HIGH(ADC_EN);
	return;
}
