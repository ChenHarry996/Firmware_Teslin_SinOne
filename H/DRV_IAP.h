/*******************************************************************
Copyright 
File name: DRV_IAP.h
Author:  	 Harry.Chen
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
#ifndef _DRV_IAP_H_
#define _DRV_IAP_H_



#define IapROM  0x00	  //0x00£ºROM 0x02:	EEPROM
#define IapEPPROM  0x02	  //0x00£ºROM 0x02:	EEPROM


extern UCHAR IAPRead(UINT Addr,UCHAR IAPArea);
extern void IAPWrite(UINT Addr,UCHAR Value,UCHAR IAPArea);
extern void IAP_Test(void);

#endif
