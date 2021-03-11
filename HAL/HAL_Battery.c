/*******************************************************************
Copyright 
File name: HAL_Battery.c
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


STATIC USHORT usBatterySampleTimerCount = 0;
STATIC UCHAR ucDeviceAutoOff_TenSec = 0;
STATIC UCHAR ucBatteryTemperatureOverCount = 0;
STATIC UCHAR ucBatteryTemperatureOverPeriod = 0;
STATIC BOOL boBatteryTemperatureOver = FALSE;
STATIC UCHAR ucBatteryLess_3400MV = 0;
#ifdef LOWPOWER_DISABLEMOTOR
STATIC BOOL boBatteryCapacityLow = FALSE;
STATIC UCHAR ucBatteryLess_3200MV = 0;
#endif
STATIC BOOL boBatteryLowIndicate = FALSE;

UCHAR ucBatterySampleFiterCount = 0;
STATIC USHORT usBatterySampleFiterValue[BATTERY_SAMPLE_DATA_LEN]={0};

void BatterySampleTimerCountClear(void);
void TemperatureWarning(void);
/**************************************************************************
* @function name: 		HAL_BatteryADCPhysicalVoltage
* @Description: 			Get the battery physical value
* @param:			
* @return:						Battery Value
* @Author: 			
* @Date: 			
***************************************************************************/
USHORT HAL_BatteryADCPhysicalVoltage(void)
{
	USHORT usBatteryPhysicalValue = 0;
//	UCHAR  ucAdcTestValue[2]={0};
	
	usBatteryPhysicalValue=2*ADCPhysicalVoltage(ADC_BATTERY_CHANNEL); // The partial voltage resistance is the same
	 
//	ucAdcTestValue[2] = usBatteryPhysicalValue&0xff;
//	ucAdcTestValue[1] = (usBatteryPhysicalValue>>8)&0xff;		
//	ucAdcTestValue[0] =	0xee;
//	BatteryUartSendValue(ucAdcTestValue);
   
	return usBatteryPhysicalValue;
}

/**************************************************************************
* @function name: 		BatterySampleFiter
* @Description: 			Make the Battery value stable
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
USHORT BatterySampleFiter(void)
{
	UCHAR ucLoop = 0;
	ULONG ulBatteryValue = 0;
//	UCHAR  ucAdcTestValue[2]={0};
	
	for(ucLoop = 0; ucLoop < BATTERY_SAMPLE_DATA_LEN; ucLoop++)
	{
		if(usBatterySampleFiterValue[ucLoop] == 0)
		{
			usBatterySampleFiterValue[ucLoop] = HAL_BatteryADCPhysicalVoltage(); 		
		}
	}
	usBatterySampleFiterValue[ucBatterySampleFiterCount] = HAL_BatteryADCPhysicalVoltage();
	
	ucBatterySampleFiterCount ++ ;
	
	if(ucBatterySampleFiterCount >= BATTERY_SAMPLE_DATA_LEN)
	{
		ucBatterySampleFiterCount = 0;
	}
	
		for(ucLoop = 0; ucLoop < BATTERY_SAMPLE_DATA_LEN; ucLoop++)
	{
		ulBatteryValue +=(ULONG)usBatterySampleFiterValue[ucLoop];
	}

	ulBatteryValue =ulBatteryValue/ BATTERY_SAMPLE_DATA_LEN;

//	ucAdcTestValue[2] = ulBatteryValue&0xff;
//	ucAdcTestValue[1] = (ulBatteryValue>>8)&0xff;			
//	ucAdcTestValue[0] = 0xff;
//	BatteryUartSendValue(ucAdcTestValue);
	
	return (USHORT)ulBatteryValue;
}


/**************************************************************************
* @function name: 		BatterySampleFrequence
* @Description: 			Set Battery sample frequence count--500ms
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void BatterySampleFrequence(void)
{ 

	usBatterySampleTimerCount++;  // This is defined for Battery ADC sample frequence
	
	if(boBatteryTemperatureOver == TRUE)
	{
		ucBatteryTemperatureOverCount++;
	}
	
	return;
}
/**************************************************************************
* @function name: 		TemperatureOverStateGet
* @Description: 		Temperature exceed limit
* @param:
* @return:				boBatteryTemperatureOver
* @Author:
* @Date:
***************************************************************************/
BOOL TemperatureOverStateGet(void)
{
	return boBatteryTemperatureOver;
}

