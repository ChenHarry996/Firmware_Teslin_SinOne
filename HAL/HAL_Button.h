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

#ifndef __HAL_BUTTON_H__
#define __HAL_BUTTON_H__

//#include "H/DRV_PWM.h"

typedef enum en_ButtonStatus
{
	BUTTON_STATUS_PRESSED = 0,
	BUTTON_STATUS_RELEASED,
	BUTTON_STATUS_BOTT,
}BUTTON_STATUS_E;

typedef enum en_ButtonPressed
    
{
	BUTTON_PRESSED_MODE=0,
	BUTTON_PRESSED_PLUS,
	BUTTON_PRESSED_MINUS,
	BUTTON_PRESSED_PLUS_MINUS,
	BUTTON_PRESSED_PLUS_LONGER,
	BUTTON_PRESSED_MINUS_LONG,
	BUTTON_PRESSED_NONE,
}BUTTON_PRESSED_E;

#define BUTTON_PRESSED_DURATION_CLICK           20 //0.1s
#define BUTTON_PRESSED_DURATION_SHORT           400 //2s
#define BUTTON_PRESSED_DURATION_LONG            400   //2S
#define BUTTON_PRESSED_DURATION_LONGER          2000 //10s
#define BUTTON_DITHERING_INTERVAL_NUM           4
#define BUTTON_PRESSED_DURATION_LONGER          2000
#define BUTTON_RELEASE_DITHERING_INTERVAL_NUM   4

#define MOTOR_WORK_DURATION                     40 // 200MS
#define MOTOR_INDICATE_NUMBER                   3 

#define MODE_BUTTON_PHYSICAL_VOLTAGE            738//738//895    //MV
#define PLUS_BUTTON_PHYSICAL_VOLTAGE            1305//1581    //MV
#define MINUS_BUTTON_PHYSICAL_VOLTAGE           1009//1224    //MV
#define PLUS_MINUS_BUTTON_PHYSICAL_VOLTAGE      660//799
#define BUTTON_PHYSICAL_VOLTAGE_TOLOERANCE      20 //MV


#define PWM_DUTY_CYCLE_VIB_CASE_ONE_MIN         36
#define PWM_DUTY_CYCLE_VIB_CASE_ONE_MAX         80

#define PWM_DUTY_CYCLE_VIB_CASE_TWO_MIN         46
#define PWM_DUTY_CYCLE_VIB_CASE_TWO_MAX         90

#define PWM_DUTY_CYCLE_VIB_CASE_THREE_MIN       56
#define PWM_DUTY_CYCLE_VIB_CASE_THREE_MAX       100

#define PWM_DUTY_CYCLE_VIB_LOW_LEVEL            50
#define PWM_DUTY_CYCLE_VIB_MEDIUM_LEVEL         75
#define PWM_DUTY_CYCLE_VIB_STRONG_LEVEL         100

#define PWM_DUTY_CYCLE_VIB_DEFAULT_MIN          45
#define PWM_DUTY_CYCLE_VIB_DEFAULT_MAX          100

#ifdef TIME2_ENABLE	
#define PWM_DUTY_CYCLE_PAT_MIN   EXERCISE_LEVEL_ONE_DUTY//17//30 //15
#define PWM_DUTY_CYCLE_PAT_MAX   EXERCISE_LEVEL_TWELEVE_DUTY//65//30 //70
#define PWM_DUTY_CYCLE_PAT_STEP   5
#else
#define PWM_DUTY_CYCLE_PAT_MIN   70
#define PWM_DUTY_CYCLE_PAT_MAX   92
#define PWM_DUTY_CYCLE_PAT_STEP   2
#endif

#define PWM_DUTY_CYCLE_VIB_STEP   5

#define PWM_DUTY_CYCLE_VIB_MODUES_TWO_STEP   4


#define PWM_DUTY_CYCLE_LEVEL_ZERO 0



typedef struct stPWMDutyLevel
{
	UCHAR ucMotorCase;
	UCHAR usPWMDutyMin;
	UCHAR usPWMDutyMax;
}PWM_DUTY_MAP_S;


//extern void HAL_ButtonInit(void);
//extern BOOL HAL_IsButtonPressed(void);
extern void PowerButtonDebounce(void);
extern void PowerButtonProc(void);
extern void ButtonBoardsProc(void);
extern void ButtonBoardsCount(void);
extern void PowerButtonISRTrigged(void);
extern BOOL IsButtonPressed(void);
extern BOOL IsMotorIndicateProcess(void);
extern UCHAR PATMotorDutyGet(void);
extern UCHAR VIBMotorDutyGet(void);
extern void VIBMotorLevelBackToLowWithCaseChange(void);

extern void MotorSmoothUpdateEnable(void);
extern void MotorSmoothUpdate();
extern void MotorSmoothStepClear(void);

extern void DeviceOff(void);
extern void MotorThreePlusIndication(void);


#endif