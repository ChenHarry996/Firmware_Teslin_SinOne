						 /*--------------------------------------------------------------------------
SC92F732x_C.H

C Header file for SC92F732x microcontroller.
Copyright (c) 2017 Shenzhen SinOne Chip Electronic CO., Ltd.
All rights reserved.
Shenzhen Saiyuan Microelectronics Co., Ltd
Revision:  V0.1    
Date:  2017.06.13
--------------------------------------------------------------------------*/
#ifndef _SC92F732x_H_
#define _SC92F732x_H_

/* ------------------- Byte register-------------------- */
///*CPU*/
sfr 	ACC	  = 0xE0;		    //Accumulator£Á
sfr 	B	    = 0xF0;		    //General Purpose Register B		
sfr 	PSW	  = 0xD0;		    //Program Status Word		
sfr 	DPH	  = 0x83;	    	//Data pointer 8 bits high		
sfr 	DPL	  = 0x82;		    //Data pointer 8 bits low	
sfr 	SP	  = 0x81;		    //Stack Pointer			

/*system*/
sfr 	PCON	= 0x87;	  	  //Power management control register

/*interrupt*/
sfr   IP1   = 0XB9;       //Interrupt priority control register 1
sfr 	IP    = 0xB8;	      //Interrupt priority control register
sfr 	IE    = 0xA8;	      //Interrupt control register
sfr   IE1   = 0XA9;    	  //Interrupt control register 1

/*PORT*/
sfr 	P5PH	= 0xDA;		    //P5 port mode control register 0		
sfr	  P5CON	= 0xD9;		    //P5 port mode control register 1		
sfr 	P5	  = 0xD8;		    //P5 port data register
sfr 	P2PH	= 0xA2;		    //P2 port mode control register 0		
sfr	  P2CON	= 0xA1;		    //P2 port mode control register 1		
sfr 	P2	  = 0xA0;		    //P2 port data register
sfr 	P1PH  = 0x92;		    //P1 port mode control register 0		
sfr	  P1CON	= 0x91;		    //P1 port mode control register 1	
sfr 	P1	  = 0x90;		    //P1 port data register
sfr	  P0PH	= 0x9B;		    //P0 port mode control register 1		
sfr	  P0CON	= 0x9A;		    //P0 port mode control register 1		
sfr 	P0	  = 0x80;		    //P0 port data register
sfr 	IOHCON= 0x97;		    //IOH setting register

/*TIMER*/
sfr 	TMCON	= 0x8E;		    //Timer frequency control register
sfr 	TH1	  = 0x8D;		    //Timer 1 high 8 bits
sfr 	TH0	  = 0x8C;		    //Timer 0 high 8 bits
sfr 	TL1	  = 0x8B;		    //Timer 1 low 8 bits
sfr 	TL0	  = 0x8A;		    //Timer 0 low 8 bits
sfr 	TMOD	= 0x89;		    //Timer operating mode register
sfr 	TCON	= 0x88;		    //Timer control register
sfr   T2CON = 0XC8;       //Timer 2 control register
sfr   T2MOD = 0XC9; 	    //Timer 2 operating mode register
sfr   RCAP2L= 0XCA;       //Timer 2 reload/capture low 8 bits
sfr   RCAP2H= 0XCB;       //Timer 2 reload/capture low 8 bits
sfr   TL2   = 0XCC;       //Timer 2 low 8 bits
sfr   TH2   = 0XCD;       //Timer 2 high 8 bits


/*ADC*/
sfr	  ADCCFG0 = 0xAB;		  //P1 port ADC function configuration register/reference voltage
sfr	  ADCCFG1 = 0xAC;		  //P1 port ADC function configuration register/reference voltage
sfr   ADCCON  = 0XAD;     //ADC control register
sfr 	ADCVL	  = 0xAE;		  //ADC result register
sfr 	ADCVH	  = 0xAF;		  //ADC result register

/*PWM*/
sfr 	PWMCFG	= 0xD1;		  //PWM control register
sfr 	PWMCON	= 0xD2;		  //PWM control register
sfr 	PWMPRD	= 0xD3;		  //PWM period setting register
sfr 	PWMDTYA	= 0xD4;		  //PWM high level set register
sfr 	PWMDTY0	= 0xD5;		  //PWM high level set register
sfr	  PWMDTY1	= 0XD6;		  //PWM high level set register
sfr	  PWMDTY2	= 0XD7;		  //PWM high level set register

///*WatchDog*/
sfr	  BTMCON  = 0XCE;     //Low frequency timer control register
sfr 	WDTCON	= 0xCF;		  //WDT control register

/*LCD*/
sfr   OTCON   = 0X8F;	    //LCD voltage output control register
sfr   P0VO    = 0X9C;     //P0 LCD Bais output register