/**************************************************************************
* @function name: 		TemperatureOverStateSet
* @Description: 		boTemperature
* @param:
* @return:				NA
* @Author:
* @Date:
***************************************************************************/
void TemperatureOverStateSet(BOOL boTemperature)
{
	boBatteryTemperatureOver = boTemperature;

	return;
}
#ifdef LOWPOWER_DISABLEMOTOR
/**************************************************************************
* @function name: 		BatteryCapacityStateGet
* @Description: 		Battery capacity low than 3.1V
* @param:
* @return:				boBatteryCapacityLow
* @Author:
* @Date:
***************************************************************************/
BOOL BatteryCapacityStateGet(void)
{
	return boBatteryCapacityLow;
}

/**************************************************************************
* @function name: 		BatteryCapacityStateSet
* @Description: 		boBatteryCapacity
* @param:
* @return:				NA
* @Author:
* @Date:
***************************************************************************/
void BatteryCapacityStateSet(BOOL boBatteryCapacity)
{
	boBatteryCapacityLow = boBatteryCapacity;

	return;
}
#endif
/**************************************************************************
* @function name: 		HAL_BatteryMonitor
* @Description: 			Charge/Temperature/Power Low Monitor
* @param:			
* @return:			NA
* @Author: 			
* @Date: 			
***************************************************************************/
void HAL_BatteryMonitor(void)
{
	USHORT usBatteryValue = 0;
	CHAR   cBatteryTemperature = 0;
	
	TemperatureWarning();

	if(usBatterySampleTimerCount < CHARGE_LED_FREQUENCE) //500ms sample once and run this logic
	{
	  return;
	}
	
		/*Button lights is high priority*/
	if((IsButtonPressed() == TRUE) ||  
		(DEVICE_APP_STATE_SLEEP == DeviceApp_GetSystemState()))     
	{
		if(DEVICE_APP_STATE_Charge != DeviceApp_GetSystemState())
		{
			BatterySampleTimerCountClear();
					
			return;
		}
	}
	
	/*Monitor Battery Temperature*/
	cBatteryTemperature = HAL_TemperatureMonitor();	
	//cBatteryTemperature = 25;

  usBatteryValue = BatterySampleFiter();//HAL_BatteryADCPhysicalVoltage(); /*Battery actually value*/
	
	/*
	if(usBatteryValue == NULL)  //avoid the value is invalid
	{
		//BatteryUartSendString("ulBatteryValueNULL\r\n");
		return;
	}
	*/
    
	/*Battery < 3.1V Off 10s Off*/
	#if 1
	/*Low Power auto off*/
	if(DEVICE_APP_STATE_Charge != DeviceApp_GetSystemState())
	{
        /*Make sure Batrery sample is stable*/
		if(usBatteryValue < BATTERY_CAPACITY_TEN_PERCENT)
		{
			ucDeviceAutoOff_TenSec++;
			if(ucDeviceAutoOff_TenSec >= BATTERY_SAMPLE_STABLE_TEN_SEC)
			{
//				BatteryUartSendString("ucDeviceAutoOff_TenSec\r\n");
				DeviceOff();
				return;
			}
		}
		else
		{
			ucDeviceAutoOff_TenSec=0;
		}

	}
	#endif
	/*need add no Button pressed logic*/
	if(IsChargeDCInput()== TRUE)
	{
		//DeviceApp_SetAppState(DEVICE_APP_STATE_Charge);
		//BatteryUartSendString("DEVICE_APP_STATE_Charge\r\n");
		if((cBatteryTemperature >= BATTERY_CHARGE_TEMPERATURE_MAX)||(cBatteryTemperature <= BATTERY_CHARGE_TEMPERATURE_MIN))
		{
			/*Charge Temperature over run*/
			
			if(TemperatureOverStateGet() == FALSE)
			{
				ChargeFunctionDisable();
				TemperatureOverStateSet(TRUE);
			}
			
			BatteryUartSendString("ChargeDisable\r\n");
			
		}
		else 
		{
			if(TemperatureOverStateGet() == TRUE)
			{
				TemperatureOverStateSet(FALSE);
			}
				
			if(IsChargeComplete() == TRUE)
			{
				/*Charge Complete*/
				ChargeFunctionDisable();
				LEDCurrentState(LED_ON);
				BatteryUartSendString("FullCharge\r\n");
			}
			else
			{			
				/*Charging*/				
				ChargeFunctionEnable();
				LEDCurrentState(LED_FLASH);
				BatteryUartSendString("Charging\r\n");
			}
		}			
	}
  else
  {	
		/*Discharge Part*/
		if((cBatteryTemperature >= BATTERY_WORK_TEMPERATURE_MAX) || (cBatteryTemperature <= BATTERY_WORK_TEMPERATURE_MIN))
		{
			if(TemperatureOverStateGet() == FALSE)
			{
				/*Need Add other funtion to avoid button and motor*/
				
				TemperatureOverStateSet(TRUE);
			}
		}
		else
		{		
			/*Clear Temperature Flag*/
			if(TemperatureOverStateGet() == TRUE)
			{
				TemperatureOverStateSet(FALSE);
			}
#ifdef LOWPOWER_DISABLEMOTOR
			/*< 3.2V, Stop Motor*/
			if (usBatteryValue < BATTERY_CAPACITY_SIXTEEN_PERCENT)
			{               
				if (BatteryCapacityStateGet() == FALSE)
				{
					ucBatteryLess_3200MV ++ ;
					if(ucBatteryLess_3200MV >= BATTERY_SAMPLE_STABLE_TEN_SEC)
					{
						BatteryCapacityStateSet(TRUE);
						ucBatteryLess_3200MV = 0;
					}
				}
			}
			else
			{
				if (BatteryCapacityStateGet() == TRUE)
				{
					BatteryCapacityStateSet(FALSE);
				}
				ucBatteryLess_3200MV = 0;
			}
#endif
			/*LED Indicate*/
			if(usBatteryValue >= BATTERY_CAPACITY_THIRTYTHREE_PERCENT)		     
			{
					/*>3.4V Motor ON LED ON*/
					LEDCurrentState(LED_ON);
					boBatteryLowIndicate = FALSE;
					ucBatteryLess_3400MV = 0;
					BatteryUartSendString("BatteryEnough\r\n");
			}
			else if(usBatteryValue < BATTERY_CAPACITY_THIRTYTHREE_PERCENT)
			{       					
					if(boBatteryLowIndicate == FALSE)
					{
						ucBatteryLess_3400MV ++ ;
						if(ucBatteryLess_3400MV >= BATTERY_SAMPLE_STABLE_TEN_SEC)
						{
							boBatteryLowIndicate =TRUE;
							ucBatteryLess_3400MV = 0;
						}
					}
					else
					{ 
						LEDCurrentState(LED_FLASH);
						
						BatteryUartSendString("LowPowerLess3.4\r\n");   
					}
	
		
			}
		}
	}

	BatterySampleTimerCountClear();
	return;
}

