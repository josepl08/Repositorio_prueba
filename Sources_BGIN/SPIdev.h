/*
 * Funciones de BUS SPI
 * */

#ifndef __SPIdev
#define __SPIdev
/* MODULE Events */

//#include "Bit1.h"
#include "AS1.h"
#include "Adq.h"
#include "Protocolo.h"
#include "Potencia.h"

#include "Events.h"
#include "Cpu.h"
#include "AS1.h"
#include "Rep.h"
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
#include "CSMem.h"
#include "Control.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
//sstat definitions
#define SBUSY 		1
#define	RECIEVE 	2
//Commands Expanders
#define EXPREAD1	0x8E
#define EXPREAD2	0x8F
#define WRITEALL	0x0A
//Devices
#define POTAC		6
#define	POTCOX		1
#define POTTDPT		0

void SPI_InitDev(void);
void SPI_SendExp(SM1_TComData com, SM1_TComData dat, byte dev, byte config);
void SPI_SendEnAd(SM1_TComData dat, byte dev);
void SPI_ReadMem(word address, byte qty);
void SPI_RecMem(byte *datrec);
void SPI_WriteMem(word address, byte *dat, byte qty);
void SPI_Recieve(void);
void SPI_SendNext(void);
void SPI_LockSync(void);
void SPI_Stat(byte *srep);
void SPI_GetSen(void);
#endif