/*INT*/
sfr   INT0F   = 0XBA; 	  //INT0 falling edge interrupt control register
sfr   INT0R   = 0XBB; 	  //INT0 upper and lower edge interrupt control register
sfr   INT1F   = 0XBC; 	  //INT1 falling edge interrupt control register
sfr   INT1R   = 0XBD; 	  //INT1 upper and lower edge interrupt control register
sfr   INT2F   = 0XC6; 	  //INT2 falling edge interrupt control register
sfr   INT2R   = 0XC7; 	  //INT2 upper and lower edge interrupt control register

/*IAP */
sfr	  IAPCTL	= 0xF6;		  //IAP control register
sfr	  IAPDAT	= 0xF5;		  //IAP data register
sfr   IAPADE  = 0xF4;     //IAP extended address register
sfr	  IAPADH	= 0xF3;		  //IAP write address high register
sfr	  IAPADL	= 0xF2;		  //IAP write address low register
sfr	  IAPKEY	= 0xF1;		  //IAP protection register

/*UART*/
sfr   SCON    = 0X98;  	  //Serial port control register
sfr   SBUF    = 0X99;  	  //Serial port data cache register

/*option*/
sfr   OPINX   = 0XFE;
sfr   OPREG   = 0XFF;

///* ------------------- Bit Register-------------------- */

/*PSW*/
sbit 	CY	= PSW^7;	    //Flag bit					0: When there is no carry in the highest order of addition or no borrow in the highest order of subtraction		1: When the highest order of addition has carry, or the highest order of subtraction has borrow
sbit 	AC	= PSW^6;	    //Auxiliary Carry Flag bit	0: No borrow, carry		1: When bit3 has carry in addition, or bit3 has borrow in subtraction
sbit 	F0	= PSW^5;	    //User flag bit
sbit 	RS1	= PSW^4;	    //Working register set selection bit
sbit 	RS0	= PSW^3;	    //Working register set selection bit
sbit 	OV	= PSW^2;	    //Overflow flag bit
sbit 	F1	= PSW^1;	    //F1 flag
sbit 	P	  = PSW^0;	    //Parity Flag	0:The number of 1 in ACC is even (including 0)		1: The number of 1 in ACC is odd

/*T2CON*/
sbit  TF2	  =  T2CON^7;
sbit  EXF2	=  T2CON^6;
sbit  RCLK	=  T2CON^5;
sbit  TCLK	=  T2CON^4;
sbit  EXEN2	=  T2CON^3;
sbit  TR2	  =  T2CON^2;
sbit  T2    =  T2CON^1;
sbit  CP	  =  T2CON^0;


/*IP*/
sbit 	IPADC	  = IP^6;		 //ADC interrupt priority control bit      				0: Set the interrupt priority of ADC to "low"		1: Set the interrupt priority of ADC to "high"
sbit	IPT2	  = IP^5;		 //PWM interrupt priority control bit 	    		 	0: Set the interrupt priority of PWM to "low"		1:Set the interrupt priority of PWM to "high"
sbit 	IPUART	= IP^4;		 //SIF interrupt priority control bit 	    		 	0: Set the interrupt priority of SIF to "low"		1:Set the interrupt priority of SIF to "high"
sbit 	IPT1	  = IP^3;		 //Timer1 interrupt priority control bit 					0:Set the interrupt priority of Timer1 to "low"	1:Set the interrupt priority of Timer1 to "high"
sbit	IPINT1	= IP^2;		 //32K Base Timer interrupt priority control bit 	0:Set the interrupt priority of 32K to "low"		1:Set the interrupt priority of 32K to "high"
sbit 	IPT0	  = IP^1;	   //Timer0 interrupt priority control bit 					0:Set the interrupt priority of Timer0 to "low"	1:Set the interrupt priority of Timer0 to "high"
sbit	IPINT0	= IP^0;

/*IE*/
sbit 	EA		= IE^7;		//Total control of interrupt enable				0:Turn off all interrupts		1:Turn on all interrupts
sbit 	EADC	= IE^6;		//ADC interrupt enable control						0:Turn off ADC interrupt		1:Turn on ADC interrupt
sbit 	ET2 	= IE^5;		//PWM interrupt enable control						0:Turn off PWM interrupt		1:Turn on PWM interrupt
sbit	EUART = IE^4;		//UART interrupt enable control						0:Turn off SIF interrupt		1:Turn on SIF interrupt
sbit 	ET1 	= IE^3;		//Timer1 interrupt enable control					0:Turn off TIMER1 interrupt	1:Turn on TIMER1 interrupt
sbit	EINT1	= IE^2;		//32K Base Timer interrupt enable control	0:Turn off 32K interrupt		1:Turn on 32K interrupt	
sbit 	ET0		= IE^1;		//Timer0 interrupt enable control					0:Turn off TIMER0 interrupt	1:Turn on TIMER0 interrupt
sbit	EINT0	= IE^0;		//TouchKey interrupt enable control				0:Turn off TouchKey interrupt  1:Turn on TouchKey interrupt

