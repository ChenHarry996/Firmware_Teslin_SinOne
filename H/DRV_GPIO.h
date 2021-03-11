
/*******************************************************************
Copyright 
File name: HAL_Button.h
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

#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

typedef enum en_LedMode
{
	POWER_BUTTON_PRESS=1,
	PLUS_BUTTON_PRESS,
	MINUS_BUTTON_PRESS,
	BOTH_BUTTON_PRESS,
	MINUS_BUTTON_LONG_PRESS,
	LED_FLASH,
	LED_ON,
	LED_OFF,
}LED_MODE_E;


#define GPIO_OUTPUT_LOW(port_reg)    port_reg = 0
#define GPIO_OUTPUT_HIGH(port_reg)   port_reg = 1
#define GPIO_OUTPUT_TOGGLE(port_reg) port_reg = ~port_reg

#define CHARGE_DC_INPUT							P20   
#define CHARGE_EN										P27  
#define CHARGE_STATE								P16
#define POWER_BUTTON								P10
#define ADC_EN											P26
//#define LED_POWER_RIGHT						P16
#define LED_POWER_LEFT							P04
#ifdef FOUR_BUTTON
#define LED_POWER_UP								P05
#else
#define LED_POWER_UP								P17
#endif
//#define LED_VIB										P05
#ifdef FOUR_BUTTON
#define LED_EN											P17
#else
#define LED_EN											P05
#endif
#define TOUCH_EN										P03
#define TOUCH_STATE_INPUT						P02
#define PAT_MOTOR										P00
#define VIB_MOTOR										P01

#define RESETPIN										P11
#define TX_PORT											P13

#define CHARGE_LED_FREQUENCE         100     //1HZ
#define BUTTON_LED_FLASH_PERIOD      10     //0.05s
#define BUTTON_LED_BLINK_PERIOD      40     //0.2s

#define CHARGE_POWER_ACTIVE          1
#define TOUCH_SENSOR_ACTIVE          1
#define BATTERY_CHARGE_COMPLETE      1

//extern USHORT usChargeLEDFlshCount;
/*GPIO PORT*/
extern void GPIO_Init(void);

extern void LEDON(void);
extern void LEDOff(void);
extern void LEDCurrentState(LED_MODE_E ucLedMode);

extern void LedBlinkParamemterClear(void);

extern BOOL IsChargeDCInput(void); 
extern BOOL IsChargeComplete(void);
extern void ChargeFunctionEnable(void);
extern void ChargeFunctionDisable(void);

extern BOOL IsTouchState(void);
extern void TouchFunctionEnable(void);
extern void TouchFunctionDisable(void);
extern void TouchFunctionSwitch(void);

extern void ADBatteryPowerSupplyEnable(void);
extern void ADBatteryPowerSupplyDisable(void);

#endif
