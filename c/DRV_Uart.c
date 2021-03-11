/*******************************************************************
Copyright 
File name: DRV_Uart.c
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

bit UartSendFlag = 0;     //Send interrupt flag bits
bit UartReceiveFlag = 0; //Receive interrupt flag bits
#ifdef UART_LOG
UCHAR ucReceiveData[RECEIVE_DATA_LENGTH] = {0};
UCHAR ucReceiveDatalength = 0;
#endif
//BOOL boIsReceiveComplete = FALSE;


/**************************************************************************
* @function name: 		Uart0_Init
* @Description: 		  Uart Initialize
* @param:			
* @return:						NA	
* @Author: 			
* @Date: 			
***************************************************************************/
void Uart0_Init(void)    //Select Timer1 as the baud rate signal generator
{
#ifdef UART_LOG
	TX_PORT = 1;		 //TX initial high level£»
	SCON = 0X50;     //Mode 1, allowing data to be received
	PCON |= 0X80; 
	T2CON = 0x00;	//Use timer 1 as the UART clock
	TMOD |= 0X20;     //Timer 1, 8-bit automatic reload
	TMCON |= 0X02;    //Timer1   Fsys£»
	TL1 = 217;		
	TH1 = 217;		 //UART Band rate 12M condition is 19200£»
	TR1 = 1;		 //Open Timer1
	EUART = 1;	     //Enable UART interrupt
	EA = 1;		     //SETB EA
#endif
}

/*
void Uart0_Init(void)    //Select Timer2 as the baud rate signal generator
{
	P1CON &= 0XF3;
	P1PH |= 0X0C;	 //Tx is the strong push output, Rx is the band pull input;
	SCON  = 0X50;    //Set the communication mode to mode 1 to allow receiving
	PCON |= 0X80; 
	TMCON |= 0X04;
	T2MOD = 0X00;
	T2CON = 0X30;
	RCAP2H = 0xFF;
	RCAP2L = 0xB2;
	TR2 = 1;
	EUART = 1;     //Enable Uart0 interrupt
	EA = 1;
}
*/

/**************************************************************************
* @function name: 		UartInt
* @Description: 		  Uart Interrupt
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void UartInt(void) interrupt 4
{
#ifdef UART_LOG
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		RI = 0;	
		ucReceiveData[ucReceiveDatalength++] = SBUF;
		if(ucReceiveDatalength == sizeof(ucReceiveData))
		{
			UartReceiveFlag = 1;
			ucReceiveDatalength = 0;
		}
	}	
#endif
}

/**************************************************************************
* @function name: 		TimerUartSendString
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TimerUartSendString(UCHAR *pucTxString)
{
    UCHAR *pucTemp = pucTxString;
  
    if (NULL == pucTemp)
    {
        return;
    }
    
    while (*pucTemp != '\0')
    {
        
			  SBUF=*pucTemp;
				while(!UartSendFlag);
		    UartSendFlag = 0;
        pucTemp++;
    }
    return;
}

/**************************************************************************
* @function name: 		TimerUartSendByte
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void TimerUartSendByte(UCHAR ucParameter)
{
	if(ucParameter=='\n')
	{

		SBUF=0xFF; 
		while(!UartSendFlag);
		UartSendFlag=0;			
	}
	else
	{
		SBUF=ucParameter;
		while(!UartSendFlag);
		UartSendFlag=0;
	}
return;
}
/**************************************************************************
* @function name: 		TimerUartReceiveByte
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/

UCHAR *TimerUartReceive()
{
#ifdef UART_LOG	
	if(UartReceiveFlag)
	{
		TimerUartSendString(ucReceiveData);
		UartReceiveFlag = 0;
		return ucReceiveData;
	}
#endif	
	return NULL;
}
/**************************************************************************
* @function name: 		DebugPrintf
* @Description: 		
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
#if 0
void DebugPrintf(CHAR *format, ...)
{
    CHAR acLogBuf[LOG_LINE_LEN] = {0};
    va_list args;

    va_start (args, format);
    vsprintf(acLogBuf, format, args);
    va_end (args);

    TimerUartSendString(acLogBuf);

    return;
}
#endif
/**************************************************************************
* @function name: 		UARTRxDataClear
* @Description: 			Claear UART RX Data
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void UARTRxDataClear(void)
{
#ifdef UART_LOG	
	UCHAR ucLoop = 0;
	
	for(ucLoop=0;ucLoop<sizeof(ucReceiveData);ucLoop++)
	{
		ucReceiveData[ucLoop] = 0;
	}
#endif
	return;
}

/**************************************************************************
* @function name: 		UART_Stop
* @Description: 			Stop Uart Port
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
void UART_Stop(void)
{
	TR1 = 0;
	EUART = 0;
	
	return;
}

/**************************************************************************
* @function name: 		MotorParamTest
* @Description: 			Motore DutyCycle set from uart TX
* @param:			
* @return:						NA
* @Author: 			
* @Date: 			
***************************************************************************/
#ifdef MOTOR_PARAM_TEST
void MotorParamTest(void)
{
	STATIC UCHAR *pReceiveData;
	STATIC USHORT usVIBDutyCycleRX = 0;
	STATIC UCHAR ucVIBDutyCycleRXHigh = 0;
	STATIC UCHAR ucVIBDutyCycleRXLow = 0;
	
			pReceiveData = TimerUartReceive();
		if(*pReceiveData == 0x01)  //Make a sample verify
		{
			ucVIBDutyCycleRXHigh = (*(pReceiveData+1));
			ucVIBDutyCycleRXLow  = (*(pReceiveData+2));
			if((ucVIBDutyCycleRXLow == 0) && (ucVIBDutyCycleRXHigh == 0))
			{
				ucVIBDutyCycleRXLow = 1;
			}
				
			usVIBDutyCycleRX = (3000/((USHORT)(ucVIBDutyCycleRXHigh << 8) + ucVIBDutyCycleRXLow ))-1;
			
			if(usVIBDutyCycleRX >= 255)
			{
				usVIBDutyCycleRX = 255;
			}
			PWMPRD = usVIBDutyCycleRX;
			
			VIBMotorDutySet(*(pReceiveData+3));
			PATMotorDutySet(*(pReceiveData+4));
						
			UARTRxDataClear();
		}
}
#endif