/**************************************************************************
* @function name: 		LEDTemperatureWarning
* @Description: 			Temperature warning make LED indicate		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TemperatureWarning(void)
{
	if(ucBatteryTemperatureOverCount >= TEMPERATUREOVER_200MS)
	{
		if(ucBatteryTemperatureOverPeriod < TEMPERATUREOVER_1000MS)
		{
			LEDCurrentState(LED_FLASH);
		}
		else if((ucBatteryTemperatureOverPeriod >=TEMPERATUREOVER_1000MS)&&(ucBatteryTemperatureOverPeriod <=TEMPERATUREOVER_3000MS)) // 2s OFF
		{
			LEDCurrentState(LED_OFF);
		}
		else
		{
			ucBatteryTemperatureOverPeriod = 0;
		}	
		ucBatteryTemperatureOverPeriod++;
		ucBatteryTemperatureOverCount = 0;
	}
	
	return;
}

/**************************************************************************
* @function name: 		BatterySampleTimerCountClear
* @Description: 			Clear Paramemeter
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void BatterySampleTimerCountClear(void)
{
	if(usBatterySampleTimerCount >= CHARGE_LED_FREQUENCE)   
	{
		usBatterySampleTimerCount = 0;
	}
	return;
}

/**************************************************************************
* @function name: 		BatterySampleParamClear
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void BatterySampleParamClear(void)
{
	usBatterySampleTimerCount = 0;
	ucDeviceAutoOff_TenSec = 0;
	
	TemperatureOverStateSet(FALSE);

	
	boBatteryLowIndicate = FALSE;

	ucBatteryTemperatureOverCount = 0;
	ucBatteryTemperatureOverPeriod = 0;
    
	ucBatteryLess_3400MV = 0;
	
#ifdef LOWPOWER_DISABLEMOTOR
	ucBatteryLess_3200MV = 0;
	BatteryCapacityStateSet(FALSE);
#endif
	
	return;
}