/*TCON*/
sbit 	TF1	= TCON^7;	  //T1 overflow interrupt request flag bit	T1 overflows. When an interrupt occurs, the hardware sets TF1 to "1" and applies for an interrupt. When the CPU responds, the hardware clears "0"¡£
sbit 	TR1	= TCON^6;	  //Timer T1 operation control bit	0:Timer1 stop count		1:Timer1 start count
sbit 	TF0	= TCON^5;	  //T0 overflow interrupt request flag bit	T0 overflows. When an interrupt occurs, the hardware sets TF0 to "1" and applies for an interrupt. When the CPU responds, the hardware clears "0"¡£
sbit 	TR0	= TCON^4;	  //Timer T0 operation control bit	0:Timer0 stop count		1:Timer0 start count

/*SCON*/
sbit	SM0	=	SCON^7;
sbit	SM1	=	SCON^6;
sbit	SM2	=	SCON^5;
sbit	REN	=	SCON^4;
sbit	TB8	=	SCON^3;
sbit	RB8	=	SCON^2;
sbit	TI	=	SCON^1;
sbit	RI	=	SCON^0;

/******************* P0 ******************
*SC92F7323 Encapsulating pins not led out£ºNone
*SC92F7322 Encapsulating pins not led out£ºP06/P07
*SC92F7321 Encapsulating pins not led out£ºP04/P05/P06/P07
*SC92F7320 Encapsulating pins not led out£ºP0
******************************************/
sbit 	P07	= P0^7;
sbit 	P06	= P0^6;
sbit 	P05	= P0^5;
sbit 	P04	= P0^4;
sbit 	P03	= P0^3;
sbit 	P02	= P0^2;
sbit 	P01	= P0^1;
sbit 	P00	= P0^0;

/************************ P1 *********************
*SC92F7323 Encapsulating pins not led out£ºNone
*SC92F7322 Encapsulating pins not led out£ºP14/P15
*SC92F7321 Encapsulating pins not led out£ºP14/P15
*SC92F7320 Encapsulating pins not led out£ºP10/P11/P14/P15/P16/P17
**************************************************/
sbit 	P17	= P1^7;
sbit 	P16	= P1^6;
sbit 	P15	= P1^5;
sbit 	P14	= P1^4;
sbit 	P13	= P1^3;
sbit 	P12	= P1^2;
sbit 	P11	= P1^1;
sbit 	P10	= P1^0;

/******************** P2 *******%**********
*SC92F7323 Encapsulating pins not led out£ºNone
*SC92F7322 Encapsulating pins not led out£ºP22/P23
*SC92F7321 Encapsulating pins not led out£ºP24/P25/P26/P27
*SC92F7320 Encapsulating pins not led out£ºP22/P23/P24/P25
*********************************%*********/
sbit 	P27	= P2^7;
sbit 	P26	= P2^6;
sbit 	P25	= P2^5;
sbit 	P24	= P2^4;
sbit 	P23	= P2^3;
sbit 	P22	= P2^2;
sbit 	P21	= P2^1;
sbit 	P20	= P2^0;

/**************** P5 **************
*SC92F7323 Encapsulating pins not led out£ºNone
*SC92F7322 Encapsulating pins not led out£ºP50/P51
*SC92F7321 Encapsulating pins not led out£ºP50/P51
*SC92F7320 Encapsulating pins not led out£ºP50/P51
***********************************/
sbit 	P51	= P5^1;
sbit 	P50	= P5^0;

/****************************************************************************
*Note£ºTo package the pins that are not led out, it needs to be set to the strong push-pull output mode
*IC Select£ºAccording to the IC model used, after initializing the IO port, call the corresponding IO port configuration without pins;
*If SC92f7323 is selected, the macro definition is not called.
*****************************************************************************/
#define  SC92F7322_NIO_Init()   {P0CON|=0xC0,P1CON|=0x30,P2CON|=0x0C,P5CON|=0x03;}  //SC92F7322 IO port configuration not led out
#define  SC92F7321_NIO_Init()   {P0CON|=0xF0,P1CON|=0x30,P2CON|=0xF0,P5CON|=0x03;}  //SC92F7321 IO port configuration not led out
#define  SC92F7320_NIO_Init()   {P0CON|=0xFF,P1CON|=0xF3,P2CON|=0x3C,P5CON|=0x03;}  //SC92F7320 IO port configuration not led out

#endif