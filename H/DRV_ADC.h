/*******************************************************************
Copyright 
File name: DRV_ADC.h
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

#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__

#define ADC_TEMPERATURE_CHANNEL    1
#define ADC_BATTERY_CHANNEL        4
#define ADC_BUTTON_CHANNEL         5

USHORT ADCPhysicalVoltage(UCHAR ADC_chanel);
USHORT ADC_Sample(UCHAR channel);
void ADC_Stop(void);





#endif