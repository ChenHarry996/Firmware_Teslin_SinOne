/*******************************************************************
Copyright 
File name: DRV_PWM.h
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

#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

typedef enum en_ExerciseLevel
{
	EXERCISE_LEVEL_RESERVED = 0,
	EXERCISE_LEVEL_ONE,
	EXERCISE_LEVEL_TWO,
	EXERCISE_LEVEL_THREE,
	EXERCISE_LEVEL_FOUR,
	EXERCISE_LEVEL_FIVE,
	EXERCISE_LEVEL_SIX,
	EXERCISE_LEVEL_SEVEN,
	EXERCISE_LEVEL_EIGHT,
	EXERCISE_LEVEL_NINE,
	EXERCISE_LEVEL_TEN,
	EXERCISE_LEVEL_ELEVEN,
	EXERCISE_LEVEL_TWELEVE,
	EXERCISE_LEVEL_BOTT,
}EXERCISE_LEVEL_E;
/*PAT Duty Cycle need meet the 20-100HZ frequency*/
#define EXERCISE_LEVEL_ONE_DUTY					17
#define EXERCISE_LEVEL_TWO_DUTY					18
#define EXERCISE_LEVEL_THREE_DUTY				20
#define EXERCISE_LEVEL_FOUR_DUTY				22
#define EXERCISE_LEVEL_FIVE_DUTY				25
#define EXERCISE_LEVEL_SIX_DUTY					28
#define EXERCISE_LEVEL_SEVEN_DUTY				31
#define EXERCISE_LEVEL_EIGHT_DUTY				35
#define EXERCISE_LEVEL_NINE_DUTY				40
#define EXERCISE_LEVEL_TEN_DUTY					47
#define EXERCISE_LEVEL_ELEVEN_DUTY			55
#define EXERCISE_LEVEL_TWELEVE_DUTY			65

typedef struct st_ExerciseLevel
{
	EXERCISE_LEVEL_E enExerciseLevel;
	UCHAR ucPWMDuty;
}EXERCISE_LEVEL_MAP_S;

extern void PWM_Init(void);
extern void PATMotorDutySet(UCHAR ucPWMDuty);
extern void VIBMotorDutySet(UCHAR ucPWMDuty);
extern UCHAR PATMotorDutyCycleRegisterValueGet(void);
extern UCHAR VIBMotorDutyCycleRegisterValueGet(void);
extern void PWM_Stop(void);






#endif