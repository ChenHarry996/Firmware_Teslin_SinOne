/*******************************************************************
Copyright 
File name: DRV_Device.h
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
#ifndef _DEVICE_H_
#define _DEVICE_H_
//#include "H/Function_Init.H"

typedef enum en_DeviceAppState
{
    DEVICE_APP_STATE_RUN = 0,
    DEVICE_APP_STATE_SLEEP,
    DEVICE_APP_STATE_Charge,
    DEVICE_APP_STATE_LOWPOWER,
}DEVICE_APP_STATE_E;

typedef enum en_DeviceAppModus
{
    DEVICE_APP_MODUS_ONE = 0,
    DEVICE_APP_MODUS_TWO,
    DEVICE_APP_MODUS_BOTT,
}DEVICE_APP_MODUS_E;


#define DEVICE_MOTOR_MODE_NUMBER  3
#define DEVICE_MOTOR_CASE_ONE     1
#define DEVICE_MOTOR_CASE_TWO 	  2
#define DEVICE_MOTOR_CASE_THREE 	3

#define DEVICE_AUTO_OFF_FIVE_MINUTES   60000
#define DEVICE_AUTO_OFF_TEN_MINUTES_COUNT   2

extern void DeviceApp_SetAppState(DEVICE_APP_STATE_E enAppState);
extern DEVICE_APP_STATE_E DeviceApp_GetSystemState(void);

extern void DeviceApp_SetAppModus(DEVICE_APP_MODUS_E enAppModus);
extern DEVICE_APP_MODUS_E DeviceApp_GetModus(void);

extern void DeviceApp_SetMotorMode(void);
extern UCHAR DeviceApp_GetMotorMode(void);

extern void DeviceAPP_SetModus(void);
extern void DeviceAPP_ModusRun(void);

extern void DeviceAutoOff(void);
extern void DeviceAutoOffCount(void);
extern void DeviceAutoOffParamClear(void);

#endif
