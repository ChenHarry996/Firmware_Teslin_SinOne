/*******************************************************************
Copyright 
File name: HAL_Temperature.c
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


code NTC_RESISTANCE_TEMPERATURE_MAP_S stNtcResistanceTemperatureMap[] = 
{

//	{-40 , 4310000},
//	{-35 , 3045000},
//	{-30 , 2176000},
//	{-25 , 1573000},
//	{-20 , 1148000},
//	{-15 ,  846600},
//	{-10 ,  630000},
	{-5  ,  473000},	
	{ 0  ,  358200},
	{ 5  ,  273400},
	{ 10 ,  210200},
	{ 15 ,  162900},
	{ 20 ,  127200},
	{ 25 ,  100000},
	{ 30 ,   79210},
	{ 35 ,   63150},
	{ 40 ,   50670},
	{ 45 ,   40900},
	{ 50 ,   33190},
	{ 55 ,   27090},
	{ 60 ,   22220},
	{ 65 ,   18320},			
//	{ 70 ,   15180},
//	{ 75 ,   12640},
//	{ 80 ,   10570},
//	{ 85 ,    8873},
//	{ 90 ,    7468},
//	{ 95 ,    6307},
//	{100 ,    5353},
//	{105 ,    4568},
//	{110 ,    3918},
//	{115 ,    3374},	
//	{120 ,    2916},
//	{125 ,    2527},
};

/**************************************************************************
* @function name: 		TemperatureValueGet
* @Description: 		
* @param:			
* @return:			Temperature ADC Sample Value
* @Author: 			
* @Date: 			
***************************************************************************/
USHORT TemperatureValueGet(void)
{
	USHORT  usTemperatureSampleValue = 0;
	
	usTemperatureSampleValue = ADC_Sample(ADC_TEMPERATURE_CHANNEL);

	return usTemperatureSampleValue;
}


/**************************************************************************
* @function name: 		HAL_TemperatureMonitor
* @Description: 		
* @param:			
* @return:						Temperature physical value
* @Author: 			
* @Date: 			
***************************************************************************/
CHAR HAL_TemperatureMonitor(void)
{
    USHORT usTemperatureAdcVol = 0;
    float fNtcResistance = 0.0;/* The value of a temperature sensor, in ohms*/
    ULONG ulNtcResistance = 0;/* The value of a temperature sensor, in ohms*/
    ULONG ulResistanceTemp0 = 0;
    ULONG ulResistanceTemp1 = 0;
    CHAR  cNtcTemperature = 0;
    UCHAR ucLoop = 0;
    USHORT usMapSize = 0;/*MAP size*/
//    UCHAR ucTestValue[3]={0};
    
    usTemperatureAdcVol = TemperatureValueGet();

    usMapSize = sizeof(stNtcResistanceTemperatureMap) / sizeof(NTC_RESISTANCE_TEMPERATURE_MAP_S);
	
#if 1
    /* Check ADC Sample value is in range*/
    if ((usTemperatureAdcVol >= 4096) || (0 == usTemperatureAdcVol))
    {
        TEMPERATUREUartSendString("usTemperatureAdcVol overflow!");
            
        cNtcTemperature = 255;

        return cNtcTemperature;
    }
#endif    


   fNtcResistance = ((usTemperatureAdcVol*100000 )/(4096.0 - usTemperatureAdcVol));

   ulNtcResistance = (ULONG)fNtcResistance;


    /*Resistance too large or too small above the critical value take the critical temperature*/
    if (ulNtcResistance > stNtcResistanceTemperatureMap[0].ulResistance)
    {
        cNtcTemperature = stNtcResistanceTemperatureMap[0].cTemperature;
    }
    else if (ulNtcResistance < stNtcResistanceTemperatureMap[usMapSize - 1].ulResistance)
    {
        cNtcTemperature = stNtcResistanceTemperatureMap[usMapSize - 1].cTemperature;
    }
    else
    {
        /* Traverse the temperature MAP resistance value to get the temperature*/
        for (ucLoop = 1; ucLoop < usMapSize; ucLoop++)
        {
            ulResistanceTemp0 = stNtcResistanceTemperatureMap[ucLoop - 1].ulResistance;
            ulResistanceTemp1 = stNtcResistanceTemperatureMap[ucLoop].ulResistance;
                
            if ((ulNtcResistance <= ulResistanceTemp0) && (ulNtcResistance >= ulResistanceTemp1))
            {
                  /*Need to confirm which temperature more close*/
                if ((ulNtcResistance - ulResistanceTemp1) < (ulResistanceTemp0 - ulNtcResistance))                   
                {
                    /*The logic follows that the resistance value is close to Temp1*/
                    cNtcTemperature = stNtcResistanceTemperatureMap[ucLoop].cTemperature;
                }
                else/*The logic follows that the resistance value is close to Temp0*/
                {
                    cNtcTemperature = stNtcResistanceTemperatureMap[ucLoop - 1].cTemperature;
                }

                break;
            }
        }
    }
//		   ucTestValue[0] = cNtcTemperature;
//		   ucTestValue[1] = 0xff;
//		   ucTestValue[2] = '\0';
//		   TEMPERATUREUartSendString(ucTestValue);

		 return cNtcTemperature;

}

