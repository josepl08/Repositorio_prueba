/*
 * Funciones de Potencia
 * */

#ifndef __Pot
#define __Pot
/* MODULE Events */

//#include "Bit1.h"
#include "AS1.h"
#include "Adq.h"
#include "SPIdev.h"

#include "Events.h"
#include "Cpu.h"
#include "AS1.h"
#include "SCLK.h"
#include "Sync.h"
#include "DataReady0.h"
#include "ADCData.h"
#include "OE0.h"
#include "OE1.h"
#include "CSIC1.h"
#include "CSIC2.h"
#include "SM1.h"
//#include "Bit1.h"
#include "RefreshScreen.h"
#include "CS0.h"
//#include "CS1.h"
//#include "CS2.h"
#include "ACTim.h"
//#include "FC1.h"
#include "Control.h"
#include "Alarmas.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#define ON		1
#define OFF		0

#define HEATCYC		50
#define HEAT100 	100
#define HEATSYNCCYC 6

#define	ENHEAT 	1
#define	HEATER 	2

#define	HUMID	6
#define MOTOR	8

#define	OXIGEN 	8

#define SYNCPULSE 0x01



#define CTISO1	0x31
#define CTISO2	0x32

#define TBGDW1	0x30
#define TBGUP1	0x2f

#define TBGDW2	0x2e
#define TBGUP2	0x2d

#define COLUP1	0x31
#define COLDW1	0x32
//#define TBGPWM	0x2f
//#define TBGEN	0x30


#define SENPTA1	0x40
#define SENPTA2	0x80
#define	SENPTA3 0x01
#define	SENPTA4 0x02
#define TBG0	0x00
#define TBG12N	0x01
#define TBG12P	0x02

#define CALEFSC	0x01

void Pot_GetAlarms(void);
void Pot_HeaterEn(byte stheat);
void Pot_SetTime(void);
void Pot_HeaterSync(void);
void Pot_HeaterPulse(void);
void Pot_HeaterPow(byte setheat,byte powshow);
void Pot_HeaterOn(void);
void Pot_HeatStart(void);
void Pot_HeaterOff(void);
void Pot_MotorOn(void);
void Pot_MotorOff(void);
void Pot_Hum(byte sthum);
void Pot_Column(byte bots);
void Pot_ColumnComand(byte com);
void Pot_Oxy(byte stoxi);
void Pot_sync(void);
void Pot_GetPot(byte *pos,byte type);
void Pot_Tbg(byte tbg);
void Pot_ACIntH(void);
void Pot_GetACStat(void);
void Pot_Test(byte stat);
void Pot_Rec(byte rec);
void Pot_HeaterSC(void);
void Pot_HeaterInit(void);
void Pot_ChkHeat(void);
void Pot_Time(void);
#endif
