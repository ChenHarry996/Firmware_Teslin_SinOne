/*******************************************************************
Copyright 
File name: DRV_ADC.c
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

void ADC_Init(UCHAR Channel);



bit AdcFlag = 0;

#define ADC_SAMPLE_COUNT 10


/**************************************************************************
* @function name: 		Select ADC Port
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
#if (IC_MODEL == SC92F7323)
	enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=15};
#endif

#if (IC_MODEL == SC92F7322)
	enum Channel {AIN0=0,AIN1=1,AIN4=4,AIN5=5,AIN6,AIN7,AIN8,AIN9,VDD4=15};
#endif
	
#if (IC_MODEL == SC92F7321)
	enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN8=8,AIN9,VDD4=15};
#endif
	
#if (IC_MODEL == SC92F7320)
	enum Channel {AIN0=0,AIN1,AIN6=6,AIN7,VDD4=15};
#endif
	
/**************************************************************************
* @function name: 		ADC_Init
* @Description: 		 	Switch ADC Channel
* @param:							Channel
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ADC_Init(UCHAR Channel)
{
	/*Switch Reference Voltage*/

	if((Channel == ADC_BUTTON_CHANNEL) ||
		(Channel == ADC_TEMPERATURE_CHANNEL))
	{
		OPINX  = 0XC2;  // ADC register.

		OPREG &=~ 0X80;  // Reference VDD
		_nop_();
		_nop_();
	}
	else if(Channel == ADC_BATTERY_CHANNEL)
	{
		OPINX  = 0XC2;  // ADC register.
		OPREG |= 0X80;  // Reference 2.4V
		_nop_();
		_nop_();
	}
		
	ADCCON = 0X80|Channel;		//Open ADC,sample frequency 2M, select the Channel ADC sampling port
	if(Channel < 8)
	{
		ADCCFG0 = 1<<Channel;   //Set Channel as sample port
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //Set Channel as sample port
	}


	IE |= 0X40;        //Open ADC interrupt
	EA = 1;
	
	return;
}

/**************************************************************************
* @function name: 		ADC_Interrupt
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X10);  //Clear interrupt bit
	AdcFlag = 1;
}

/**************************************************************************
* @function name: 		ADC_Sample
* @Description: 			
* @param:							Channel
* @return:			      ADC Sample Value
* @Author: 			
* @Date: 			
***************************************************************************/
USHORT ADC_Sample(UCHAR Channel)
{
	UCHAR   ucLoop = 0;
	UINT    uiAdcValue = 0;
	USHORT  usMinAd = 0x0fff;
	USHORT  usMaxAd = 0x0000;
	USHORT  ADCValue = 0;
	
	ADC_Init(Channel);
	
	for (ucLoop = 0; ucLoop < ADC_SAMPLE_COUNT; ucLoop++) 
	{
		
		ADCCON |= 0X40;						  //Start ADC Sample
		while(!AdcFlag);	         		 //Wait sample complete
		AdcFlag = 0;
		ADCValue = ((UINT)ADCVH<<4)+(ADCVL>>4);

		if(ADCValue > usMaxAd)
		{
			usMaxAd = ADCValue;
		}
		if(ADCValue < usMinAd)
		{
			usMinAd = ADCValue;
		}
		uiAdcValue += ADCValue;   //Sum sample value
	}

		uiAdcValue -= usMinAd;
		uiAdcValue -= usMaxAd;
		uiAdcValue >>= 3;		//Average

		
	//AdcFlag = 0;

	return ((USHORT)uiAdcValue);
}
/**************************************************************************
* @function name: 		HAL_ADCPhysicalVoltage
* @Description: 		Sample and calculate the ADC Physical value
                      Reference voltage is  fixed 2.4v, if use this port need 
											consider add parameter
* @param:							ADC_chanel
* @return:						Physical voltage
* @Author: 			
* @Date: 			
***************************************************************************/

USHORT ADCPhysicalVoltage(UCHAR ADC_chanel)
{
//	  UCHAR  ucAdcTestValue[1]={0};
    ULONG  ulAdcSampleValue = 0;
    USHORT usADCPhysicalVoltage = 0;

    ulAdcSampleValue = ADC_Sample(ADC_chanel);
	usADCPhysicalVoltage =((ulAdcSampleValue*1000)/4096)*2400/1000; // Voltage MV, Reference 2.4V

//		ucAdcTestValue[1] = usADCPhysicalVoltage&0xff;
//		ucAdcTestValue[0] = (usADCPhysicalVoltage>>8)&0xff;			
//		ADCUartSendString(ucAdcTestValue);

   return usADCPhysicalVoltage;
}

/**************************************************************************
* @function name: 		ADC_Stop
* @Description:				Close ADC function to enter into Low Power
* @param:
* @return:						NA
* @Author:
* @Date:
***************************************************************************/
void ADC_Stop(void)
{
	ADCCON &=~ 0x80;

	return;
}