#include "H/Function_Init.H"
#include "H/DRV_IAP.h"


UCHAR code *IapAddr = 0x00;

BOOL boTestFlag = FALSE;
UCHAR ucIAPReadValuePrint[1] = { 0 };
/*****************************************************
* @function name: 	IAP_Test
* @Description:		Only enable UART print 2 times	  
* @param:			NA
* @return:			NA	
* @Author: 			
* @Date: 
*****************************************************/
void IAP_Test(void)
{

	UCHAR ucIapReadData = 0x00;   //Store read IAP data

	if ((DEVICE_APP_STATE_RUN == DeviceApp_GetSystemState()) &&  (boTestFlag == FALSE))
	{
		ucIapReadData = IAPRead(0x7f, IapEPPROM);
		//	IAPADE = 0x00;       //MOVC to ROM
		ucIAPReadValuePrint[0]= ucIapReadData;
		if (ucIapReadData <= 2)
		{
			Uart0_Init();		
			ucIapReadData++;
			IAPWrite(0x7f, ucIapReadData, IapEPPROM);
			//	IAPADE = 0x00;       //MOVC to ROM
			boTestFlag = TRUE;
			TimerUartSendString(ucIAPReadValuePrint);
		}
	}

}
/*****************************************************
* @function name: 		IAPWrite
* @Description: 		  
* @param:			UINT Addr,UCHAR Value,UCHAR IAPArea
* @return:						NA	
* @Author: 			
* @Date: 
*****************************************************/
void IAPWrite(UINT Addr,UCHAR Value,UCHAR IAPArea)
{	
	EA = 0;
	IAPDAT = Value;      //Write Data
	IAPADH = ((Addr>>8)&0X7F);   //Write address 5 bits high
	IAPADL = Addr;               //Write address 8 bits low
	
	IAPADE = IAPArea;  //IAPArea=0x00£ºChoose ROM IAPArea=0x02: Choose EEPROM
	IAPKEY = 240;		   //IAP open protection control time
	IAPCTL = 0x06;     //CPU hold time 2ms
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;      //MOVC to ROM
	EA = 1;
}
/*****************************************************
* @function name: 		IAPRead
* @Description: 		  
* @param:			UINT Addr,UCHAR IAPArea
* @return:						ReadValue	
* @Author: 			
* @Date: 	
*****************************************************/
UCHAR IAPRead(UINT Addr,UCHAR IAPArea)
{
	UCHAR ucReadValue = 0x00;
	EA = 0;
	IAPADE = IAPArea;
	ucReadValue = *(IapAddr+Addr); //Read Data
	IAPADE = 0x00;               //MOVC to ROM
	EA = 1;
	return ucReadValue;
}
