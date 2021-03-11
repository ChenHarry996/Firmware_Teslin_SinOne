/*******************************************************************
Copyright 
File name: DRV_UART.h
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

#ifndef __DRV_UART_H__
#define __DRV_UART_H__




extern void Uart0_Init(void);
extern void TimerUartSendString(UCHAR *pucTxString);
extern void TimerUartSendString(unsigned char *pucTxString);
extern void TimerUartSendByte(UCHAR ucParameter);
//extern void TimerUartReceive(void);
extern UCHAR *TimerUartReceive();
void UARTRxDataClear(void);
void UART_Stop(void);
#if 0
extern void DebugPrintf(CHAR *format, ...);
#endif

#ifdef MOTOR_PARAM_TEST
void MotorParamTest(void)
#endif

#define RECEIVE_DATA_LENGTH 5

#define BatteryUartSendString(pucTxString) 				//TimerUartSendString(pucTxString)
#define BatteryUartSendValue(pucTxString) 				//TimerUartSendString(pucTxString)
#define ButtonUartSendString(pucTxString)  				//TimerUartSendString(pucTxString)
#define DeviceUartSendString(pucTxString)  				//TimerUartSendString(pucTxString)
#define ADCUartSendString(pucTxString)     				//TimerUartSendString(pucTxString)
#define PWMUartSendString(pucTxString)     				//TimerUartSendString(pucTxString)
#define GPIOUartSendString(pucTxString)    				//TimerUartSendString(pucTxString)
#define TEMPERATUREUartSendString(pucTxString)    //TimerUartSendString(pucTxString)

#endif